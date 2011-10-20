<?php

/**
 * @file
 * Subscriptions module mail gateway (cron functions).
 */

/**
 * Implementation of hook_cron().
 *
 * Takes items from {subscriptions_queue} and generates notification emails.
 */
function _subscriptions_mail_cron() {

  global $user, $language;
  drupal_language_initialize();
  _subscriptions_mail_module_load_include('templates.inc');

  $mails_allowed = variable_get('subscriptions_number_of_mails', 0);
  $from = _subscriptions_mail_site_mail();
  $single_count = $single_failed = $digest_count = $digest_failed = 0;
  $loaded_objects = array();
  $fields = array();

  // Strategy for cron:
  // Use a defined percentage of the total cron time (default: 50%), but leave at least 5s.
  $total_seconds = ini_get('max_execution_time');
  $total_seconds = (empty($total_seconds) ? 240 : $total_seconds);
  $lost_seconds = timer_read('page')/1000;
  $available_seconds = $total_seconds - $lost_seconds;
  $usable_seconds = min(array($available_seconds - 5, $total_seconds*subscriptions_mail_get_cron_percentage()/100));
  //TEST: watchdog('cron', "Subscriptions has $available_seconds of $total_seconds seconds available.");
  if ($usable_seconds <= 0) {
    $variables = array(
      '!Module' => 'Subscriptions',
      '@link'   => url('admin/config/subscriptions', array('fragment' => 'edit-subscriptions-cron-percent')),
    );
    $watchdog = 'watchdog';  // keep potx from translating 'cron'
    if ($usable_seconds == 0) {
      $watchdog('cron', t('!Module cannot send any notifications because its <a href="@link">cron job time</a> is 0!', $variables), NULL, WATCHDOG_WARNING);
    }
    else {
      $watchdog('cron', t('!Module cannot send any notifications because the cron run has less than 5 available seconds left!', $variables), NULL, WATCHDOG_WARNING);
    }
  }

  while ((empty($mails_allowed) || $single_count + $digest_count < $mails_allowed)
          && timer_read('page')/1000 < $lost_seconds + $usable_seconds) {
    $result = db_query_range("SELECT * FROM {subscriptions_queue} WHERE suspended = 0 AND last_sent + send_interval < :time ORDER BY sqid", 0, 1, array(':time' => time()));
    if (!($subs = $result->fetchAssoc())) {
      break;  // No more ready queue items, terminate loop.
    }

    $saved_user = $user;
    $saved_language = $language;
    drupal_save_session(FALSE);
    $user = user_load($subs['uid']);
    $lang_code = $language->language;
    $digest_data = array();

    do {  // once and repeat while adding to a digest
      if ($user->status && $user->access) {
        $subs['mail'] = $user->mail;
        $load_function = $subs['load_function'];
        $load_args = $subs['load_args'];
        if (!isset($loaded_objects[$user->uid][$load_function][$load_args]) && $load_function($subs)) {
          $object = $subs['object'];
          $access = module_invoke_all('subscriptions', 'access', $load_function, $load_args, $object);
          // One FALSE vote is enough to deny. Also, we need a non-empty array.
          $allow = !empty($access) && array_search(FALSE, $access) === FALSE;
          $loaded_objects[$user->uid][$load_function][$load_args] = $allow ? $object : FALSE;
        }
        if ($object = $loaded_objects[$user->uid][$load_function][$load_args]) {
          $module = $subs['module'];
          $ori_field = $field = $subs['field'];
          $ori_value = $value = $subs['value'];
          if (!isset($fields[$lang_code][$module])) {
            $fields[$lang_code][$module] = module_invoke_all('subscriptions', 'fields', $module);
          }
          $data_function = $fields[$lang_code][$module][$field]['data_function'];
          $mailmod = (empty($fields[$lang_code][$module][$field]['mail_module']) ? 'subscriptions_mail' : $fields[$lang_code][$module][$field]['mail_module']);
          $mailkey = $fields[$lang_code][$module][$field]['mailkey'];
          $mailkey = module_invoke_all('subscriptions', 'mailkey_alter', $mailkey, $object);
          $mailkey = $mailkey[0];
          $digest = $subs['digest'] > 0 || $subs['digest'] == -1 && _subscriptions_get_setting('digest', 0) > 0;
          //$show_node_info = (isset($object->type) ? variable_get('node_submitted_' . $object->type, TRUE) : TRUE);

          $data = array(
            'subs' => array(
              'type'             => $fields[$lang_code][$module][$field]['subs_type'],
              'unsubscribe_path' => "s/del/$module/$ori_field/$ori_value/" . $subs['author_uid'] . '/' . $subs['uid'] . '/' . md5(drupal_get_private_key() . $module . $ori_field . $ori_value . $subs['author_uid'] . $subs['uid']),
            ),
            'user' => user_load(!empty($object->revision_uid) ? $object->revision_uid : $object->uid),
          );
          $data_function($data, $object, $subs);
          drupal_alter('subscriptions_data', $data, $object, $subs);
          //mail_edit_format($values['subscriptions_comment_body'], $data + array('comment' => $comment), array('language' => $language));
          if ($digest && !empty($object->_subscriptions_comments) && module_exists('subscriptions_content')) {
            $digest_comment_template = subscriptions_mail_template_load($mailmod, SUBSCRIPTIONS_DIGEST_MAILKEY . '+comment', $lang_code, 'body', 'DITEMCMT');
            $data['subs:comments'] = subscriptions_content_format_comments($object, $digest_comment_template, '');
          }
          if ($digest) {
            $digest_data['bodies'][] = $body;
            $digest_data['send'] = array(
              'name' => $subs['name'],
              'mail' => $subs['mail'],
              'from' => $from,
              '!name' => $mailvars['!name'],
              '!manage_url' => $mailvars['!manage_url'],
            );
            $digest_data['send_intervals'][$subs['send_interval']] = $subs['send_interval'];
          }
          else {
            (_subscriptions_mail_send($mailmod, $mailkey, $subs['name'], $subs['mail'], $from, $subs['uid'], array($subs['send_interval']), $data)
              ? ++$single_count
              : ++$single_failed);
          }
        }
      }
      db_delete('subscriptions_queue')
        ->condition('load_function', $subs['load_function'])
        ->condition('load_args', $subs['load_args'])
        ->condition('uid', $subs['uid'])
        ->execute();

      if (!empty($digest)) {
        // Get next ready queue item for this user.
        $result = db_query_range('SELECT * FROM {subscriptions_queue} WHERE uid = :uid AND last_sent + send_interval < :send_interval ORDER BY sqid', 0, 1, array(':uid' => $user->uid, ':send_interval' => time()));
        if (!($subs = $result->fetchAssoc())) {
          // No more ready queue items for this user, finish off this digest.
          $subs = $digest_data['send'];
          $subject_template = subscriptions_mail_template_load($mailmod, SUBSCRIPTIONS_DIGEST_MAILKEY, $lang_code, 'subject', 'DSUBJ');
          $body_template = subscriptions_mail_template_load($mailmod, SUBSCRIPTIONS_DIGEST_MAILKEY, $lang_code, 'body', 'DBODY');
          $separator = subscriptions_mail_template_load($mailmod, SUBSCRIPTIONS_DIGEST_MAILKEY . '+item', $lang_code, 'subject', 'SEP');
          $mailvars['!bodies'] = implode($separator, $digest_data['bodies']);
          $mailvars['!name'] = $subs['!name'];
          $mailvars['!manage_url'] = $subs['!manage_url'];
          $digest_data['mailvars'] = $mailvars;
          foreach (module_implements('subscriptions_digest_alter') as $module) {
            $function = $module . '_subscriptions_digest_alter';
            $function($digest_data);
            if (empty($digest_data)) {
              break;  // forget it
            }
          }
          if (!empty($digest_data)) {
            $mailvars = $digest_data['mailvars'];
            $subject = strtr($subject_template, $mailvars);
            $body = strtr($body_template, $mailvars);
            (_subscriptions_mail_send($mailmod, SUBSCRIPTIONS_DIGEST_MAILKEY, $subs['name'], $subs['mail'], $subject, $body, $subs['from'], $user->uid, $digest_data['send_intervals'], $data)
              ? ++$digest_count
              : ++$digest_failed);
          }
          $digest = FALSE;
        }
      }
    } while ( $digest );

    $user = $saved_user;
    $language = $saved_language;
    drupal_save_session(TRUE);
  }

  if ($single_count + $digest_count + $single_failed + $digest_failed > 0) {
    $current_seconds = timer_read('page')/1000;
    $variables = array(
      '!Module'            => 'Subscriptions',
      '!single_count'      => $single_count,
      '!digest_count'      => $digest_count,
      '!single_failed'      => $single_failed,
      '!digest_failed'      => $digest_failed,
      '!used_seconds'      => round($current_seconds - $lost_seconds),
      '!total_seconds'     => $total_seconds,
      '!remaining_items'   => db_query("SELECT COUNT(*) FROM {subscriptions_queue} WHERE last_sent + send_interval < :send_interval AND suspended = 0", array(':send_interval' => REQUEST_TIME))->fetchField(),
      '!suspended_items'   => db_query("SELECT COUNT(*) FROM {subscriptions_queue} WHERE last_sent + send_interval < :send_interval AND suspended <> 0", array(':send_interval' => REQUEST_TIME))->fetchField(),
      '!remaining_seconds' => round($total_seconds - $current_seconds),
      '%varname'           => 'subscriptions_mail_trash_silently',
      '!cron'              => 'cron',
    );
    if (variable_get('subscriptions_mail_trash_silently', 0)) {
      $message = t('!Module DISCARDED !single_count single and !digest_count digest notifications in !used_seconds of !total_seconds seconds, due to the %varname variable being set.', $variables);
    }
    elseif ($single_failed > 0 || $digest_failed > 0) {
      $message = t('!Module FAILED !single_failed single and !digest_failed digest notifications, sent !single_count single and !digest_count digest notifications in !used_seconds of !total_seconds seconds.', $variables);
    }
    else {
      $message = t('!Module sent !single_count single and !digest_count digest notifications in !used_seconds of !total_seconds seconds.', $variables);
    }
    $message .= ' ' . t('!remaining_items queue items remaining (plus !suspended_items suspended), !remaining_seconds seconds left for other @cron client modules.', $variables);
    $watchdog = 'watchdog'; // keep potx from translating 'cron'
    $watchdog('cron', $message);
  }
}

/**
 * Implements hook_mail().
 *
 * @param $key
 * @param $message
 * @param $params
 */
function subscriptions_mail_mail($key, &$message, $params) {
  global $base_url;

  $url = parse_url($base_url);
  $list_id = variable_get('site_name', '') . ' ' . t('Subscriptions') . ' <subscriptions.' . $url['host'] . '>';
  $message['headers']['List-Id'] = $list_id;
  //dpm($message, "subscriptions_mail_mail($key)");
}


/**
 * Sends the notification by mail.
 *
 * @param $module
 * @param $mailkey
 * @param $name
 * @param $to
 * @param $from
 * @param $uid
 * @param $send_intervals
 * @param $data
 *
 * @return bool|null
 */
function _subscriptions_mail_send($module, $mailkey, $name, $to, $from, $uid, $send_intervals, $data) {
  global $user;

  if (variable_get('subscriptions_mail_trash_silently', 0)) {
    // Block notification mail; useful for staging and development servers.
    return NULL;
  }

  $mail_success = drupal_mail($module, $mailkey, $to, user_preferred_language($user), array(
    'data' => $data,
    'account' => $user,
    'object' => NULL,
    'context' => array(
      'recipient' => $to,
    ),
  ), $from, TRUE);

  $watchdog_params = array('%name' => $name, '!address' => "<a href='mailto:$to'>$to</a>");
  if (!empty($mail_success['result'])) {
    if (variable_get('subscriptions_watchgood', 1)) {
      watchdog('subscriptions', 'Notification sent to...', $watchdog_params);
    }
    foreach ($send_intervals as $send_interval) {
      db_merge('subscriptions_last_sent')
        ->key(array(
          'uid'           => $uid,
          'send_interval' => $send_interval,
        ))
        ->fields(array(
          'last_sent'     => REQUEST_TIME,
        ))
        ->execute();
    }
    return TRUE;
  }
  watchdog('subscriptions', 'Error e-mailing notification to %name (!address).', $watchdog_params, WATCHDOG_ERROR, l('edit user', "user/$uid/edit"));
  return FALSE;
}

/**
 * Implements hook_mail_alter().
 *
 * @param $message
 */
function subscriptions_mail_mail_alter(&$message) {
  /* Comment this line for debugging...
  dpm($message, 'drupal_mail() is disabled in subscriptions_mail_mail_alter(); this would be sent');
  $message['to'] = '';
  /**/
}

