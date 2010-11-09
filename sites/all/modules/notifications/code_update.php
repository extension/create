<?php
/**
 * This is old code to be updated and placed somewhere else
 */

/**
 * Reverse mapping from node title to nid
 * 
 * We also handle autocomplete values (title [nid:x]) and validate the form
 */
function notifications_node_title2nid($name, $field = NULL, $types = array()) {
  if (!empty($name)) {
    preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $name, $matches);
    if (!empty($matches)) {
      // Explicit [nid:n].
      list(, $title, $nid) = $matches;
      if (!empty($title) && ($node = node_load($nid)) && $title != $node->title) {
        if ($field) {
          form_set_error($field, t('Node title mismatch. Please check your selection.'));
        }
        $nid = NULL;
      }
    }
    else {
      // No explicit nid.
      $reference = _notifications_node_references($name, 'equals', $types, 1);
      if (!empty($reference)) {
        $nid = key($reference);
      }
      elseif ($field) {
        form_set_error($field, t('Found no valid post with that title: %title', array('%title' => $name)));
      }
    }
  }
  return !empty($nid) ? $nid : NULL;  
}

/**
 * Find node title matches.
 * 
 * Some code from CCK's nodereference.module
 */
function _notifications_node_references($string, $match = 'contains', $types = array(), $limit = 10) {
  $match_operators = array(
    'contains' => "LIKE '%%%s%%'",
    'equals' => "= '%s'",
    'starts_with' => "LIKE '%s%%'",
  );
  if ($types) {
    $where[] = 'n.type IN (' . db_placeholders($types, 'char') . ') ';
    $args = $types;
  }
  $where[] = 'n.title '. (isset($match_operators[$match]) ? $match_operators[$match] : $match_operators['contains']);
  $args[] = $string;
  $sql = db_rewrite_sql('SELECT n.nid, n.title, n.type FROM {node} n WHERE ' . implode(' AND ', $where) . ' ORDER BY n.title, n.type');
  $result = db_query_range($sql, $args, 0, $limit) ;
  $references = array();
  while ($node = db_fetch_object($result)) {
    $references[$node->nid] = array(
      'title' => $node->title,
      'rendered' => check_plain($node->title),
    );
  }
  return $references; 
}

/**
 * Get full info array for field type that has the property.
 * 
 * It will return the info array for the field it it has the property or the info for the object type
 * 
 * @param $type
 *   Field type
 * @param $property
 *   Property we are looking for
 *   
 * @return array()
 *   Info array from field or from object type
 */
function notifications_field_get_info($type, $property) {
  if ($info = notifications_field_type($type)) {
    if (isset($info[$property])) {
      return $info;
    }
    elseif (!empty($info['object_type']) && notifications_object_type($info['object_type'], $property)) {
      return notifications_object_type($info['object_type']);
    }
  }
}

/**
 * Format field type name
 */
function notifications_field_format_name($type) {
  $name = notifications_field_type($type, 'name');
  return $name ? $name : t('Unknown');
}

/**
 * Format field value
 * 
 * @param $type
 *   Field type
 * @param $value
 *   Field value
 * @param $html
 *   Whether to format the field as HTML (if FALSE will return plaintext format)
 * @param $subscription
 *   Subscription instance or template for which we want to format this field
 */
function notifications_field_format_value($type, $value, $html = TRUE, $subscription = NULL) {
  if ($format_info = notifications_field_get_info($type, 'format callback')) {
    $format_value = _notifications_info_callback($format_info, 'format callback', array($value, $html));
  }
  elseif ($options = notifications_field_subscription_options($type)) {
    // If not we try options callback, we can get the name from the array of options
    $format_value = isset($options[$value]) ? $options[$value] : t('Not available');
  }

  // If nothing got, we return the value
  if (!isset($format_value)) {
    $format_value = check_plain($value);
  }
  return $format_value;
}

/**
 * Collect submitted fields and parse new values
 */
function notifications_field_parse_submitted(&$form_state, $element_name = 'fields') {
  $fields = array();
  if (!empty($form_state['values'][$element_name]['type'])) {
    $field_values = &$form_state['values'][$element_name];
    foreach ($field_values['type'] as $key => $type) { 
      // If marked for deletion we just keep it there, don't return field
      if (empty($field_values['delete'][$key])) {
        // First collect all field values from the form   
        $field = array('type' => $type, 'value' => $field_values['value'][$key], 'edit' => $field_values['edit'][$key]);  
        // Complete field edit value, depending on field definition.
        if (empty($field_values['parsed'][$key])) {
          $value = notifications_field_real_value($type, $field['edit']);
          if (isset($value)) {
            $field['value'] = $value;
            $field_values['value'][$key] = $value;
            $field['parsed'] = TRUE;
            $field_values['parsed'][$key] = TRUE;
          }
          // Otherwise we let the field keep its value
        }
        // Add field to the list and mark as formatted so we can use this value for the form
        $fields[] = $field;
      }
    }
    
  }
  return $fields;
}

/**
 * Validate submitted field values and set the new ones as valid array of values
 */
function notifications_field_validate_submitted(&$form_state, $element_name = 'fields', $require_one = TRUE, $require_all = TRUE) {
  $checked_values = array();
  if ($field_values = notifications_field_parse_submitted($form_state, $element_name)) {
    foreach ($field_values as $key => $field) {
      $string_id = "$element_name][edit][$key";
      // We validate the field, type included
      if (notifications_field_valid_value($field['edit'])) {
        if (empty($field['parsed']) || !notifications_field_valid_value($field['value'], $field['type'])) {
          form_set_error($string_id, t('The value for this field is not valid.'));
          continue;
        }
      }
      elseif ($require_all) {
        form_set_error($string_id, t('You must set a value for this field.'));
        continue;
      }
      $checked_values[] = array('type' => $field['type'], 'value' => $field['value']);
    }
  }
  elseif ($require_one) {
    form_set_error(NULL, t('You must set at least one field for this subscription type.'));
  }
  return $checked_values;
}

/**
 * Convert field value from submission into its real value
 */
function notifications_field_real_value($type, $value) {  
  if (!notifications_field_valid_value($value)) {
    return NULL;
  }
  elseif ($info = notifications_field_get_info($type, 'value callback')) {
    // We have a value callback for field or object so use it
    return _notifications_info_callback($info, 'value callback', array($value));
  }
  else {
    // As we have nothing better, return the value itself
    return $value;
  }
}


/**
 * Get options for fields with options callback, may depend on subscription type
 * 
 * - First tries 'subscription_type options callback'
 * - If not found try generic 'options callback'
 */
function notifications_field_subscription_options($type, $subscription = NULL) {
  // First try specific options for this subscription type if any
  if ($subscription && ($info = notifications_field_get_info($type, "$subscription->type options callback"))) {
    return _notifications_info_callback($info, "$subscription->type options callback", array($subscription));
  }
  elseif ($info = notifications_field_get_info($type, 'options callback')) {
    return _notifications_info_callback($info, 'options callback', array($subscription));
  }
}

/**
 * Get field conditions for this specific object
 */
function notifications_object_condition_fields($type, $object) {
  if ($object = notifications_object_load($type, $object)) {
    // As this does an array_merge_recursive() we get grouped field => array(value1, value2..)
    $fields = module_invoke_all('notifications_object_' .$type, 'conditions', $object);
    // Now we just need to filter out duplicate values
    foreach ($fields as $key => $value) {
      if (is_array($value)) {
        $fields[$key] = array_unique($value);
      }
    }
    return $fields;
  }
}

/**
 * Form for object (node, user, term...) subscriptions
 *
 * @param $subscriptions
 *   Array of subscription options
 */
function notifications_object_options_form($form_state, $subscriptions) {
  $form['subscriptions'] =  notifications_object_options_fieldset($subscriptions, FALSE);  
  $form['submit'] = array('#type' => 'submit', '#value' => t('Update'));

  // If full form, redirect so the full page which may have subscription links is updated
  $form['#redirect'] = $_GET['q'];
  // Add always submit callback because the form may have a different name
  $form['#submit'][] = 'notifications_subscriptions_options_form_submit';
  return $form;
}

/**
 * Process submission
 */
function notifications_object_options_form_submit($form, $form_state) {
  $enabled = $disabled = 0;
  // We may have also send method and destination in this form, like on forms from anonymous users
  $send_method = isset($form_state['values']['send_method']) ? $form_state['values']['send_method'] : NULL;
  $destination = isset($form_state['values']['destination']) ? $form_state['values']['destination'] : NULL;
  foreach ($form_state['values']['subscriptions']['options'] as $index => $value) {
    $subscription = $form_state['values']['subscriptions']['params'][$index];   
    if ($value && !$subscription->is_instance()) {
      // We checked a disabled subscription
      if ($send_method) {
        $subscription->send_method = $send_method;
      }
      if ($destination) {
        $subscription->set_destination($destination);
      }
      notifications_save_subscription($subscription);
      $enabled++;
    }
    elseif (!$value && $subscription->is_instance()) {
      // we unchecked an enabled subscription
      Notifications_Subscription::delete_subscription($subscription->sid);
      $disabled++;
    }
  }

  if ($enabled) {
    drupal_set_message(format_plural($enabled, 'A subscription has been created', '@count subscriptions have been created'));
  }
  if ($disabled) {
    drupal_set_message(format_plural($disabled, 'A subscription has been deleted', '@count subscriptions have been deleted'));
  }  
}

/**
 * Get subscribe / unsubscribe page link for subscription
 * 
 * @param $op
 *   Operation: 'subscribe', 'unsubscribe'
 * @param $subscription
 *   Subscription object, may be instance or not
 * @param $options
 *   Link options
 */
function notifications_subscription_get_link($op, $subscription, $options = array()) {
  $options += array('destination' => $_GET['q']);
  if ($op == 'subscribe' && variable_get('notifications_option_subscribe_links', 0)) {
    $options += array('signed' => TRUE, 'confirm' => FALSE);
  }
  if ($op == 'unsubscribe' && variable_get('notifications_option_unsubscribe_links', 0)) {
    $options += array('signed' => TRUE, 'confirm' => FALSE);
  } 
  return notifications_build_link($op, $options, 'subscription', $subscription);
}


/**
 * Check subscription parameters
 */
function notifications_check_subscription($subscription) {
  if (!$subscription->check_account()) {
    $subscription->error_message = t('Invalid user account for this subscription.');
    return FALSE;
  }
  elseif (!$subscription->check_fields()) {
    $subscription->error_message = t('Invalid field values for this subscription.');
    return FALSE;
  }
  elseif (!$subscription->check_destination()) {
    $subscription->error_message = t('The destination method or address for the subscription is not valid.');
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Get an individual subscription.
 *
 * @param $subs
 *   Either a subscription object or a subscription id (sid).
 * @param $refresh
 *   Force cache refresh
 * @return
 *   Subscriptions object.
 */
function notifications_load_subscription($subs, $refresh = FALSE) {
  $subscriptions = &drupal_static(__FUNCTION__);
  if (is_object($subs)) {
    if(is_a($subs, 'Notifications_Subscription')) {
      $subscriptions[$subs->sid] = $subs;
    }
    else {
      $subscriptions[$subs->sid] = Notifications_Subscription::build($subs);
    }
    return $subscriptions[$subs->sid];
  }
  else {
    if ($refresh || !$subscriptions || !array_key_exists($subs, $subscriptions)) {
      $subscriptions[$subs] = Notifications_Subscription::load($subs);
    }
    return $subscriptions[$subs];
  }
}

/**
 * Get subscriptions that match a set of conditions.
 *
 * @param $main_conditions
 *   Conditions for the main notifications table
 * @param $field_conditions
 *   Optional array of condition fields. The elements may be
 *   - single field => value pairs 
 *   - or numeric key => array('type' => field, 'value' => value)
 * @param $exact_fields
 *   Whether to limit the result to subscriptions with exactly that condition fields. Otherwise we
 *   look for subscriptions that have and match that fields but may have more than that.
 * @param $key
 *   Optional key field to use as the array index. Will default to sid 
 *   For notifications with one field, it may be 'value' or 'intval'
 * @param $pager
 *   Whether to throw a pager query 
 * @return
 *   Array of subscriptions indexed by uid, module, field, value, author
 * 
 * @todo Check field types for building the query
 */
function notifications_get_subscriptions($main_conditions, $field_conditions = NULL, $exact_fields = TRUE, $key = 'sid', $pager = NULL) {
  notifications_include('query.inc');
  // Build query conditions using the query builder.  
  $query['select'][] = 's.*';
  // If we have the exact fields, make sure we match only the rows with this number of conditions
  if ($exact_fields && isset($field_conditions)) {
    $main_conditions += array('conditions' => count($field_conditions));
  }
  // Query for the notifications table
  $result = notifications_query_subscriptions($main_conditions, $field_conditions, $query);

  // Build list with results, we may need to index by a different field
  if ($key == 'sid') {
    $subscriptions = $result;
  }
  else {
    $subscriptions = array();
    foreach ($result as $subs) {
      if ($key == 'value' || $key == 'intval') {
        $field = array_shift($subs->get_fields());
        $subscriptions[$field->value] = $subs;
      }
      else {
        $subscriptions[$subs->$key] = $subs;
      }      
    }
  }

  return $subscriptions;
}

/**
 * Get active subscriptions for a given user to some object
 * 
 * This is a shorthand function to quickly check some types of subscriptions.
 * 
 * @param $account
 *   User id or user account object to check subscriptions for
 * @param $type
 *   Object type
 * @param $field
 *   Field type for the subscription. I.e. for a node it will be nid
 * @param $value
 *   Field value to check subscriptions to. I.e. $node
 * 
 * @return
 *   Array of subscriptions for this user and object indexed by sid 
 */
function notifications_user_get_subscriptions($account, $type, $object, $refresh = FALSE) {
  $subscriptions = &drupal_static(__FUNCTION__);
  $uid = messaging_user_uid($account);
  // No subscriptions for anonymous users
  if (!$uid) return array();
  
  // Check the object exists, is loaded and we've got a key field
  $object = notifications_object_load($type, $object);
  $key = notifications_object_type($type, 'key_field');
  if (!$object || !$key || empty($object->$key)) return array();
  
  // Now we've got the key fields for caching, try the cache or find subscriptions
  if ($refresh || !isset($subscriptions[$uid][$type][$object->$key])) {
    // Collect subscription types for this object and this account
    // Get allowed subscription options for this account to this object
    $user_subscriptions = array();
    $subscribe_options = notifications_object_subscribe_options($type, $object, messaging_user_object($account));
    foreach ($subscribe_options as $template) {
      $type_subscriptions = notifications_get_subscriptions(
        array('uid' => $uid, 'type' => $template['type'], 'status' => Notifications_Subscription::STATUS_ACTIVE), // Main conditions
        $template['fields'], // Field conditions
        TRUE // Match exactly these condition fields, no less, no more
      );
      if ($type_subscriptions) {
        $user_subscriptions += $type_subscriptions; //array_merge($user_subscriptions, $type_subscriptions);
      }
    }
    $subscriptions[$uid][$type][$object->$key] = $user_subscriptions;
  }
  return $subscriptions[$uid][$type][$object->$key];
}


/**
 * Return link array for subscriptions. OBSOLETED.
 * 
 * This one is kept just for backwards compatibility. Use notifications_build_link() instead.
 * 
 * @param $type
 *   Object type: 'subscribe', 'unsubscribe'
 * @param $params
 *   Aditional parameters for the subscription, may be
 *   - uid, the user for which the link is generated
 *   - confirm, whether to show confirmation page or not
 *   - destination, form destination or TRUE to use current path
 *   - signed, to produce a signed link that can be used by anonymous users (Example: unsubscribe link in emails)
 *   - type, object type
 *   - language, language object to get the url
 *   - Other subscription parameters: type, fields...
 * @param $format
 *   Whether to return a formatted link instead of a raw one (array)
 */
function notifications_get_link($op, $params, $format = FALSE) {
  // Add some default values
  $params += array('uid' => 0);
  $elements = array();

  switch ($op) {
    case 'subscribe':
      $type = 'subscription';
      break;
    case 'unsubscribe':     
      // The unsubscribe link can be for a single subscription or all subscriptions for a user
      if (!empty($params['sid'])) {
        $type = 'subscription';
        $params['oid'] = $params['sid'];
      }
      elseif ($params['uid']) {
        $type = 'user';
        $params['oid'] = $params['uid'];
      }
      break;
    case 'edit': // Edit subscription
      $type = 'subscription';
      $params['oid'] = $params['sid'];
      break;  
  }

  return notifications_build_link($op, $params, $type, NULL, $format ? 'link' : 'array');
}

/**
 * Return link array for notifications objects
 * 
 * @param $op
 *   Link operation: 'subscribe', 'unsubscribe', 'edit', 'manage'
 * @param $params
 *   Aditional parameters for the subscription, may be
 *   - uid, the user for which the link is generated
 *   - confirm, whether to show confirmation page or not
 *   - destination, form destination or TRUE to use current path
 *   - signed, to produce a signed link that can be used by anonymous users (Example: unsubscribe link in emails)
 *   - language, language object to user for generating the url
 *   - Other subscription parameters: type, fields...
 * @param $type
 *   Object type: 'subscription', 'destination'
 * @param $format
 *   Optional format string to return a formatted link/url instead or a raw one (array)
 *   - 'link' => Return a link
 *   - 'url' => Only the URL (no link)
 */
function notifications_build_link($op, $params, $type = 'subscription', $object = NULL, $format = 'array') {
  // Find the key fields and adjust parameters for each object type
  $key = array_search($type, array('sid' => 'subscription', 'mdid' => 'destination', 'uid' => 'user'));

  if ($object) {
    $params += array('oid' => $object->$key, 'uid' => $object->uid);
  }
  else {
    $params += array('uid' => 0);
  }
  // Build path elements that won't include the fixed prefix 'notifications'
  $elements = array();
  // Anonymous operations will be handled differently and the path will be notifications/anonymous
  if (!$params['uid']) {
    $params += array('signed' => TRUE);
  }
  $title = isset($params['title']) ? $params['title'] : NULL;
  // Build path elements depending on object type and operation
  switch ($op) {
    case 'subscribe':
      if ($object && !isset($params['type']) && !isset($params['fields'])) {
        $params += array('type' => $object->type, 'fields' => $object->get_conditions());
      }
      if ($params['uid']) {
        $elements = array('subscribe', $params['uid']);
      }
      else {
        $elements = array('anonymous', 'subscribe');
      }
      $elements[] = $params['type'];
      $elements[] = implode(',', array_keys($params['fields']));
      $elements[] = implode(',', $params['fields']);
      $title = $title ? $title : t('subscribe');
      break;
    case 'unsubscribe':
      // The unsubscribe link can be for a single subscription or all subscriptions for a user or a destination
      if (!$params['uid'] && $type == 'subscription') {
        // For anonymous: notifications/subscription/sid/unsubscribe
        $elements[] = 'anonymous';
        $elements[] = 'subscription';
        $elements[] = $params['oid'];
        $elements[] = 'unsubscribe';
      }
      else {
        $elements[] = 'unsubscribe';     
        $elements[] = $type;
        $elements[] = $params['oid'];
      }
      $title = $title ? $title : t('unsubscribe');
      break;
      
    case 'confirm':
      // For this and next cases we just set title if not set and fallback to default
      $title = $title ? $title : t('confirm');
    case 'edit':
       $title = $title ? $title : t('edit');
    case 'manage':
       $title = $title ? $title : t('manage');
    case 'delete':
       $title = $title ? $title : t('delete');
    default:
      if (!$params['uid']) {
        $elements[] = 'anonymous';
      }
      // This is a generic operation like 'subscription/sid/edit' or 'destination/mdid/manage'
      $elements[] = $type;
      $elements[] = $params['oid'];
      $elements[] = $op;
      break;
  }
  $params += array('title' => $title);
  
  $link =  _notifications_get_link($elements, $params);
  
  // Finally, format as requested or return full array
  switch ($format) {
    case 'link':
      return l($params['title'], $link['href'], $link['options']);
    case 'url':
      return url($link['href'], $link['options']);
    default:
      return $link;
  }
}

/**
 * Shorthand for building absolute signed URLs for messages
 * 
 * @see notifications_build_link()
 */
function notifications_build_url($op, $type = 'subscription', $object = NULL, $params = array()) {
  $params += array('absolute' => TRUE, 'signed' => TRUE);
  return notifications_build_link($op, $params, $type, $object, 'url');
}


/**
 * Get notifications link from elements and params
 * 
 * @param $elements
 *   Array of path elements, not including the 'notifications' prefix
 * @param $params
 *   Notifications link parameters as in notifications_build_link()
 * @param $options
 *   Aditional parameters for the url() function
 */
function _notifications_get_link($elements, $params = array()) {
  // Add some default values
  $params += array(
    'confirm' => TRUE,
    'signed' => FALSE,
    'destination' => FALSE,
    'query' => array(),
    'options' => array(),
  );
  if ($params['destination'] === TRUE) {
    $params['destination'] = $_GET['q'];
  }
  // Build query string using named parameters
  $query = $params['query'];
  if ($params['destination']) {
    $query['destination'] = $params['destination'];
  }
  // To skip the confirmation form, the link must be signed
  // Note tat the query string will be 'confirm=1' to skip confirmation form
  if (!$params['confirm']) {
    $query['confirm'] = 1;
    $params['signed'] = 1;
  }
  if ($params['signed']) {
    $query += array('timestamp' => time());
    $query['signature'] =  _notifications_signature($elements, $query);
  }
  // Build final link parameters
  $options = $params['options'];
  $options['query'] = $query;

  foreach (array('absolute', 'html', 'language') as $name) {
    if (isset($params[$name])) {
      $options[$name] = $params[$name];
    }
  }
  return array(
    'href' => 'notifications/'. implode('/', $elements),
    'options' => $options,
  );  
}

/**
 * Check access to objects
 * 
 * This will check permissions for subscriptions and events before subscribing
 * and before getting updates.
 * 
 * @param $type
 *   Type of object to check for access. Possible values:
 *   - 'event', will check access to event objects
 *   - 'subscription', will check access to subscribed objects
 */
function notifications_user_allowed($type, $account, $object = NULL) {
  notifications_include('object.inc');
  // First invoke the hook for 'event'  or 'subscription'. If we get any false return value, that's it
  $hook = 'notifications_' . $type;
  foreach (module_implements($hook) as $module) {   
    $permission = module_invoke($module, $hook, 'access', $object, $account);
    if (isset($permission) && ($permission === FALSE || is_array($permission) && in_array(FALSE, $permission, TRUE))) {
      return FALSE;
    }
  }
  if (is_object($object)) {
    // For events and subscriptions check first all objects are loaded
    if (!$object->load_objects()) {
      return FALSE;
    }
    // Now check all the loaded objects
    foreach ($object->get_objects() as $check_type => $values) {
      // Subscriptions can have several objects of one type
      $type_list = is_array($values) ? $values : array($values);
      foreach ($type_list as $check_object) {
        if (notifications_object_access($check_type, $check_object, $account) === FALSE) {
          return FALSE;
        }
      }
    }
  }
  // This means we've done all the checking and found nothing, so we allow access
  return TRUE;
}

/**
 * Check access to create/edit subscriptions
 */
function notifications_user_allowed_subscription($account, $subscription) {
  // For administrators, everything is allowed
  if (user_access('administer notifications', $account)) {
    return TRUE;
  }
  else {
    return notifications_user_allowed('subscription', $account, $subscription);
  }
}

/**
 * Implementation of hook_notifications()
 * 
 * Check access permissions to subscriptions and take care of some backwards compatibility
 */
function notifications_notifications($op) {
  switch ($op) {
    case 'build methods':
      // Return array of building engines
      $info['simple'] = array(
        'type' => 'simple',
        'name' => t('Simple'),
        'description' => t('Produces one message per event, without digesting.'),
         // This is a built in method so we don't have a 'build callback' for it
         //'build callback' => array('Notifications_Message', 'build_simple',
        'digest' => FALSE,
      );
      return $info;

    case 'object types':
      $types['node'] = array(
        'title' => t('Node'),
        'class' => 'Notifications_Node',
      );      
      $types['user'] = array(
        'title' => t('User'),
        'class' => 'Notifications_User',
      ); 
      return $types;

    case 'field types':
      $types['node'] = array(
        'title' => t('Node'),
        'class' => 'Notifications_Node_Field'
      );
      $types['user'] = array(
        'title' => t('User'),
        'class' => 'Notifications_User_Field'
      );
      return $types;
  }
}

/**
 * Implementation of hook notifications_subscription()
 */
function notifications_notifications_subscription($op, $subscription = NULL, $account = NULL) {
  switch ($op) {
    case 'access':
      // First we check valid subscription type
      $access = FALSE;
      if ($subscription->type && ($info = notifications_subscription_type($subscription->type))) {
        // To allow mixed subscription types to work we dont have a fixed field list
        // Then check specific access to this type. Each type must have a permission
        if (!empty($info['access callback'])) {
          $access = call_user_func($info['access callback'], $account, $subscription);
        }
        elseif (!empty($info['access'])) {
          $access = $info['access'] === TRUE || user_access($info['access'], $account);
        }
        // If allowed access so far, check field values
        if ($access) {
          $access = $subscription->check_fields();
        }
      }
      return $access;
      break;

    case 'page objects':
      if (arg(0) == 'user' && is_numeric(arg(1)) && ($account = menu_get_object('user'))) {
        return array('user' => $account);  
      }
      break;
  }  
}

/**
 * Implementation of hook notifications_event()
 * 
 * Invoke hook notifications with old parameters
 */
function notifications_notifications_event($op, $event, $account = NULL) {
  switch ($op) {

    case 'query':
      $query = array();
      // First get all the condition fields for the event object type
      // I.e. for a node event, we get all condition fields for the event's node
      if ($object = $event->get_object($event->type)) {
        if ($fields = notifications_object_condition_fields($event->type, $object)) {
          // We get a merged array with field => array(value1, value2...);
          $query[]['fields'] = $fields;
        }        
      }
      return $query;

    case 'load':
      // Invoke old hook for backwards compatibility
      return module_invoke_all('notifications', 'event load', $event);
  }
}

/**
 * Signature for url parameters
 * 
 * @param $params
 *   Ordered path elements
 * @param $query
 *   Query string elements
 */
function _notifications_signature($params, $query = array()) {
  $payload = implode('/', $params) . ':' . ($query ? notifications_array_serialize($query) : 'none');
  return md5('notifications' . drupal_get_private_key() .':' . $payload); 
}

/**
 * Default values for subscription
 */
function _notifications_subscription_defaults($account = NULL) {
  return array(
    'send_interval' => notifications_user_setting('send_interval', $account, 0),
    'send_method' => notifications_user_setting('send_method', $account, ''),
    'module' => 'notifications',
    'status' => Notifications_Subscription::STATUS_ACTIVE,
    'destination' => '',
    'cron' => 1,
  ); 
}

/**
 * Generic user page for a subscription type
 */
function notifications_user_subscription_list_page($type, $account) {
  module_load_include('pages.inc', 'notifications');
  module_load_include('manage.inc', 'notifications'); // Needed for bulk operations
  return drupal_get_form('notifications_subscription_list_form', $type, $account);  
}

/**
 * Generic subscriptions content form
 * 
 * Builds a form for a user to manage its own subscriptions with
 * some generic parameters
 * 
 * Currently it only manages simple condition fields
 * @param $account 
 *   User account
 * @param $type
 *   Subscription type
 * @param $subscriptions
 *   Current subscriptions of this type. If null they'll be loaded
 */
function notifications_user_form($form_state, $account, $type, $subscriptions) {
  module_load_include('pages.inc', 'notifications');
  module_load_include('manage.inc', 'notifications'); // Needed for bulk operations
  return notifications_subscription_list_form($form_state, $type, $account, $subscriptions);
}

/**
 * Process generic form submission
 */
function notifications_user_form_validate($form, &$form_state) {
  module_load_include('pages.inc', 'notifications');
  return notifications_subscription_list_form_validate($form, $form_state);
}

/**
 * Process generic form submission
 */
function notifications_user_form_submit($form, &$form_state) {
  module_load_include('pages.inc', 'notifications');
  return notifications_subscription_list_form_submit($form, $form_state);
}

/**
 * Short hand for info logs
 */
function notifications_log($message = NULL, $variables = NULL) {
  return messaging_log($message, $variables);
}

/**
 * Short hand for debug logs
 */
function notifications_debug($message = NULL, $variables = NULL) {
  return messaging_debug($message, $variables);
}

/**   
 * Wrapper function for 1i8nstrings() if i18nstrings enabled.   
 */   
function notifications_translate($name, $string, $langcode = NULL, $textgroup = 'notifications') {
  return function_exists('i18nstrings') ? i18nstrings($textgroup . ':' . $name, $string, $langcode) : $string;  
}

/**
 * Implementation of hook_locale().
 */
function notifications_locale($op = 'groups') {
  switch ($op) {
    case 'groups':
      return array('notifications' => t('Notifications'));
    case 'info':
      $info['notifications']['refresh callback'] = 'notifications_locale_refresh';
      $info['notifications']['format'] = FALSE; // Strings have no format
      return $info;
  }
}

/**
 * Refresh notifications strings
 */
function notifications_locale_refresh() {
  if ($intervals = variable_get('notifications_send_intervals', FALSE)) {
    foreach ($intervals as $key => $name) {
      i18nstrings_update("notifications:send_interval:$key:name", $name);
    }
  }
  return TRUE;
}

/**
 * Include module files as necessary.
 * 
 * The files must be in an 'includes' subfolder inside the module folder. 
 */
function notifications_include($file, $module = 'notifications') {
  return messaging_include($file, $module);
}


/**
 * Implementation of hook_token_values()
 * 
 * @ TODO: Work out event tokens
 */
function notifications_token_values($type, $object = NULL, $options = array()) {
  $language = isset($options['language']) ? $options['language'] : $GLOBALS['language'];
  switch ($type) {
    case 'subscription':
      $values = array();
      // Tokens only for registered users
      if ($subscription = messaging_check_object($object, 'Notifications_Subscription')) {
        $values['subscription-unsubscribe-url'] = notifications_build_url('unsubscribe', 'subscription', $subscription, array('language' => $language));
        $values['subscription-edit-url'] = notifications_build_url('edit', 'subscription', $subscription, array('language' => $language));
        $values['subscription-type'] = $subscription->get_type('name');
        $values['subscription-name'] = $subscription->get_name();
        $values['subscription-description-short'] = $subscription->format_short(FALSE);
        $values['subscription-description-long'] = $subscription->format_long(FALSE);
        // Old token, populate it for old templates
        $values['unsubscribe-url'] = $values['subscription-unsubscribe-url'];
      }
      return $values;
      
    case 'user':
      // Only for registered users.
      if (($account = $object) && !empty($account->uid)) {
        // We have a real user, so we produce full links
        $values['subscriptions-manage'] = url("user/$account->uid/notifications", array('absolute' => TRUE, 'language' => $language));
        $values['unsubscribe-url-global'] = notifications_build_url('unsubscribe', 'user', $account, array('language' => $language));
        return $values;
      }
      break;

    case 'destination':
      // These are just for registered users. For anonymous, notifications_anonymous will do it
      if (($destination = messaging_check_object($object, 'Messaging_Destination')) && !empty($destination->uid)) {
        $values['destination-unsubscribe-url'] = notifications_build_url('unsubscribe', 'destination', $destination, array('language' => $language));
        $values['destination-manage-url'] = notifications_build_url('manage', 'destination', $destination, array('language' => $language));
        $values['destination-edit-url'] = notifications_build_url('edit', 'destination', $destination, array('language' => $language));
        return $values;
      }
      break;

    case 'event':
      if ($event = messaging_check_object($object, 'Notifications_Event')) {
        $timezone = isset($options['timezone']) ? $options['timezone'] : variable_get('date_default_timezone', 0);
        $values['event-type-description'] = $event->get_type('description');
        $values['event-date-small'] = format_date($event->created, 'small', '', $timezone, $language->language);
        $values['event-date-medium'] = format_date($event->created, 'medium', '', $timezone, $language->language);
        $values['event-date-large'] = format_date($event->created, 'large', '', $timezone, $language->language);
        return $values;
      }
      break;
  }
}

/**
 * Implementation of hook_token_list(). Documents the individual
 * tokens handled by the module.
 */
function notifications_token_list($type = 'all') {
  $tokens = array();
  if ($type == 'user' || $type == 'all') {
    $tokens['user']['subscriptions-manage']    = t('The url for the current user to manage subscriptions.');
    $tokens['user']['unsubscribe-url-global'] = t('The url to allow a user to delete all their subscriptions.');
  }
  if ($type == 'subscription' || $type == 'all') {
    $tokens['subscription']['subscription-unsubscribe-url']  = t('The url for disabling a specific subscription.');
    $tokens['subscription']['subscription-edit-url']  = t('The url for editing a specific subscription.');
    $tokens['subscription']['subscription-type'] = t('The subscription type.');
    $tokens['subscription']['subscription-name'] = t('The subscription name.');
    $tokens['subscription']['subscription-description-short'] = t('The subscription short description.');
    $tokens['subscription']['subscription-description-long'] = t('The subscription long description.');
  }
  if ($type == 'event' || $type == 'all') {
    $tokens['event']['event-type-description'] = t('Description of event type.');
    $tokens['event']['event-date-small'] = t('Date of the event, short format.');
    $tokens['event']['event-date-medium'] = t('Date of the event, medium format.');
    $tokens['event']['event-date-large'] = t('Date of the event, large format.');
  }
  if ($type == 'destination' || $type == 'all') {
    // Nore destination tokens provided by messaging module
    $tokens['destination']['destination-unsubscribe-url'] = t('URL to unsubscribe to the destination.');
    $tokens['destination']['destination-manage-url'] = t('URL to manage all subscriptions for the destination.');
    $tokens['destination']['destination-edit-url'] = t('URL to edit the destination.');

  }
  return $tokens;
}

/**
 * Implementation of hook_form_alter()
 */
function notifications_form_alter(&$form, $form_state, $form_id) {
  switch ($form_id) {
    // Default send interval for user form
    case 'user_profile_form':
      if ($form['_category']['#value'] == 'account' && (user_access('maintain own subscriptions') || user_access('administer notifications'))) {
        $form['messaging']['#title'] = t('Messaging and Notifications settings');
        $send_intervals = notifications_send_intervals();
        $form['messaging']['notifications_send_interval'] = array(
          '#type' => 'select',
          '#title' => t('Default send interval'),
          '#options' => $send_intervals,
          '#default_value' => notifications_user_setting('send_interval', $form['_account']['#value']),
          '#disabled' => count($send_intervals) == 1,
          '#description' => t('Default send interval for subscriptions.'),
        );    
      }
  }
}
