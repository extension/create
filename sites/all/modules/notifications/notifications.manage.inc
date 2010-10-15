<?php
// $Id: notifications.manage.inc,v 1.1.2.3.2.7.2.12 2010/07/21 10:55:30 jareyero Exp $

/**
 * @file
 *   Common functions for bulk management of subscriptions
 */

/**
 * Check access for current user to manage multiple subscriptions
 * 
 * @param $sids
 *   Array of subscription ids
 */
function notifications_manage_subscriptions_access($sids) {
  if (user_access('administer notifications') || user_access('manage all subscriptions')) {
    return TRUE;
  }
  else {
    $count = db_query("SELECT COUNT(*) FROM {notifications_subscription} WHERE uid = :uid AND sid IN (:sid)" , array(
      ':uid' => $GLOBALS['user']->uid,
      ':sid' => $sids
    ))->fetchField();
    return $count == count($sids);
  }
} 

/**
 * Administer subscriptions. For user tabs and admin tabs
 * 
 * @param $conditions
 *   Filter conditions. I.e. for user administering its own subscriptions, we'll have an 'uid' condition
 */
function notifications_manage_subscriptions_form($conditions = array()) {
  $admin_access = user_access('administer notifications');
  // It is a user list if we have an uid parameter
  $account = !empty($conditions['uid']) ? user_load($conditions['uid']) : NULL; 
  $status_list = Notifications_Subscription::status_list();
  $type_list = notifications_subscription_type(NULL, 'title');
  $send_methods = messaging_method_info(NULL, 'name');
  $send_intervals = notifications_send_intervals();
  $drupal_destination = drupal_get_destination();
  // This is the full options subsform
  $form = notifications_manage_subscriptions_form_options($admin_access);
  // Depending on the context (user, admin) we'll have a different base path for the operation links
  $base_path = $account ? 'user/'. $account->uid .'/notifications/subscription' : 'notifications/subscription';

  // Build the sortable table header.
  $header = array(
    'sid' => array('data' => t('Id'), 'field' => 's.sid'),
    'type' => array('data' => t('Type'), 'field' => 's.type'),
    'status' => array('data' => t('Status'), 'field' => 's.status'),
    'name' => t('Description'),
    'created' => array('data' => t('Created'), 'field' => 's.created', 'sort' => 'desc')
  );
  if (!$account) {
    $header['uid'] = array('data' => t('User'), 'field' => 's.uid');
  }
  // If we have more than one send method or interval, add the columns
  if (count($send_methods) > 1) {
    $header['send_method'] = array('data' => t('Send method'), 'field' => 's.send_method');
  }  
  if (count($send_intervals) > 1) {
    $header['send_interval'] = array('data' => t('Send interval'), 'field' => 's.send_interval');
  }
  // Add operations at the end
  $header['operations'] = array('data' => t('Operations'));
  
  // Query data with $conditions and filters
  $query = db_select('notifications_subscription', 's')->extend('PagerDefault')->extend('TableSort');
  foreach ($conditions as $field => $value) {
    $query->condition('s.' . $field, $value);
  }
  notifications_manage_subscriptions_build_filter_query($query, $conditions);
  
  $sids = $query
    ->fields('s',array('sid'))
    ->limit(50)
    ->orderByHeader($header)
    ->execute()
    ->fetchCol();
  $subscriptions = entity_load('notifications_subscription', $sids);
  $options = array();
  foreach ($subscriptions as $subs) {
    $options[$subs->sid] = array(
      'sid' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $subs->sid,
          '#href' => $base_path . '/' . $subs->sid,
        ),
      ),
      'title' =>  $subs->get_title(),
      'name' => $subs->get_name(),
      'type' => $type_list[$subs->type],
      'status' => $status_list[$subs->status],
      'created' => format_date($subs->created, 'short'),
    );
      if (isset($header['uid'])) {
      // If not account, this is an admin form for multiple users, print full account and address
      if ($user = $subs->get_user()) {
        $username = theme('username', array('account' => $user));
      }
      else {
        // Anonymous subscription, print destination instead
        $dest = $sub->get_destination();
        $username = $dest->address_name() . ' ' . l($dest->format_address(FALSE), 'notifications/destination/' . $sub->mdid . '/manage') ;
      }
      $options[$subs->sid]['uid'] = $username;
    }
    if (!empty($header['send_method'])) {
      $options[$subs->sid]['send_method'] = !empty($send_methods[$subs->send_method]) ? $send_methods[$subs->send_method] : $subs->send_method;
    }
    if (!empty($header['send_interval'])) {
      $options[$subs->sid]['send_interval'] = !empty($send_intervals[$subs->send_interval]) ? $send_intervals[$subs->send_interval] : $subs->send_interval;
    }
    // Build a list of all the accessible operations for the current subscription.
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' =>  $base_path . '/' . $subs->sid . '/edit',
      'query' => $drupal_destination,
    );
  
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => $base_path . '/' . $subs->sid . '/delete',
      'query' => $drupal_destination,
    );

    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      $options[$subs->sid]['operations'] = array(
        'data' => array(
          '#theme' => 'links',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
    elseif (!empty($operations)) {
      // Render the first and only operation as a link.
      $link = reset($operations);
      $options[$subs->sid]['operations'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array('query' => $link['query']),
        ),
      );
    }   
  }

  // Only use a tableselect when the current user is able to perform any
  // operations.
  if ($admin_access) {
    $form['subscriptions'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No subscriptions available.'),
    );
  }
  // Otherwise, use a simple table.
  else {
    $form['subscriptions'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
      '#empty' => t('No subscriptions available.'),
    );
  }
  $form['pager'] = array('#markup' => theme('pager', array('tags' => NULL)));
  return $form;
}

/**
 * Form options
 */
function notifications_manage_subscriptions_form_options($admin_access = FALSE) {
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $options = array();
  foreach (notifications_manage_subscriptions_operations($admin_access) as $operation => $array) {
    if (!empty($array['parent'])) {
      $options[$array['parent']][$operation] = $array['label'];
    } else {  
      $options[$operation] = $array['label'];
    }
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('notifications_manage_subscriptions_form_validate'),
    '#submit' => array('notifications_manage_subscriptions_form_submit'),
  );
  return $form;
}

/**
 * Validate notifications_admin_subscriptions form submissions.
 * 
 * Check if any items have been selected to perform the chosen
 * 'Update option' on.
 */
function notifications_manage_subscriptions_form_validate($form, &$form_state) {
  $items = array_filter($form_state['values']['subscriptions']);
  if (count($items) == 0) {
    form_set_error('', t('No items selected.'));
  }
  else if (!notifications_manage_subscriptions_access(array_keys($items))) {
    form_set_error('', t('You don\'t have permissions to manage these subscriptions'));
  }
}

/**
 * Handle manage form submissions, run batch operations
 */
function notifications_manage_subscriptions_form_submit($form, &$form_state) {
  $operations = notifications_manage_subscriptions_operations();
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked subscriptions
  $items = array_filter($form_state['values']['subscriptions']);
 
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($items), $operation['callback arguments']);
    }
    else {
      $args = array($items);
    }
    call_user_func_array($function, $args);
  }
  else {
    // We need to rebuild the form to go to a second step.  For example, to
    // show the confirmation form for the deletion of subscriptions.
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Build query for node administration filters based on session.
 */
function notifications_manage_subscriptions_build_filter_query($query, $conditions) {
  $filters = notifications_manage_subscriptions_filters();
  // Build query
  if (!empty($_SESSION['subscriptions_overview_filter'])) {
    foreach ($_SESSION['subscriptions_overview_filter'] as $index => $filter) {
      list($key, $value) = $filter;
      switch ($key) {
        case 'type':
        case 'status':
        case 'send_interval':
        case 'send_method':
          $query->condition('s.' . $key, $value);
          break;
      }
    }
  }
}

/**
 * List node administration filters that can be applied.
 * 
 * @param $admin
 *   Whether this is for the site admin page, will display more options
 */
function notifications_manage_subscriptions_filters($admin = FALSE) {
  global $user;

  $filters['status'] = array(
    'title' => t('status'),
    'options' => Notifications_Subscription::status_list(),
  );
  $filters['type'] = array(
    'title' => t('type'),
    // If not admin page, check access to each type
    'options' => notifications_subscription_type(NULL, 'title'),
  );
  $filters['send_method'] = array(
    'title' => t('method'),
    // If not admin mode, filter for current user
    'options' => _notifications_send_methods($admin ? NULL : $user),
  );
  $filters['send_interval'] = array(
    'title' => t('interval'), 
    'options' => notifications_send_intervals(),
  );
  // Take out the filters when only one option
  foreach ($filters as $key => $data) {
    if (empty($data['options']) || count($data['options']) == 1) {
      unset($filters[$key]);
    }
  }
  return $filters;
}

/**
 * Subscription mass operations.
 * 
 * @param $account
 *   User account if we are administering subscriptions for this user
 */
function notifications_manage_subscriptions_operations($account = NULL) {
  $operations = array(
    'activate' => array(
      'label' => t('Activate'),
      'callback' => 'notifications_manage_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('status' => Notifications_Subscription::STATUS_ACTIVE)),
    ),
    'deactivate' => array(
      'label' => t('Deactivate'),
      'callback' => 'notifications_manage_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('status' => Notifications_Subscription::STATUS_INACTIVE)),
    ),
    'delete' => array(
      'label' => t('Delete'),
      'callback' => NULL,
    ),
  );
  // Block option only for administrators
  if (user_access('administer notifications') || user_access('manage all subscriptions')) {
    $operations['block'] = array(
      'label' => t('Block'),
      'callback' => 'notifications_manage_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('status' => Notifications_Subscription::STATUS_BLOCKED)),
    );
  }
  // Sending methods, not for destination
  if (!empty($account->mdid)) {
    $parent = t('Change send method to');
    foreach (_notifications_send_methods($account) as $key => $name) {
      $operations['send_method-' . $key] = array(
        'label' => $name,
        'parent' => $parent,
        'callback' => 'notifications_manage_subscriptions_mass_update',
        'callback arguments' => array('updates' => array('send_method' => $key)),
      );
    }
  }
  $parent = t('Change send interval to');
  foreach (notifications_send_intervals() as $key => $name) {
    $operations['send_interval-' . $key] = array(
      'label' => $name,
      'parent' => $parent,
      'callback' => 'notifications_manage_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('send_interval' => $key)),
    );
  }

  // Intervals
  return $operations;
}

/**
 * Make mass update of subscriptions, changing all nodes in the $nodes array
 * to update them with the field values in $updates.
 *
 * IMPORTANT NOTE: This function is intended to work when called
 * from a form submit handler. Calling it outside of the form submission
 * process may not work correctly.
 *
 * @param array $subscriptions
 *   Array of subscriptions nid to update.
 * @param array $updates
 *   Array of key/value pairs with node field names and the
 *   value to update that field to.
 */
function notifications_manage_subscriptions_mass_update($subscriptions, $updates) {
  foreach ($subscriptions as $id) {
    _notifications_subscriptions_mass_update_helper($id, $updates);
  }
  drupal_set_message(t('The update has been performed.'));
}

function _notifications_manage_subscriptions_mass_update_helper($sid, $updates) {
  $subs = notifications_load_subscription($sid);
  foreach ($updates as $name => $value) {
    $subs->$name = $value;
  }
  notifications_save_subscription($subs);
  return $subs;  
}

/**
 * Form for multiple delete. When account passed check that all subscriptions belong to the user account
 */
function notifications_manage_multiple_delete_confirm(&$form_state, $items, $destination = NULL) {
  $destination = $destination ? $destination : $_GET['q'];
  if (notifications_manage_subscriptions_access(array_keys($items))) {
    $form['items'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
    // array_filter returns only elements with TRUE values
    foreach ($items as $id => $value) {
      // Load the subscription to display a friendly name
      $subscription = notifications_load_subscription($id);
      $form['items'][$id] = array(
        '#type' => 'hidden',
        '#value' => $id,
        '#prefix' => '<li>',
        '#suffix' => $subscription->format_long()."</li>\n",
      );
    }    
    $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
    $form['#submit'][] = 'notifications_multiple_delete_confirm_submit';
    $form['#validate'][] = 'notifications_multiple_delete_confirm_validate';
    $form['#redirect'] = $destination;
    return confirm_form($form,
                      t('Are you sure you want to delete these items?'),
                      $destination, t('This action cannot be undone.'),
                      t('Delete all'), t('Cancel'));
  }
  else {
    drupal_set_message(t('Validation error. You don\'t have permission to delete some of these subscriptions'), 'error');
    drupal_access_denied();
  }
}

/**
 * Validate permissions to delete all the subscriptions
 */
function notifications_manage_multiple_delete_confirm_validate($form, &$form_state) {
  if (!notifications_manage_subscriptions_access(array_keys($form_state['values']['items']))) {
    form_set_error('', t('You don\'t have permission to manage all these subscriptions'));
  }
}

/**
 * Submit multiple delete from
 */
function notifications_manage_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    Notifications_Subscription::delete_subscription(array_keys($form_state['values']['items']));
    drupal_set_message(t('The subscriptions have been deleted.'));
  }
  return;
}

/**
 * Return form for node administration filters.
 * 
 * @param $admin
 *   Whether this is for the site admin page, will display more options
 */
function notifications_manage_subscriptions_filter_form($admin = FALSE) {
  $session = &$_SESSION['subscriptions_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = notifications_manage_subscriptions_filters($admin);

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'node_filters', // We reuse this theme from node admin pages
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    $value = $filters[$type]['options'][$value];
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
    if (in_array($type, array('type', 'language'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }
  
  
  
  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }
  
  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );  
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * Process result from node administration filter form.
 */
function notifications_manage_subscriptions_filter_form_submit($form, &$form_state) {
  $filters = notifications_subscriptions_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      if (isset($form_state['values']['filter'])) {
        $filter = $form_state['values']['filter'];
        // Flatten the options array to accommodate hierarchical/nested options.
        $flat_options = form_options_flatten($filters[$filter]['options']);
        if (isset($flat_options[$form_state['values'][$filter]])) {
          $_SESSION['subscriptions_overview_filter'][] = array($filter, $form_state['values'][$filter]);
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['subscriptions_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['subscriptions_overview_filter'] = array();
      break;
  }
}

/**
 * Theme subscriptions management form
 *
 * @ingroup themeable
 */
function theme_notifications_manage_subscriptions($variables) {
  $form = $variables['form'];
  // If there are rows in this form, then $form['title'] contains a list of
  // the title form elements.
  $has_posts = isset($form['type']) && is_array($form['type']);
  $select_header = $has_posts ? theme('table_select_header_cell') : '';
  $header = array($select_header);
  $header[] = t('Type');
  if (!empty($form['description'])) {
    $header[] = t('Description');
  }
  if (!empty($form['username'])) {
    $header[] = t('User');
  }
  if (!empty($form['send_method'])) {
    $header[] = t('Send method');
  }
  if (!empty($form['send_interval'])) {
    $header[] = t('Send interval');
  }
  $header[] = t('Status');
  $header[] = t('Operations');
  $output = '';

  $output .= drupal_render($form['options']);
  if ($has_posts) {
    foreach (element_children($form['type']) as $key) {
      $row = array();
      $row[] = drupal_render($form['subscriptions'][$key]);
      $row[] = drupal_render($form['type'][$key]);
      if (isset($form['description'])) {
        $row[] = drupal_render($form['description'][$key]);
      }
      if (isset($form['username'])) {
        $row[] = drupal_render($form['username'][$key]);
      }
      if (isset($form['send_method'])) {
        $row[] = drupal_render($form['send_method'][$key]);
      }
      if (isset($form['send_interval'])) {
        $row[] = drupal_render($form['send_interval'][$key]);
      }   
      $row[] = drupal_render($form['status'][$key]);
      $row[] = drupal_render($form['operations'][$key]);
      $rows[] = $row;
    }

  }
  else {
    $rows[] = array(array('data' => t('No subscriptions available.'), 'colspan' => '6'));
  }

  $output .= theme('table', array('header' => $header, 'rows' =>$rows));
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

/**
 * Theme node administration filter form.
 *
 * @ingroup themeable
 */
function theme_notifications_manage_subscriptions_filter_form($variables) {
  $form = $variables['form'];
  $output = '';
  $output .= '<div id="notifications-admin-filter">';
  $output .= drupal_render($form['filters']);
  $output .= '</div>';
  $output .= drupal_render($form);
  return $output;
}
