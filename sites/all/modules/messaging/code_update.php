<?php
/**
 * This is old code to be updated and placed somewhere else
 */

/**
 * Implementation of hook_locale().
 */
function messaging_locale($op = 'groups') {
  switch ($op) {
    case 'groups':
      return array('messaging' => t('Messaging'));
    case 'info':
      $info['messaging']['refresh callback'] = 'messaging_locale_refresh';
      $info['messaging']['format'] = TRUE; // Strings may have format
      return $info;
  }
}

/**
 * Refresh strings for translation system
 */
function messaging_locale_refresh() {
  foreach (messaging_method_info() as $name => $info) {
    i18nstrings_update("messaging:method:$name:name", $info['name']);
  }
  return TRUE;
}

/**
 * Implementation of hook_token_list(). Documents the individual
 * tokens handled by the module.
 */
function messaging_token_list($type = 'all') {
  $tokens = array();
  if ($type == 'message' || $type == 'all') {
    $tokens['message']['message-subject']    = t('The message subject.');
    $tokens['message']['message-body']    = t('The message body.');
    $tokens['message']['message-author-name'] = t('The message\'s author name.');
    $tokens['message']['message-method'] = t('The message\'s method name.');
    $tokens['message']['message-date'] = t('The message\'s sending date.');
  }
  if ($type == 'destination' || $type == 'all') {
    $tokens['destination']['destination-address'] = t('Destination address.');
    $tokens['destination']['destination-type'] = t('Destination address type.');
  }
  return $tokens;
}

/**
 * Implementation of hook_token_values()
 */
function messaging_token_values($type, $object = NULL, $options = array()) {
  $language = isset($options['language']) ? $options['language'] : $GLOBALS['language'];
  switch ($type) {
    case 'message':
      if ($message = messaging_check_object($object, 'Messaging_Message')) {
        $values['message-subject'] = check_plain($message->get_subject());
        $values['message-body'] = filter_xss($message->get_body());
        $values['message-author-name'] = check_plain($message->get_sender_name());
        $values['message-method'] = messaging_method_info($message->method, 'name');
        $timezone = isset($options['timezone']) ? $options['timezone'] : variable_get('date_default_timezone', 0);
        $values['message-date'] = format_date($message->sent, 'medium', '', $timezone, $language->language);
        return $values;
      }
      break;
    case 'destination':
      // Messaging destinations
      if ($destination = messaging_check_object($object, 'Messaging_Destination')) {
        $values['destination-address'] = $destination->format_address(FALSE);
        $values['destination-type'] = $destination->address_name();
        return $values;
      }
      break;
  }
}

/**
 * Implementation of hook_user().
 *
 * Adds fieldset and default sending method setting.
 */
function messaging_user($type, $edit, &$user, $category = NULL) {
  switch ($type) {
    case 'form':
      if ($category == 'account' && ($list = messaging_method_list($user))) {
        $form['messaging'] = array(
          '#type'        => 'fieldset',
          '#title'       => t('Messaging settings'),
          '#weight'      => 5,
          '#collapsible' => TRUE,
        );
        $form['messaging']['messaging_default'] = array(
          '#type'          => 'select',
          '#title'         => t('Default send method'),
          '#default_value' => messaging_method_default($user),
          '#options' => $list,
          '#description'   => t('Default sending method for getting messages from this system.'),
          '#disabled' => count($list) == 1,
        );
        return $form;
      }
      break;
  }
}