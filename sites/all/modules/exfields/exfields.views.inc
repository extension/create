<?php
/**
 * Implementation of hook_views_data
 */
function exfields_views_data_alter(&$data) {
	
	

    // Telling views that varchar field is actually a date.
    $data['field_data_field_appraisal']['field_appraisal_improvements_date'] = array(
      'group' => t('Date'),
      'title' => t('Improvements were completed'),
      'help' => t('Filter content according to appraisal improvement date'),
      'filter' => array(
        'handler' => 'exfields_handler_filter_datetime',
        'empty field name' => t('Undated'),
        'is date' => TRUE,
      ),
    );
	
	
  return $data;
}