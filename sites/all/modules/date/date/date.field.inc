<?php
// $Id: date.field.inc,v 1.10 2010/11/01 10:48:58 karens Exp $
/**
 * @file
 * Field hooks to implement a date field.
 */

/**
 * Implements hook_field_formatter_info().
 */
function date_field_formatter_info() {
  $formatters = array(
    'date_default' => array(
      'label' => t('Default'),
      'field types' => array('date', 'datestamp', 'datetime'),
      'settings' => array(
        'format_type' => 'long',
        'show_repeat_rule' => 'show',
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'fromto' => 'both',
      ),
    ),
    'format_interval' => array(
      'label' => t('As Time Ago'),
      'field types' => array('date', 'datestamp', 'datetime'),
      'settings' => array(
        'interval' => 2,
      ),
    ),
  );

  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function date_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $formatter = $display['type'];
  module_load_include('inc', 'date', 'date_admin');
  switch ($formatter) {
    case 'format_interval':
      return date_interval_formatter_settings_form($field, $instance, $view_mode, $form, $form_state);
    default:
      return date_default_formatter_settings_form($field, $instance, $view_mode, $form, $form_state);
  }
}
 
/**
 * Implements hook_field_formatter_settings_summary().
 */
function date_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $formatter = $display['type'];
  module_load_include('inc', 'date', 'date_admin');
  switch ($formatter) {
    case 'format_interval':
      return date_interval_formatter_settings_summary($field, $instance, $view_mode);
    default:
      return date_default_formatter_settings_summary($field, $instance, $view_mode);
  }
}

/**
 * Implements hook_field_formatter_view().
 *
 * Useful values:
 *
 *   $entity->date_id
 *     If set, this will show only an individual date on a field with
 *     multiple dates. The value should be a string that contains
 *     the following values, separated with colons:
 *     - module name of the module adding the item
 *     - node nid
 *     - field name
 *     - delta value of the field to be displayed
 *     - other information the module's custom theme might need
 *
 *     Used by the calendar module and available for other uses.
 *     example: 'date:217:field_date:3:test'
 *
 *   $entity->date_repeat_show
 *     If true, tells the theme to show all the computed values
 *     of a repeating date. If not true or not set, only the
 *     start date and the repeat rule will be displayed.
 *
 */
function date_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];
  $formatter = $display['type'];
  $vars = array(
    'entity' => $entity,
    'entity_type' => $entity_type,
    'field' => $field,
    'instance' => $instance,
    'langcode' => $langcode,
    'items' => $items,
    'display' => $display,
    'dates' => array(),
  );
  
  // See if we are only supposed to display a selected
  // item from multiple value date fields.
  $selected_deltas = array();
  if (!empty($entity->date_id)) {
    foreach ((array) $entity->date_id as $key => $id) {
      list($module, $nid, $field_name, $selected_delta, $other) = explode(':', $id);
      if ($field_name == $field['field_name']) {
        $selected_deltas[] = $selected_delta;
      }
    }
  }
  
  switch ($display['type']) {
    case 'format_interval':
      foreach ($items as $delta => $item) {
        if (!empty($entity->date_id) && !in_array($delta, $selected_deltas)) {
          continue;
        }
        else {
          $vars['delta'] = $delta;
          $vars['item'] = $item;
          $vars['dates'] = date_formatter_process($formatter, $entity_type, $entity, $field, $instance, $langcode, $item, $display);
          $element[$delta] = array('#markup' => theme('date_display_interval', $vars));
        }
      }
      break;
    default:
      foreach ($items as $delta => $item) {
        if (!empty($entity->date_id) && !in_array($delta, $selected_deltas)) {
          continue;
        }
        else {
          $vars['delta'] = $delta;
          $vars['item'] = $item;
          $vars['dates'] = date_formatter_process($formatter, $entity_type, $entity, $field, $instance, $langcode, $item, $display);
          $element[$delta] = array('#markup' => theme('date_display_combination', $vars));
        }
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_is_empty().
 */
function date_field_is_empty($item, $field) {
  // Sometimes a $item is a date object.
  // Coming from repeating dates. Why?? 
  if (!is_array($item)) {
    return FALSE;
  }
  if (empty($item['value'])) {
    return TRUE;
  }
  elseif ($field['settings']['todate'] == 'required' && empty($item['value2'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_field_info().
 */
function date_field_info() {
  $settings = array(
    'settings' => array(
      'todate' => '',
      'repeat' => 0,
      'granularity' => drupal_map_assoc(array('year', 'month', 'day', 'hour', 'minute')),
      'tz_handling' => 'site',
      'timezone_db' => 'UTC',
    ),
    'instance_settings' => array(
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'blank',
      'default_value_code2' => '',
      'default_format' => 'medium',
    ),
    // Integrate with the Entity Metadata module.
    'property_type' => 'date',
    'property_callbacks' => array('date_entity_metadata_property_info_alter'),
  );
  return array(
    'date' => array(
      'label' => 'Date',
      'description' => t('Store a date in the database as an ISO date, recommended for historical or partial dates.'),
      'default_widget' => 'date_select',
      'default_formatter' => 'date_default',
      ) + $settings,
    'datestamp' => array(
      'label' => 'Datestamp',
      'description' => t('Store a date in the database as a timestamp, deprecated format to suppport legacy data.'),
      'default_widget' => 'date_select',
      'default_formatter' => 'date_default',
      ) + $settings,
    'datetime' => array(
      'label' => 'Datetime',
      'description' => t('Store a date in the database as a datetime field, recommended for complete dates and times that may need timezone conversion.'),
      'default_widget' => 'date_select',
      'default_formatter' => 'date_default',
      ) + $settings,
  );
}

/**
 * Implements hook_field_widget_info().
 */
function date_field_widget_info() {
  $settings = array(
    'settings' => array(
      'input_format' => date_default_format('date_select'),
      'input_format_custom' => '',
      'increment' => 1,
      'text_parts' => array(),
      'year_range' => '-3:+3',
      'label_position' => 'above',
      'repeat_collapsed' => 0,
    ),
    'behaviors' => array(
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'default value' => FIELD_BEHAVIOR_CUSTOM,
    ),
  );
  
  // Repeating dates have custom handling
  // for multiple values.
  $repeat_settings = $settings;
  $repeat_settings['behaviors']['multiple values'] = FIELD_BEHAVIOR_CUSTOM;

  $info = array(
    'date_select' => array(
      'label' =>  t('Select List'),
      'field types' => array('date', 'datestamp', 'datetime'),
    ) + $settings,
    'date_select_repeat' => array(
      'label' =>  t('Select List with Repeat options'),
      'field types' => array('date', 'datestamp', 'datetime'),
    ) + $repeat_settings,
    'date_text' => array(
      'label' =>  t('Text Field with custom input format'),
      'field types' => array('date', 'datestamp', 'datetime'),
     ) + $settings,
    'date_text_repeat' => array(
      'label' =>  t('Text Field with Repeat options'),
      'field types' => array('date', 'datestamp', 'datetime'),
    ) + $repeat_settings,
  );
  if (module_exists('date_popup')) {
    $info['date_popup'] = array(
      'label' =>  t('Text Field with Date Pop-up calendar'),
      'field types' => array('date', 'datestamp', 'datetime'),
    ) + $settings;
    $info['date_popup_repeat'] = array(
      'label' =>  t('Text Field with Date Pop-up and Repeat options'),
      'field types' => array('date', 'datestamp', 'datetime'),
    ) + $repeat_settings;
  }
  if (!module_exists('date_repeat')) {
    unset($info['date_select_repeat']);
    unset($info['date_text_repeat']);
    if (isset($info['date_popup_repeat'])) {
      unset($info['date_popup_repeat']);
    }
  }
  return $info;
}

/**
 * Implements hook_field_load().
 */
function date_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      // If the file does not exist, mark the entire item as empty.
      $timezone = isset($item['timezone']) ? $item['timezone'] : '';
      if (is_array($item)) {
        $items[$id][$delta]['timezone'] = date_get_timezone($field['settings']['tz_handling'], $timezone);
        $items[$id][$delta]['timezone_db'] = date_get_timezone_db($field['settings']['tz_handling']);
        $items[$id][$delta]['date_type'] = $field['type'];
      }
    }
  }
}

/**
 * Implements hook_field_validate().
 */
function date_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  $field_name = $field['field_name'];
  $flexible = 0;
  
  // Don't try to validate if there were any errors before this point
  // since the element won't have been munged back into a date.
  if (!form_get_errors()) {
    foreach ($items as $delta => $item) {
      if (is_array($item)) {
        $process = date_process_values($field, $instance);
        $date1 = new DateObject($item['value'], $item['timezone'], date_type_format($field['type']));
        if (empty($item['value2']) && $item['value2'] !== 0) {
          $date2 = clone($date1);
        }
        else {
          $date2 = new DateObject($item['value2'], $item['timezone'], date_type_format($field['type']));
        }
        $valid1 = $date1->valid($field['settings']['granularity'], $flexible);
        $valid2 = $date2->valid($field['settings']['granularity'], $flexible);
        
        foreach ($process as $processed) {
          if ($processed == 'value' && $field['settings']['todate'] && !$valid1 && $valid2) {
            $errors[$field['field_name']][$langcode][$delta][] = array(
              'error' => 'value',
              'message' => t("A 'From date' date is required for field %field #%delta.", array('%delta' => $field['cardinality'] ? intval($delta + 1) : '', '%field' => t($instance['label']))),
            );
          }
          if ($processed == 'value2' && $field['settings']['todate'] == 'required' && ($field['required'] && $valid1 && !$valid2)) {
            $errors[$field['field_name']][$langcode][$delta][] = array(
              'error' => 'value2',
              'message' => t("A 'To date' is required for field %field #%delta.", array('%delta' => $field['cardinality'] ? intval($delta + 1) : '', '%field' => t($instance['label']))),
            );
          }            
        }       
      }
    }
  }

}

/**
 * Implements hook_field_presave().
 */
function date_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  date_field_update($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_insert().
 */
function date_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  date_field_update($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_update().
 */
function date_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  $field_name = $field['field_name'];

  if (empty($items)) {
    return;
  }
  // Add some information needed to interpret token values.
  $values = $items;
  foreach ($values as $delta => $item) {
    $timezone = isset($item['timezone']) ? $item['timezone'] : '';
    if (is_array($item)) {
      $items[$delta]['timezone'] = date_get_timezone($field['settings']['tz_handling'], $timezone);
      $items[$delta]['timezone_db'] = date_get_timezone_db($field['settings']['tz_handling']);
      $items[$delta]['date_type'] = $field['type'];
    }  
  }  
  $entity->$field['field_name'][$langcode] = $items;

  foreach ($values as $delta => $item) {
    if (is_array($item)) {
      // Special case for ISO dates which may have been given artificial values for
      // some date parts to make them into valid dates.
      // @TODO Decide how to replace the date_limit_value() function.
      if (!empty($item['value']) && $field['type'] == DATE_ISO) {
        //$items[$delta]['value'] = date_limit_value($items[$delta]['value'], date_granularity($field), $field['type']);
        if ($field['settings']['todate']) {
          //$items[$delta]['value2'] = date_limit_value($items[$delta]['value2'], date_granularity($field), $field['type']);
        }
      }  
    }
  }
  $entity->$field['field_name'][$langcode] = $items;
}

/**
 * Wrapper functions for date administration, included only when
 * processing field settings.
 */
function date_field_instance_settings_form($field, $instance) {
  module_load_include('inc', 'date', 'date_admin');
  return _date_field_instance_settings_form($field, $instance);
}

function date_field_widget_settings_form($field, $instance) {
  module_load_include('inc', 'date', 'date_admin');
  return _date_field_widget_settings_form($field, $instance);
}

function date_field_settings_form($field, $instance, $has_data) {
  module_load_include('inc', 'date', 'date_admin');
  return _date_field_settings_form($field, $instance, $has_data);
}
