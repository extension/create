<?php
// $Id: messaging_store.class.inc,v 1.1.2.8 2010/05/15 16:42:19 jareyero Exp $
/**
 * @file
 *   Database storage for the messaging framework
 */

// Max number of rows to process for each step before clean up
define('MESSAGING_STEP_ROWS', variable_get('messaging_step_rows', 1000));

// Minimum amount of seconds the process will need for clean-up tasks
// Just to make sure that after exhausting cron assigned time we'll have a few spare seconds for some cleanup
define('MESSAGING_TIME_MARGIN', variable_get('messaging_time_margin', 5));

/**
 * Default storage and queueing system for Messaging
 * 
 * This class has only static methods that will be invoked through 'messaing_store' function
 */
class Messaging_Store implements DrupalReliableQueueInterface {
  const STEP_ROWS = MESSAGING_STEP_ROWS;
  const TIME_MARGIN = MESSAGING_TIME_MARGIN;
  // Queue name, will allow different sub-queues in the main queue
  protected $name;

  /**
   * Start working with a queue.
   *
   * @param $name
   *   Arbitrary string. The name of the queue to work with.
   */
  public function __construct($name = 'notifications') {
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
  public function createItem($message) {
    $data += array(
      'name' => $this->name,
      'created' => time(),
    );
    $result = drupal_write_record('messaging_store', $data);
    if (!empty($data['mqid'])) {
      $message->mqid = $data['mqid'];
    }
    return $result;
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
    return db_query('SELECT COUNT(mqid) FROM {messaging_store} WHERE name = :name', array(':name' => $this->name))->fetchField();
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
      $item = db_query_range('SELECT * FROM {messaging_store} q WHERE expire = 0 AND name = :name ORDER BY created ASC', 0, 1, array(':name' => $this->name))->fetchObject();
      if ($item) {
        // Try to update the item. Only one thread can succeed in UPDATEing the
        // same row. We cannot rely on REQUEST_TIME because items might be
        // claimed by a single consumer which runs longer than 1 second. If we
        // continue to use REQUEST_TIME instead of the current time(), we steal
        // time from the lease, and will tend to reset items before the lease
        // should really expire.
        $update = db_update('messaging_store')
          ->fields(array(
            'expire' => time() + $lease_time,
          ))
          ->condition('mqid', $item->mqid)
          ->condition('expire', 0);
        // If there are affected rows, this update succeeded.
        if ($update->execute()) {
          $item->data = $item->data ? unserialize($item->data) : array();     
          return $item;
        }
      }
      else {
        // No items currently available to claim.
        return FALSE;
      }
    }
  }    

  public function releaseItem($item) {
    $update = db_update('queue')
      ->fields(array(
        'expire' => 0,
      ))
      ->condition('mqid', $item->mqid);
      return $update->execute();
  }

  public function deleteItem($item) {
    db_delete('messaging_store')
      ->condition('mqid', $item->mqid)
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
    db_delete('messaging_store')
      ->condition('name', $this->name)
      ->execute();  
  }
  
  /**
   * Capabilities: whether this Queue can expire old messages
   */
  public static function can_expire() {
    return TRUE;
  }
  
  /**
   * Queue message for next delivery
   */
  public function message_queue($message) {
    $message->queue = 1;
    $message->queued = TRUE;
    $message->store = $this->name;
    // Set created date if not previously set
    if (empty($message->created)) {
      $message->created = time();
    }
    return $this->createItem($message);
  }
  
  /**
   * Count queued messages
   */
  public function queue_count() {
    return $this->numberOfItems();
  }
  /**
   * Process single messag from queue. Note we change visibility for this method
   */
  public static function queue_process_message($message) {
    // Do not queue again but send out
    $message->queue = 0;
    $result = $message->send();
    // If it needs logging use the default messaging_store
    if ($message->log && !$message->logged) {
      $message->log();
    }
    return $result;
  }

  /**
   * Process and send messages in queue, to be called from cron
   * 
   * It will check for predefined limits and repeat the cycle
   *   [fetch] -> [send] -> [check]
   * until the queue is empty or any of the limits are met
   * 
   * The limits array may contain any of these conditions:
   * - time, absolute max execution time
   * - timeout, calculated time out (like for cron, based on the time we've been already running)
   * - message, max number of messages sent
   * - percent, max % of page execution time that can be spent on cron processing
   * 
   * @param $limits
   *   Optional limits for queue processing
   * @return
   *   Array of results indexed by message id
   */
  public function queue_process($limits = array()) {
    $results = array();
    $limit = self::process_limits($limits);
    return $this->queue_process_step($limit['message'], $limit['timeout']);
  }
  /**
   * Retrieve and send queued messages
   * 
   * @param $limit
   *   Maximum number of queued messages to process for this step
   * @param $timeout
   *   Optional time limit for processing, will return when if reached during processing
   * @return
   *   Array of sending results indexed by message id
   */
  protected static function queue_process_step($limit, $timeout = 0) {
    $count = 0;
    $sent = $unsent = $processed = array();
    while ((!$limit || $count <= $limit) && (!$timeout || time() <= $timeout) && ($item = $this->claimItem())) {
      $message = $item->data;
      $success = $this->queue_process_message($message);
      $queue->deleteItem($item);

      $processed[] = $success;
      if ($success) {
        $sent[] = $message->mqid;
        messaging_debug('Processed message from queue', array('message' => $message, 'success' => $success));
      } else {
        $unsent[] = $message->mqid;
        watchdog('messaging', 'Failed queue processing for @message', array('@message' => (string)$message), WATCHDOG_WARNING);
      }
      $count++;
    }
    return $processed;
  }
 
  /**
   * Get status summary
   * 
   * @param $fields
   *   Fields to queue and group by
   * @param $conditions
   *   Array of field conditions to restrict the query
   *   
   * @return array
   *   Array of arrays with the status fields and a 'total' counter for each row
   */
  public static function get_status($fields, $conditions = array()) {
    $status = array();
    $group = $fields;
    $fields[] = 'count(*) AS total';
    $result = self::query_select($fields, $conditions, array('group' => $group))->execute();  
    while ($data = db_fetch_array($result)) {
      $status[] = $data;
    }
    return $status;
  }  
  /**
   * Delete all message from queue and logs
   */
  public static function delete_all() {
    return db_delete('messaging_store')->execute();
  }
  
  /**
   * Process messages on cron
   */
  public function cron_process() {
    // Pass on a time out condition, that will be based on 'max_execution_time'
    $limit['timeout'] = variable_get('cron_semaphore', 0) + ini_get('max_execution_time') - self::TIME_MARGIN;
    $this->queue_process($limit);
    self::queue_expire_messages();
    self::queue_expire_logs();
  }
  
  /**
   * Calculate limits for queue processing
   */
  public static function process_limits($limits = array()) {
    $limits += variable_get('messaging_process_limit', array('message' => 0, 'time' => 0, 'percent' => MESSAGING_DEFAULT_CRON_PERCENT));
    // Calculate time limit. We get the smaller of all these times in seconds
    if (!empty($limit['timeout'])) {
      $times[] = $limit['timeout'];
    }
    if (!empty($limit['time'])) {
      $times[] = time() + $limit['time'];
    }
    if (!empty($limit['percent'])) {
      $times[] = time() + ini_get('max_execution_time') * $limit['percent'] / 100;
    }
    $limits['timeout'] = !empty($times) ? min($times) : 0; 
    return $limits;  
  }

  /**
   * Queue clean up
   * - Remove expired logs (include errors)
   * - Remove expired queued messages
   */
  public static function queue_clean() {
    $count = self::queue_expire_messages();
    return $count;
  }

  /**
   * Remove expired messages from queue
   */
  public function expire_messages() {
    if ($expire_messages = variable_get('messaging_queue_expire', 0)) {
      $time = time() - $expire_messages;
      return db_delete('messaging_store')
        ->condition('name', $this->name)
        ->conditions('created', $time, '<')
        ->execute();
    }
    else {
      return 0;
    }
  }

  /**
   * Run select query with given parameters
   * 
   * @param $fields
   *   Array of field names for SELECT
   * @param $conditions
   *   Array of query conditions
   * @param $query_params
   *   Mixed query parameters (limit, pager, group....)
   */
  public function query_select($conditions = array()) {
    $conditions += array('name' => $this->name);
    $query = db_select('messaging_store', 'm');
    foreach ($conditions as $field => $value) {
      $query->condition($field, $value);
    }
    return $query->execute();
  }

  /** 
   * Load single message from store 
   */
  public function message_load($msid) {
    $message = $this->query_select(array(), array('msid' => $mqid))->fetchObject();
    $message->store = $this->name;
    return $message;
  }
  /**
   * Load full message from wherever it is
   */
  public static function load($msid) {
    $message = Messating_Message::load($msid);
    if ($message && !empty($message->store) && ($store = messaging_store($message->store))) {
      if ($stored = $store->load($msid)) {
        return $stored;
      }
    }
    return $message;
  }
  /**
   * Load multiple messages
   */
  public static function load_multiple($msids = array(), $conditions = array()) {
    return entity_load('notifications_message', $eids, $conditions);
  }
  
  /**
   * Mark messages as sent, either deleting them, or keeping logs
   * 
   * @param $mqid
   *   Single message id or array of message ids
   * @param $error
   *   Optional, just mark as error move queue messages to log, for messages on which sending failed 
   */
  public function message_sent($mqid, $error = FALSE) {
    $where = self::query_fields(array('mqid' => $mqid));
    if ($error) {
      // Error, log them all, sent = 0
      $sent = 0;
    } else {
      // First delete the ones that are not for logging, then mark as sent
      db_query("DELETE FROM {messaging_store} WHERE log = 0 AND ".implode(' AND ', $where['where']) , $where['args']);
      $sent = time();
    }
  
    // Now unmark the rest for queue processing, as logs
    $args = array_merge(array($sent), $where['args']);
    db_query("UPDATE {messaging_store} SET queue = 0, cron = 0, log = 1, sent = %d WHERE ".implode(' AND ', $where['where']), $args);
  }
  
  /**
   * Delete multiple messages from queue
   */
  public static function delete_multiple($params) {
    $query = db_delete('messaging_store');
    foreach ($params as $field => $value) {
      $query->condition($field, $value);
    }
    return $query->execute();
  }
 

  /**
   * Delete single message from all queues
   */
  public function message_delete($message) {
    return db_delete('messaging_store')
      ->condition('name', $this->name)    
      ->condition('mqid', $message->mqid)
      ->execute();
  }

  /**
   * Put into database storage, create one line for each destination
   * 
   * If there are many destinations they will be stored as 'multiple'
   * 
   * @param $message
   *   Message object
   * @return int
   *   Result from drupal_write_record: SAVED_NEW, SAVED_UPDATED, FALSE
   */
  public function message_save($message) {
    // Check we have a message object
    $result = $this->createItem($message);
    messaging_debug('Saved message to store', array('message' => $message));
    return $result;
  }
  
  /**
   * Get help for admin pages
   */
  public static function admin_help() {
    return array(
      'name' => t('Messaging Queue.'),
      'description' => t('It uses <i>Drupal Queue</i> module for handling queued messages.'),
      'queue' => t('Queued messages can be processed on cron or using multiple workers with <i>drush queue-cron</i>.'),
      'logs' => t('Logs will be kept if enabled using the <i>Messaging Store</i> tables.'),
    );
  }

  /**
   * Get more settings for Admin page
   */
  public static function admin_settings() {
  }
}