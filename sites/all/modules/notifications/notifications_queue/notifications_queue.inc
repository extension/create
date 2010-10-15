<?php
// $Id: notifications_queue.class.inc,v 1.1.2.10 2010/07/24 10:16:12 jareyero Exp $
/**
 * Notifications module. Queue processing.
 * 
 * @ TO DO: Support different languages for message localization
 */

// Number of users to process for each step
define('NOTIFICATIONS_STEP_ROWS', variable_get('notifications_step_rows', 1000));
define('NOTIFICATIONS_STEP_USERS', variable_get('notifications_step_users', 200));

// Minimum amount of seconds the process will need for clean-up tasks
// Just to make sure that after exhausting cron assigned time we'll have a few spare seconds for some cleanup
// Change this time for allowing more cron time to modules running cron after Notifications
define('NOTIFICATIONS_TIME_MARGIN', variable_get('notifications_time_margin', 5));

/**
 * Queue management and processing
 * 
 * Tables used here: notifications_queue, notifications_sent, notifications_event
 *
 */
class Notifications_Queue implements DrupalReliableQueueInterface {
  // Queue name, will allow different sub-queues in the main queue
  protected $name;
  // Process control variables
  public $process_limit;
  public $process_current;
  public $process_options;
  public $process_max_sqid;


  /**
   * Start working with a queue.
   *
   * @param $name
   *   Arbitrary string. The name of the queue to work with.
   */
  public function __construct($name) {
    $this->name = $name;
  }

  /**
   * Add a queue item and store it directly to the queue.
   *
   * @param $data
   *   Arbitrary data to be associated with the new task in the queue.
   * @return
   *   TRUE if the item was successfully created and was (best effort) added
   *   to the queue, otherwise FALSE. We don't guarantee the item was
   *   committed to disk etc, but as far as we know, the item is now in the
   *   queue.
   */
  public function createItem($data) {
    $data += array(
      'name' => $this->name,
      'created' => time(),
    );
    return drupal_write_record('notifications_queue', $data);  
  }

  /**
   * Retrieve the number of items in the queue.
   *
   * This is intended to provide a "best guess" count of the number of items in
   * the queue. Depending on the implementation and the setup, the accuracy of
   * the results of this function may vary.
   *
   * e.g. On a busy system with a large number of consumers and items, the
   * result might only be valid for a fraction of a second and not provide an
   * accurate representation.
   *
   * @return
   *   An integer estimate of the number of items in the queue.
   */
  public function numberOfItems() {
    return db_query('SELECT COUNT(sqid) FROM {notifications_queue} WHERE name = :name', array(':name' => $this->name))->fetchField();
  }

  /**
   * Claim an item in the queue for processing.
   *
   * @param $lease_time
   *   How long the processing is expected to take in seconds, defaults to an
   *   hour. After this lease expires, the item will be reset and another
   *   consumer can claim the item. For idempotent tasks (which can be run
   *   multiple times without side effects), shorter lease times would result
   *   in lower latency in case a consumer fails. For tasks that should not be
   *   run more than once (non-idempotent), a larger lease time will make it
   *   more rare for a given task to run multiple times in cases of failure,
   *   at the cost of higher latency.
   * @return
   *   On success we return an item object. If the queue is unable to claim an
   *   item it returns false. This implies a best effort to retrieve an item
   *   and either the queue is empty or there is some other non-recoverable
   *   problem.
   */
  public function claimItem($lease_time = 60) {
    // Claim an item by updating its expire fields. If claim is not successful
    // another thread may have claimed the item in the meantime. Therefore loop
    // until an item is successfully claimed or we are reasonably sure there
    // are no unclaimed items left.
    while (TRUE) {
      $item = db_query_range('SELECT * FROM {notifications_queue} q WHERE expire = 0 AND name = :name ORDER BY created ASC', 0, 1, array(':name' => $this->name))->fetchObject();
      if ($item) {
        // Try to update the item. Only one thread can succeed in UPDATEing the
        // same row. We cannot rely on REQUEST_TIME because items might be
        // claimed by a single consumer which runs longer than 1 second. If we
        // continue to use REQUEST_TIME instead of the current time(), we steal
        // time from the lease, and will tend to reset items before the lease
        // should really expire.
        $update = db_update('notifications_queue')
          ->fields(array(
            'expire' => time() + $lease_time,
          ))
          ->condition('sqid', $item->sqid)
          ->condition('expire', 0);
        // If there are affected rows, this update succeeded.
        if ($update->execute()) {
          $item->data = $item->data ? unserialize($item->data) : array();     
          return new Notifications_Queue_Item($item);
        }
      }
      else {
        // No items currently available to claim.
        return FALSE;
      }
    }
  }    
  
  /**
   * Release item for it to be processed later
   */
  public function releaseItem($item) {
    $update = db_update('queue')
      ->fields(array(
        'expire' => 0,
      ))
      ->condition('sqid', $item->sqid);
      return $update->execute();
  }

  /**
   * Delete item from queue
   */
  public function deleteItem($item) {
    db_delete('notifications_queue')
      ->condition('sqid', $item->sqid)
      ->execute();
  }
  
  /**
   * Claim full batch (multiple items) from queue
   */
  public function claimBatch($max_size, $min_size = 1, $conditions = NULL, $lease_time = 60) {
    // Claim an item by updating its expire fields. If claim is not successful
    // another thread may have claimed the item in the meantime. Therefore loop
    // until an item is successfully claimed or we are reasonably sure there
    // are no unclaimed items left.
    $query = db_select('notifications_queue', 'q')
      ->fields('q')
      ->condition('expire', 0)
      ->condition('name', $this->name)
      ->orderBy('created', 'ASC')
      ->range(0, $max_size);
    if ($conditions) {
      $query->condition($conditions);
    }
    while (TRUE && lock_adquire('notifications_queue_batch')) {
      if ($items = $query->execute()->fetchAll()) {
        // Try to update the items. Only one thread can succeed in UPDATEing the
        // same row.
        foreach ($items as $index => $item) {
          $update = db_update('notifications_queue')
            ->fields(array(
              'expire' => time() + $lease_time,
            ))
            ->condition('sqid', $item->sqid)
            ->condition('expire', 0);
          // If there are affected rows, this update succeeded.
          if ($update->execute()) {
            $item->data = $item->data ? unserialize($item->data) : array();
          }
          else {
            unset($items[$index]);
          }
        }
        if ($items) {
          lock_release('notifications_queue_batch');
          return $items;
        }
      }
      else {
        lock_release('notifications_queue_batch');
        return FALSE;
      }
    }    
  }
  /**
   * Release a full batch (multiple items)
   */
  public function releaseBatch($batch) {
    return $this->db_update()
      ->condition('sqid', array_keys($this->get_items()))
      ->fields(array('expire' => 0))
      ->execute(); 
  }
  /**
   * Delete a full batch (multiple items)
   */
  public function deleteBatch($batch) {
    return $this->db_delete()
      ->condition('sqid', array_keys($this->get_items()))
      ->execute(); 
  }
  public function createQueue() {
    // All tasks are stored in a single database table so there is nothing we need
    // to do to createa new queue.
  }

  /**
   * Delete a queue and every item in the queue.
   */
  public function deleteQueue() {
    $this->db_delete()->execute();
  }

  /**
   * Get select * query for this queue
   */
  function db_select() {
    return db_select('notifications_queue', 'q')->condition('name', $this->name);
  }
  /**
   * Get select * query for this queue
   */
  function db_select_all() {
    return $this->db_select()->fields('q');
  }
  /**
   * Get update * query for this queue
   */
  function db_update() {
    return db_update('notifications_queue')->condition('name', $this->name);
  }
  /**
   * Get delete * query for this queue
   */
  function db_delete() {
    return db_delete('notifications_queue')->condition('name', $this->name);
  }
  /**
   * Callback for Drupal cron
   */
  function process_cron() {
    $this->process_clean();
    $this->process_prepare();
    $this->process_control('cron');
    $this->process_run();
  }

  /**
   * Function to be called on cron by the main notifications_cron
   * 
   * It will call each subscription_process for each interval a number of times
   * 
   * This should send out messages starting with immediate delivery. We send first immediate delivery
   * because the other ones can be added up for each period. 
   * Assumption: The bigger the interval, the longer delay it may admit (?) I.e. sending hourly email 
   * after 1 hour 15 mins may be ok if the system is overloaded.
   * 
   * @return int
   *   Number of rows processed
   */
  function process_run() {
    notifications_log('Starting notifications process');

    $count = 0;
    $stop = FALSE;
    $send_intervals = _notifications_queue_send_intervals();
    unset($send_intervals[-1]);
    
    if ($max_sqid = $this->process_prepare()) {
      foreach ($send_intervals as $interval => $name) {
        notifications_log('Processing queue', array('send interval' => $name));
        while ($rows = $this->process_queue($interval, $max_sqid)) {
          $count += $rows;
          $stop = !$this->process_control('check');
        }
        if ($stop) {
          notifications_log('Process stopped, reached processing limits');
          break;
        } else {
          notifications_log('Process finished', array('send interval' => $name));
        }
      }
    } else {
      notifications_log('No rows in queue');
    }
    $this->process_control('stop');
    return $count;
  }
  
  /**
   * Prepare subscriptions queue
   * 
   * This is intended to avoid race conditions where new rows are added while the process is running
   * 
   * @return
   *   Max $sqid that will be processed this cron
   */
  function process_prepare() {
    if (!isset($this->process_max_sqid)) {
      $this->process_control('start');
      // This will get the latest notification in queue so we don't mess with new ones being created during cron run
      // It will also prevent clashes with the immediate sending feature
      $this->process_max_sqid = db_result(db_query("SELECT max(sqid) FROM {notifications_queue}"));
    }
    return $this->process_max_sqid;
  }
  
  /**
   * Clean up queue and events before starting process
   */
  public static function process_clean() {
    // Clean up expired logs from queue if logging enabled
    if ($keep = variable_get('notifications_log', 0)) {
      db_query("DELETE FROM {notifications_queue} WHERE cron = 0 AND sent < %d", time() - $keep);
    }
    // Clean up event table
    self::event_clean();   
  }
  


  /**
   * Queue events for notifications adding query conditions from plug-ins
   * 
   * This queues in different queues, depending on the 'queue' value in notificaitons table
   * 
   * This is an example of the resulting query
   *
   * INSERT INTO {notifications_queue} (uid, sid, module, eid, send_interval, send_method, cron, created, conditions)
   * SELECT DISTINCT s.uid, s.sid, s.module, 34, s.send_interval, s.send_method, s.cron, 1230578161, s.conditions FROM notifications s 
   * INNER JOIN notifications_fields f ON s.sid = f.sid 
   * WHERE s.status = 1 AND s.event_type = 'node' AND s.send_interval >= 0 
   * AND ((f.field = 'nid' AND f.value = '2') OR (f.field = 'type' AND f.value = 'story') OR (f.field = 'author' AND f.value = '1'))
   * GROUP BY s.uid, s.sid, s.module, s.send_interval, s.send_method, s.cron, s.conditions 
   * HAVING s.conditions = count(f.sid)
   *
   * @param $event
   *   Event object
   * 
   * @return int
   *   Number of queued rows
   */
  public static function queue_event($event) {
    $subscription_types = $event->get_subscription_types();
    if ($event_conditions = $event->get_conditions($subscription_types)) {
      $insert = db_insert('notifications_queue')
        ->fields(array('name', 'uid', 'mdid', 'send_method', 'sid', 'send_interval', 'language', 'conditions', 'created', 'eid'));
      $select = db_select('notifications_subscription', 's')
        ->distinct()
        ->fields('s', array('queue', 'uid', 'mdid', 'send_method','sid', 'send_interval', 'language', 'conditions'));
      $select->addExpression(REQUEST_TIME);
      $select->addExpression($event->eid);
      $select->innerJoin('notifications_subscription_fields', 'f', 's.sid = f.sid');
      $select->groupBy('uid')
        ->groupBy('sid')
        ->groupBy('send_interval')
        ->groupBy('send_method');
      $select->condition('status', 1)
        ->condition('send_interval', 1, '>=');
      // Add all field conditions
      $fields = db_or();
      foreach ($event_conditions as $condition) {
        $fields->condition($condition);
      }
      $select->condition(db_and()->condition($fields));
      // Add metadata for altering and run
      $select->addTag('notifications queue event');
      $select->addMetaData('event', $event);
      $insert->from($select)->execute();
    }
    // Return number of queued rows
    return db_result(db_query('SELECT COUNT(*) FROM {notifications_queue} WHERE eid = :eid', array(':eid' => $event->eid)));
  }
  /**
   * Get default cron process for this queue
   * 
   * @return Notifications_Cron_Process
   */
  function default_process() {
    return Notifications_Queue_Process::build()
      ->set_limits($limits)
      ->add_worker(new Notifications_Queue_Worker($this));
  }
  /**
   * Acquire lock for this queue
   */
  function lock_acquire($name) {
    return lock_acquire('notifications_queue-' . $this->name . '-' . $name);
  }
  /**
   * Release lock for this queue
   */
  function lock_release($name) {
    return lock_release('notifications_queue-' . $this->name . '-' . $name);
  }
}

/**
 * Items from the queue
 */
class Notifications_Queue_Item {
  // Queue parameters
  public $sqid;
  public $name;
  public $expired;
  public $created;
  public $data;
  // Subscription parameters
  public $sid;
  public $uid;
  public $mdid;
  public $language;
  public $type;
  public $send_interval;
  public $send_method;
  public $send_time;
  public $conditions;
  public $destination;
  // Event parameters
  public $eid;
  // Queue and batch objects it belongs to
  protected $queue;
  protected $batch;
  // Temporary store
  protected $events;
  protected $subscriptions;

  /**
   * Constructor
   */
  public function __construct($template = array()) {
    foreach ((array)$template as $key => $value) {
      $this->$key = $value;
    }
  }
  /**
   * Get queue object
   */
  function get_queue() {
    return $this->queue();
  }
  /**
   * Delete item
   */
  function delete() {
    $this->get_queue()->deleteItem($this);
  }
  /**
   * Release item
   */
  function release() {
    $this->get_queue()->releaseItem($this);
  }
  /**
   * Process item: Build and send message
   */
  function process() {
    if ($event = $this->get_event($this->eid)) {
      return $event->build_template($this)
        ->build_message($this)
        ->send();
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get event object
   */
  function get_event($eid) {
    return Notifications_Event::load($eid);
  }
}

/**
 * Queue process
 */
class Notifications_Queue_Process {
  public $batch_size = 10;
  protected $limit = array();
  protected $current = array();
  protected static $options;
  // Optional language to process only rows in this language. Not used yet.
  // This can be used to run different languages in different crons and prent static caching issues.
  public $process_language = NULL;
  // Start time
  protected $start = 0;
  /**
   * Constructor from Queue and Worker
   * 
   * @param unknown_type $queue
   * @param unknown_type $worker
   */
  public function __construct() {
    $this->limit = $this->default_limits();
    $this->option('batch_size', $this->batch_size);
    $this->option('language', $this->process_language);
  }
  /**
   * Add worker to the process
   */
  public function add_worker($worker) {
    $this->workers[] = $worker;
    return $this;
  }
  /**
   * Default queue name for this process
   */
  public static function queue_name() {
    return 'queue';
  }
  /**
   * Build with default values
   */
  public static function build() {
    return new Notifications_Queue_Process();
  }
  /**
   * Start process
   */
  public function start() {
    $this->start = time();
    $this->current = array('items' => 0, 'messages' => 0);
  }

  /**
   * Run process
   */
  public function run() {
    $this->start();
    while ($this->check()) {
      foreach ($this->workers as $key => $worker) {
        if ($worker->fetch($this->limit)) {
          $results = $worker->process($this->limit, $this->options);
          $this->add_results($results);
        }
        else {
          // The worker finished it's work, drop it
          $worker->done();
          unset($this->workers[$key]);
        }
      }
    }
    return $this;
  }
  /**
   * Check we are under limits and we still have workers
   */
  function check() {
    $current = $this->current;
    $current['time'] = time();
    $current['process'] = time() - $this->start;
    foreach ($this->limit as $key => $value) {
      if (isset($current[$key]) && $current[$key] > $value) {
        return FALSE;
      }
    }
    return !empty($this->workers);
  }
  /**
   * Get current value
   */
  function current($type) {
    return isset($this->current[$type]) ? $this->current[$type] : 0;
  }
  /**
   * Add results to current values
   */
  function add_results($results) {
    foreach ($results as $key => $value) {
      $this->current[$key] = $this->current($key) + $value;
    }
  }
  /**
   * Set / unset limit for the process
   * 
   * @param $type
   *   - item, Number of items to be processed
   *   - message, Number of messages to be sent
   *   - process, Total processing time
   *   - time, Fixed time to finish
   */
  function set_limit($type, $value = NULL) {
    if (isset($value)) {
      $this->limit[$type] = $value;
    }
    elseif (isset($this->limit[$type])) {
      unset($this->limit[$type]);
    }
    return $this;
  }
  /**
   * Set array of limits
   */
  function set_limits($limits) {
    foreach ($limits as $type => $value) {
      $this->set_limit($type, $value);
    }
    return $this;
  }
  /**
   * Default limits
   */
  protected function default_limits() {
    return variable_get('notifications_process_limits', array('items' => 10, 'messages' => 10, 'process' => 60));
  }
  /**
   * Get / set process option (static = same for all processes)
   */
  public static function option($name, $value = NULL) {
    // Get / set value for option
    if (isset($value)) {
      self::$options[$name] = $value;
    }
    else {
      return isset(self::$options[$name]) ? self::$options[$name] : FALSE;
    }    
  }
  /**
   * Return array of variables needed to print out some messages
   */ 
  function get_results() {
    return array(
      '@rows' => $this->current['items'],
      '@messages' => $this->current['messages'],
      '@time' => time() - $this->start,
    );
  }
  /**
   * Start process from cron
   */
  public function cron() {
    $this->cron_prepare();
    return $this->run();
  }
  /**
   * Prepare process for cron
   */
  public function cron_prepare() {
    // Calculate time limit. We get the smaller of all these times in seconds
    // There's an issue with poormanscron not setting the cron semaphore so it will default to current time
    $timelimit = array();
    $cronstart = REQUEST_TIME;     
    // Max execution time may be zero meaning no limit, then no limits based on this
    if ($maxtime = ini_get('max_execution_time')) {
      $timelimit[] =  $cronstart + $maxtime - NOTIFICATIONS_TIME_MARGIN;
    }
    // This is an absolute limit, applies always if set
    if (!empty($this->limits['process'])) {
      $timelimit[] = time() + $this->limits['process'];
    }
    if ($timelimit) {
      $this->limits['time'] = min($timelimit);
    }
    return $this;  
  }
}

/**
 * Queue worker. This base class processes items one at a time
 */
class Notifications_Queue_Worker {
  protected $queue;
  // Items currently being processed
  protected $process;
  // Results for this process
  protected $result = array();
  // 
  protected $options;
  // Unique id for this worker, see _lock_id() 
  protected $uniqueid;

  public function __construct($queue, $options = array()) {
    $this->queue = $queue;
    $this->options = $options + $this->default_options();
  }
  /**
   * Get item to process, one at a time
   */
  function fetch($limits) {
    $this->reset();
    $this->process = $this->queue->claimItem($this->options['lease_time']);
    return $this->process;
  } 
  /**
   * Process current item / batch
   * 
   * @param $process Notifications_Queue_Process
   */
  function process($limits) {
    $result = $this->process_item($item);
    $this->count($result ? 'success' : 'error');
    return $this->result;
  }
  /**
   * The worker has finished and may need some clean up
   */
  function done() {
    return TRUE;
  }
  /**
   * Process queued item, one at a time
   */
  function process_item($item) {
    $this->count('items');
    $result = $item->process();
    $item->delete();
    return $result;
  }
  /**
   * Reset batch results
   */
  function reset() {
    $this->process = NULL;
    $this->result = array('items' => 0, 'messages' => 0, 'success' => 0, 'error' => 0);
  }
  /**
   * Increase counter
   */
  function count($type, $number = 1) {
    $this->result[$type] += $number;
  }
  /**
   * Get unique id for this worker
   */
  function uniqueid() {
    if (!isset($this->uniqueid)) {
      $this->uniqueid = uniqid(mt_rand(), TRUE);
    }
    return $this->uniqueid;
  }
  /**
   * Get default options
   */
  protected function default_options() {
    return array('lease_time' => 60);
  }
}

/**
 * Queue worker. Processes batches
 */
abstract class Notifications_Batch_Worker extends Notifications_Queue_Worker{
  /**
   * Create a new batch object
   */
  abstract function create_batch();
  /**
   * Get item to process, one at a time
   */
  function fetch($limits) {
    $this->reset();
    $this->process = $this->claim($limits);
    return $this->process;
  }
  /**
   * Claim items from the queue
   */
  function claim($limits) {
    $batch_size = $options['batch_size'];
    $this->queue->claimBatch($this->create_batch());
  }
  /**
   * Process current batch
   * 
   * @param $process Notifications_Queue_Process
   */
  function process($limits) {
    $this->result = $this->process_batch($this->process);
    return $this->result;
  }
  /**
   * Process queued item, one at a time
   * 
   * @return process result array
   */
  function process_batch($batch, $result = array()) {
    $result = $batch->process();
    $result['items'] = $this->count('items', $batch->count());
    $batch->delete();
    return $result;
  }
  /**
   * Get default options
   */
  protected function default_options() {
    return array('batch_size' => 10) + parent::default_options();
  }
}

/**
 * Each one of the elements of the queue
 * 
 * Items are grouped by Event so each group corresponds to an Event
 */
class Notifications_Queue_Batch extends Notifications_Queue_Item {
  protected $items;
  protected $events;
  protected $subscriptions;
  // Array of field => value pairs used as criteria for this batch
  public $criteria;
  
  /**
   * Add each item
   */
  function add_item($item) {
    $this->items[$item->qid] = $item;
  }
  /**
   * Get all pending
   */
  function get_items() {
    return $this->items;
  }
  /**
   * Count number of items
   */
  function count() {
    return !empty($this->items) ? count($this->items) : 0;
  }
  /**
   * Delete all items
   */
  function delete() {
    return $this->get_queue()->deleteBatch($this);   
  }
  /**
   * Release all items
   */
  function release() {
    return $this->get_queue()->releaseBatch($this);
  }
  /**
   * Get key, unique per batch type
   */
  function get_key() {
    return 'notifications_queue_batch';
  }
  /**
   * Get batch conditions
   */
  function batch_conditions() {
  }
}