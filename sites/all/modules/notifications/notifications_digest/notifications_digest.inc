<?php
// $Id: notifications_queue.class.inc,v 1.1.2.10 2010/07/24 10:16:12 jareyero Exp $
/**
 * Notifications module. Queue processing.
 * 
 * @ TO DO: Support different languages for message localization
 */

/**
 * Queue for digesting notifications
 */
class Notifications_Digest extends Notifications_Queue {
  // Process control variables
  public $process_limit;
  public $process_current;
  public $process_options;
  public $process_max_sqid;
  // Optional language to process only rows in this language. Not used yet.
  // This can be used to run different languages in different crons and prevent static caching issues.
  public $process_language = NULL;  
  /**
   * Start working with a queue.
   *
   * @param $name
   *   Arbitrary string. The name of the queue to work with.
   */
  public function __construct($name = 'notifications_digest') {
    $this->name = $name;
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
   * Process rows given query conditions
   * 
   * This is the main notifications queue processing function, fetching queued notifications,
   * loading all related objects and sending out messages.
   * 
   * @see queue_query()
   * 
   * @param $conditions
   *   Array of query conditions
   * @param $limit
   *   Optional, limit the number of rows to process
   * @param $update
   *   Optional, update queue rows and event counter after processing
   * 
   * @return int
   *   Number of rows processed
   */
  function process_rows($conditions, $limit = 0, $update = TRUE) {
    notifications_log('Processing queue rows', $conditions + array('limit' => $limit));
    $test = $this->process_option('test');
    $count = 0;
    // Build query and fetch rows from queue
    $query = $this->queue_query($conditions);
    $sql = "SELECT * FROM {notifications_queue}";
    $sql .= " WHERE ". implode(' AND ', $query['where']);
    $sql .= " ORDER BY module, mdid, send_method, send_interval";
    if ($limit) {
      $result = db_query_range($sql, $query['args'], 0, $limit);
    }
    else {
      $result = db_query($sql, $query['args']);
    }
  
    // Group rows by module, destination, send_interval before composing and sending
    // This loop has to run a final time after all rows have been fetched
    $last = $pending = NULL;
    while (($queue = db_fetch_object($result)) || $pending) {
      if (!$queue || $last && ($queue->module != $last->module || $queue->mdid != $last->mdid || $queue->send_method != $last->send_method || $queue->send_interval != $last->send_interval || $queue->language != $last->language)) {
        // New destination, send if not the first row and reset
        $count += $this->process_group($pending, $update, $last);
        $pending = NULL;
      }
      if ($queue) {
        $this->process_control('count', 'row');
        // Add queue row to pending list
        $pending[$queue->sqid] = $queue; 
      }
      $last = $queue;
    }
    // Done with queue, update event tracker
    if ($update) {
      Notifications_Event::track_update();
    }
    // Return number of rows processed
    return $count;
  }
  
  /**
   * Process queued rows, send messages, etc, etc...
   * 
   * @param $group
   *   Array of queue rows indexed by destination id, send interval, queue id
   */
  protected function process_group($group, $update, $params) {
    $module = $params->module;
    $send_method = $params->send_method;
    $send_interval = $params->send_interval;
    $destination = Messaging_Destination::load($params->mdid);
    $account = $destination ? $destination->get_account() : NULL;
    
    $test = $this->process_option('test');
    $count = 0;
  
    // Start loading needed objects and process all group.
    $subscriptions = $events = $processed = array();
    if (!$destination) {
      notifications_log('Cannot load destination', (array)$params);
    }
    elseif (!$account) {
      notifications_log('Cannot load account', (array)$params);
    }
    elseif ($account->uid && !$account->status) {
      $account = NULL;
      notifications_log('User account blocked', (array)$params);
    }
    // Process every row, but if we don't have destination or account we just do event tracking
    foreach ($group as $sqid => $queue) {
      $count++;
      $processed[] = $sqid;
      $event = Notifications_Event::track_load($queue->eid);
      if ($destination && $account) {
        if (!$event) {
          notifications_log('Cannot load event', (array)$queue);
        }
        elseif (!$event->user_access($account)) {
          notifications_log('Access denied for event', (array)$queue);
        }
        else {
          // This will take care of duplicated events
          $events[$queue->eid] = $event;
          // We keep track also of subscriptions originating this event
          $subscriptions[$queue->eid][] = $queue->sid;           
        }
      }
    }
  
    if ($events) {
      $messages = $this->process_callback($module, 'process_compose', $send_method, $destination, $events, $subscriptions, $send_interval, $params->language);
      notifications_log('Composed messages', array('number' => count($messages), 'send_method' => $send_method));
      // Note that we pass the testing parameter to notifications_process_send
      if ($messages) {
        $this->process_callback($module, 'process_send', $send_method, $destination, $messages, $test);
      }
      if (!$test) {          
        $this->queue_update_sent($destination->mdid, $send_interval, time(), count($messages));
      }
    }
    if ($processed && $update) {
      $this->queue_done(array('sqid' => $processed));        
    }
    return $count;
  }
  
  /**
   * Process subscriptions queue
   * 
   * The subscriptions queue has the following fields
   * sqid, uid, eid, sid, digest
   * 
   * This function should be able of splitting the whole processing in several steps.
   * It will be called multiple times for each send interval
   * 
   * Messages will be processed for each send interval, send_method, user
   * 
   * @param $send_interval
   *   Send interval to process
   * @param $max_sqid
   *   Max queue id to process
   * 
   * @return Number of rows processed
   */
  function process_queue($send_interval, $max_sqid= NULL) {
    $max_sqid = isset($max_sqid) ? $max_sqid : $this->process_prepare();
    $language = $this->process_language;
    
    notifications_log('Starting queue processing', array('send interval' => $send_interval, 'max sqid' => $max_sqid));
    // Option for test running, marking messages as test, nor updating not sending
    $test = $this->process_option('test');
    // Option for normal running but without updating the queue records
    $keep = $this->process_option('keep');

    // Count processed rows
    $count = 0;
    // For scheduled notifications, send just rows after this time
    $send_time = time();
    // This is the time from which stored rows will be sent
    $timelimit = $send_time - $send_interval;

    // Check remaining rows to process to adjust query limits for both users and rows
    $step_users = NOTIFICATIONS_STEP_USERS;
    $step_rows = NOTIFICATIONS_STEP_ROWS;
    if ($row_limit = $this->process_limit('row')) {
      $remaining_rows = $row_limit - $this->process_control('current', 'row');
      if ($remaining_rows > 0) {
        $step_users = min($remaining_rows, $step_users);
        $step_rows = min($remaining_rows, $step_rows);
      }    
    }
    // Common batch parts for processing rows
    $default_batch = array(
      'cron' => 1,
      'max_sqid' => $max_sqid,
      'send_interval' => $send_interval,
      'send_time_after' => $send_time,
    );
    // Get users to process messages for, with this time interval and ordered by squid
    // Order by last sent for this send interval
    // Note: If we get the users with more messages pending first this may save some time
    $sql_select = "SELECT q.mdid, q.send_method, q.module, COUNT(q.sqid) AS count_rows FROM {notifications_queue} q ";
    $sql_select .= " LEFT JOIN {notifications_sent} su ON q.mdid = su.mdid AND q.send_interval = su.send_interval ";
    $sql_select .= " WHERE q.cron = 1 AND q.send_interval = '%d' AND q.send_time < %d AND q.sqid <= %d";
    $sql_select .= " AND (su.mdid IS NULL OR su.sent < %d) ";
    // Note: the group by su.sent seems to be needed by pgsql
    $sql_group = " GROUP BY q.mdid, q.send_method, q.module, su.sent ORDER BY su.sent";
    // If processing by language some things change
    if ($language) {
      $sql_select .= " AND q.language = '%s' ";
      $default_batch['language'] = $language->language;
      $result = db_query_range($sql_select . $sql_group, $send_interval, $send_time, $max_sqid, $timelimit, $language->language, 0, $step_users);
    } else {
      $result = db_query_range($sql_select . $sql_group, $send_interval, $send_time, $max_sqid, $timelimit, 0, $step_users);
    }
    // We create a bach for each mdid (user, destination, method) and handle it to notifications_process_rows()
    while (($queue = db_fetch_object($result)) && $this->process_control('check')) {
      $module = $queue->module;
      $processed = array();    
      // Process all rows for this user. With some hard limit to prevent process lock ups.
      // In case we have too many rows, we go updating step by step
      if ($queue->count_rows > $step_rows) {
        $limit = $step_rows;
        // Still if we want to keep data, we don't update as we go
        $update = !$keep;
      }
      else {
        $limit = $queue->count_rows;
        $update = FALSE;
      }
      // Prepare batch query for actual row processing
      $batch = $default_batch + array(
        'mdid' => $queue->mdid,
        'send_method' => $queue->send_method,
        'module' => $queue->module,   
      );
  
      notifications_log('Queue processing', $batch);
      // These rows may be processed by a different module. Defaults to notifications_process_rows()
      $processed = $this->process_callback($queue->module, 'process_rows', $batch, $limit, $update);
      $count += $processed;
  
      if ($processed && !$test && !$update && !$keep) {
        $this->queue_done($batch);      
      }
    }
  
    // If not doing a test run, update event counter and return count
    // If doing a test run, return 0 so we don't go through this again
    if (!$test && !$keep) {
      Notifications_Event::track_update();
      return $count;
    }
    else {
      return 0;
    }
  }
  
  /**
   * Update user last time sent for each destination / interval
   * 
   * @param $mdid
   *   Destination id
   * @param $interval
   *   Send interval
   * @param $time
   *   Timestamp, when notifications were sent
   * @param $counter
   *   How many messages were sent
   */
  function queue_update_sent($mdid, $interval, $time, $counter = 1) {
    db_query("UPDATE {notifications_sent} SET sent = %d, counter = counter + %d WHERE mdid = %d AND send_interval = '%d'", $time, $counter, $mdid, $interval);
    if (!db_affected_rows()) {
      $this->queue_init_interval($mdid, $interval, $time, $counter);
    }
  }
  
  /**
   * Init sent time when creating/updating a new subscription. 
   * 
   * This is to avoid delayed notifications (i.e. once a week) to be sent right away
   */
  function queue_init_subscription($subscription) {
    $mdid = $subscription->get_destination()->mdid;
    $interval = $subscription->send_interval;
    // We don't need this for 'immediate' notifications
    if ($interval && !db_result(db_query("SELECT sent FROM {notifications_sent} WHERE mdid = %d AND send_interval = %d", $mdid, $interval))) {
      $this->queue_init_interval($mdid, $interval, time());
    }
  }
  
  /**
   * Init user last time sent for destination, interval
   */
  protected function queue_init_interval($mdid, $interval, $time, $counter = 0) {
    db_query("INSERT INTO {notifications_sent}(mdid, send_interval, sent, counter) VALUES(%d, '%d', %d, %d)", $mdid, $interval, $time, $counter);
  }

  
  /**
   * Message composition.
   * 
   * Processes everything, included templating and digestion and sends message/s.
   * 
   * Adds some more information into $message['notifications'] that may be used by other modules
   *
   * @param $destination
   *   User account to send the notification to
   * @param $events
   *   Array of loaded event objects to be processed
   * @param $subscriptions
   *   Array of arrays of subscription ids (sids) for each event(eid)
   * 
   * @return array()
   *   Array of messages ready for sending out
   */
  function process_compose($send_method, $destination, $events, $subscriptions, $send_interval, $langcode = NULL, $module = 'notifications') {
    notifications_log('Processing for sending', array('method' => $send_method, 'interval' => $send_interval, 'module' => $module, 'events' => count($events)));
    // Build message template that will be used for all produced messages
    $template = $this->message_template(array(
      'language' => $langcode,
      'method' => $send_method,
      'send_interval' => $send_interval,
    ));
    $template->set_destination($destination);

    
    // Find build method for this interval. Check the function is there in case some module has been disabld
    $build_method = $this->build_method($template, array('events' => $events, 'subscriptions' => $subscriptions, 'module' => $module));
    if ($build_method && !empty($build_method['build callback'])) {
      $build_function = $build_method['build callback'];
      $template->digest = !empty($build_method['digest']);
      $template->build_method = $build_method['type'];
    }
    else {
      // Default building function
      $build_function = array($template, 'build_simple');
      $template->build_method = 'simple';
    }
  
    // Invoke building function that will return an array of messages  
    $messages = call_user_func($build_function, $template, $events, $subscriptions, $module);
    return $messages;
  }
  /**
   * Information about digesting method for composing a message
   * 
   * This just calls notifications_process_build_method() with the send_interval, though its purpose 
   * is just to allow overriding and finding a suitable build method having all the information.
   * 
   * @param $template
   *   Message template
   * @param $params
   *   Other parameters for these message: 'events', 'subscriptions', 'module'
   * 
   * @return array()
   *   Ditest information for that interval, or all the information if no interval
   */
  function build_method($template, $params = array()) {
    return notifications_build_method($template->send_interval);
  }
  
  /**
   * Send array of messages through messaging module
   * 
   * @param $destination
   *   Messaging destination to send to
   * @param $messages
   *   Array of messages prepared for sending
   * @param $test
   *   Optional just test composition and formating but do not send
   */
  protected function process_send($method, $destination, $messages) {
    notifications_log('Sending out notifications', array('method' => $method, 'address' => $destination->address, 'messages' => count($messages)));
    $test = $this->process_option('test');
    foreach ($messages as $message) {      
      $this->message_send($method, $destination, $message, $test);
    }
    return $messages;
  }
  
  /**** Retrieving and replacing text parts, interfacing with tokens and messaging module ****/
  
  /**
   * Message sending, pass the message to Messaging back end
   * 
   * @param $account
   *   User account to send the message to
   * @param $message
   *   Message array, will be converted to object
   * @param $send_method
   *   Send method
   * @param $test
   *   Optional, set to TRUE if doing a test run (messages not to be actually sent)
   * 
   * @return boolean
   *   TRUE if sending was successfull
   */
  protected function message_send($method, $destination, $message, $test = FALSE) {
    notifications_debug('Preparing user notification for messaging', array('message' => $message, 'destination' => $destination));
    $message = messaging_message_build($message);
    $message->type = 'notifications';
    $message->test = $test;
    $this->process_control('count', 'message');
    return messaging_message_send_destination($method, $destination, $message);
  }
  
  /**
   * Save item straight to the queue
   */
  function queue_item($item) {
    return drupal_write_record('notifications_queue', $item);
  }

  /**
   * Mark queue rows as done
   * 
   * Either log, if logging enabled, or delete
   */
  function queue_done($params) {
    if (variable_get('notifications_log', 0)) {
      $this->queue_update($params, array('cron' => 0, 'sent' => time()));
    } else {
      $this->queue_delete($params);
    }
  }
  
  /**
   * Update queue rows with defined values
   * 
   * @arg $params
   *   Parameters to select the queue rows for updating. Array of field => value pairs
   * @arg $update
   *   Fields values to update. Array of field => value pairs
   */
  public static function queue_update($fields, $condition) {
    return db_update('notifications_queue')
      ->fields($fields)
      ->condition($condition)
      ->execute();
  }
  
  /**
   * Delete rows from subscriptions queue
   * 
   * Note: Handle with care if wrong params it may delete all rows
   * 
   * @param $params
   *   Array of conditions. If none, all rows for disabled subscriptions will be deleted
   */
  public static function queue_delete($condition) {
    return db_delete('notifications_queue')
      ->condition($condition)
      ->execute();
  }
  
  /**
   * Clean queue for a user and update event tracker
   * 
   * @param $params
   *   Array of conditions. If none, all rows for disabled subscriptions will be deleted
   */
  function queue_clean($condition) {
    $this->queue_delete($condition);
    $this->event_clean(TRUE);
  }

  /**
   * Reset queue, clean everything
   */
  public static function queue_reset() {
    db_query("DELETE FROM {notifications_queue}");
    db_query("DELETE FROM {notifications_event}");
  }
  
  /**
   * Count the number of rows in queue
   */
  public static function queue_count($condition = NULL) {
    $query = db_select('notifications_queue', 'q');
    if ($condition) {
      $query->condition($condition);
    }
    return $query->countQuery()->execute()->fetchField();
  }

  
  /**
   * Clean up event table. Drop expired events for which we don't have pending rows.
   * 
   * @param $update
   *   Update event counter
   */
  public static function event_clean($update = FALSE) {
    // This expiretime will prevent some race condition that occurs when the event is saved but the subs queue not yet populated  
    $expiretime = time() - 60; 
    if ($update) {
      // Update event counter, which keeps the number of notifications pending for each event
      db_query("UPDATE {notifications_event} e SET counter = (SELECT COUNT(*) FROM {notifications_queue} q WHERE q.eid = e.eid ) WHERE e.created < %d", $expiretime);
    }
    db_query("DELETE FROM {notifications_event} WHERE counter = 0 AND created < %d", $expiretime);
    // Delete events with no pending notifications. As events are created sequentially, we use this fact to speed up the query
    db_query("DELETE FROM {notifications_event} WHERE created < %d AND eid < (SELECT MIN(eid) FROM {notifications_queue})", $expiretime);
  }
  
}

/**
 * Process for digested notifications
 */
class Notifications_Digest_Process extends Notifications_Queue_Process {
  /**
   * Default queue name for this process
   */
  public static function queue_name() {
    return 'digest';
  }
}

/**
 * Worker for digested notifications
 * 
 * - Creates a digest batch for each send interval
 */
class Notifications_Digest_Worker extends Notifications_Batch_Worker {
  /**
   * Create batch object of the right class
   */
  public function createBatch() {
    $batch = new Notifications_Digest_Batch();
    $batch->set_conditions($this->batch_conditions);
  }

  /**
   * Process current batch
   * 
   * @param $process Notifications_Queue_Process
   */
  function process_batch($process) {
    foreach ($this->batch as $item) {
      $result = $this->process_item($item);
      $this->count($result ? 'success' : 'error');
    }
    return $this->result;
  }
  /**
   * Process queued item, one at a time
   */
  function process_item($item) {
    $this->count('items');
    $this->deleteItem($item);
    return TRUE;
  }
}

/**
 * Batch for digesting queue items.
 * 
 * - Select items by send_interval, send_method, user
 * - Digest items by event  
 */
class Notifications_Digest_Batch extends Notifications_Queue_Batch {
  public $batch_conditions;

  /**
   * Set batch conditions
   */
  function set_conditions($conditions) {
    $this->batch_conditions = $conditions;
    foreach ($conditions as $key => $value) {
      $this->$key = $value;
    }
  }
  /**
   * Get count query for this batch
   */
  function batch_count() {
    $query = $this->get_queue()
      ->db_select()
      ->condition('expire', 0)
      ->fields('q', $this->group_fields())
      
    
    foreach ($this->group_fields() as $field) {      
      $query->groupBy($field);
    }
    // This needs to be added at the end?
    $query->countQuery();
    $query->addExpression('COUNT(*) AS cuenta');
  }
  /**
   * Get group by fields
   */
  function group_fields() {
    return array('send_interval', 'send_method', 'uid', 'mdid')
  }
  
}