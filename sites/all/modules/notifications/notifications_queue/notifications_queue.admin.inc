<?php
// $Id: notifications.admin.inc,v 1.5.2.6.2.19.2.12.2.15 2010/07/22 11:57:49 jareyero Exp $

/**
 * Send intervals administration
 */
function notifications_send_intervals_form($form, &$form_state) {
  // Collect information about digesting methods and print out some help
  // Collect information about building methods and print out some help
  /*
  $form['build'] = array(
    '#title' => t('Build formats'),
    '#type' => 'fieldset',
    '#description' => t('These are the building formats available for each interval.')
  );
  foreach (notifications_build_method() as $type => $method) {
    $build_methods[$type] = $method['name'];
    $rows[] = array($method['name'], $method['description']);    
  }
  $form['build']['info'] = array('#markup' => theme('table', array(), $rows));
  */
  // Build options. Need some complex formatting.
  $units = array(
    60 => t('Minutes'),
    60*60 => t('Hours'), 
    24*60*60 => t('Days'),
  );

  $intervals = _notifications_queue_send_intervals() + array('new1' => '', 'new2' => '');
  // We need to use different indexes (not time value) as negative ones won't work
  $index = 0;
  $index_name = $time_index = array();
  $header = array(
    'time' => t('Time'),
    'unit' => t('Unit'),
    'name' => t('Display name'),
    'default' => t('Default'),
  );
  $options = array();
  $form['intervals'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Send intervals'),
    '#header' => $header,
    '#theme' => 'notifications_admin_table_form',
    '#empty' => t('You need to enable some plug-ins to provide notifications events.'),
    '#description' => t('To delete an interval, set the time unit and the name empty. If the time is negative, notifications will never be sent.'),
  );
  foreach ($intervals as $time => $name) {
    // Store index => name mapping
    $index_options[$index] = '';
    $time_index[$time] = $index;
    // Calculate value and unit
    $current = $number = 0; 
    if (!is_numeric($time)) {
      $number = $unit = '';
    }
    elseif ($time >= 0) {
      foreach (array_reverse(array_keys($units)) as $unit) {
        if ($time % $unit == 0) {
          $current = $unit;
          $number = $time / $unit;
          break;
        }
      }
    }
    else {
      // May be -1 for 'Never'
      $number = $time;
      $unit = '';
    }
    $form['intervals']['time'][$index] = array(
      '#default_value' => $number,
      '#type' => 'textfield',
      '#size' => 2,
    );
    $form['intervals']['unit'][$index] = array(
      '#type' => 'select',
      '#options' => array('' => '') + $units,
      '#default_value' => $unit,
    );
    $form['intervals']['name'][$index] = array(
      '#type' => 'textfield',
      '#default_value' => $name,
      '#size' => 40,
    );
    /*
    $build = notifications_build_method($time);
    $form['intervals'][$index]['build'] = array(
      '#type' => 'select',
      '#disabled' => ($number < 0),
      '#default_value' => $build ? $build['type'] : 'simple',
      '#options' => $build_methods,
    );
    */
    // Store also index -> time mapping
    $form['intervals']['key'][$index] = array('#type' => 'value', '#value' => $time);
    $index++;
  }
  // Add field for default value
  $form['intervals']['default'] = array(
    '#type' => 'radios',
    '#options' => $index_options,
    '#default_value' => $time_index[variable_get('notifications_default_send_interval', 0)],
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  $form['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults'));

  return $form;
}

/**
 * Build a table with send intervals
 */
function theme_notifications_send_intervals_form($variables) {
  $element = $variables['element'];
  $output = ''; //drupal_render($element['digest']);
  $header = array(array('data' => t('Time'), 'colspan' => 2), t('Display name'), t('Format'), t('Default'));
  foreach (element_children($element['intervals']) as $key) {
    $rows[] = array(
      drupal_render($element['intervals'][$key]['time']),
      drupal_render($element['intervals'][$key]['unit']),
      drupal_render($element['intervals'][$key]['name']),
      drupal_render($element['intervals'][$key]['build']),
      drupal_render($element['default'][$key]),
    );  
  }
  $element['intervals']['#value'] = theme('table', array('header' => $header, 'rows' =>$rows));
  //$output .= theme('table', array('header' => $header, 'rows' =>$rows));
  $output .= drupal_render($element);
  return $output;
}

/**
 * Form validate for time intervals
 */
function notifications_send_intervals_form_validate($form, &$form_state) {
  // Check the default is a valid one, not one to be deleted
  $default = $form_state['values']['default'];
  if (empty($form_state['values']['intervals'][$default]['unit']) || empty($form_state['values']['intervals'][$default]['name'])) {
    form_set_error('default', t('Invalid default send interval.'));
  }
}

/**
 * Form submit for time intervals
 */
function notifications_send_intervals_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  if ($form_values['op'] == t('Reset to defaults')) {
    variable_del('notifications_send_intervals');
    variable_del('notifications_digest_methods');
    variable_del('notifications_default_send_interval');
  }
  else {
    $intervals = $digest = array();
    foreach ($form_values['intervals'] as $index => $values) {
      if (is_numeric($values['time']) && $values['name']) {
        $unit = $values['unit'] ? (int)$values['unit'] : 1;
        $time = (int)$values['time'] * $unit;
        $intervals[$time] = $values['name'];
        $build_methods[$time] = $values['build'];
        if ($index == $form_values['default']) {
          variable_set('notifications_default_send_interval', $time);
        }
      }
    }
    ksort($intervals);
    variable_set('notifications_send_intervals', $intervals);
    variable_set('notifications_digest_methods', $build_methods);
  }
  drupal_set_message(t('The time intervals for your subscriptions have been updated'));
  // Update orphaned notifications with invalid send interval
  $valid = array_keys($intervals);
  $params = array_merge(array(variable_get('notifications_default_send_interval', 0)), $valid);
  db_query('UPDATE {notifications_subscription} SET send_interval = %d WHERE send_interval NOT IN (' . db_placeholders($valid) . ')', $params);
  if ($updated = db_affected_rows()) {
    drupal_set_message(format_plural($updated, 'Updated a subscription with invalid interval.', 'Updated @count subscriptions with invalid intervals.'));
  }
  // Refresh strings after update if translation enabled
  if (module_exists('i18nstrings')) {
    notifications_locale_refresh();
  }
}
