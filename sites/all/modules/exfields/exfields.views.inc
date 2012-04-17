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
	
	
	
	$data['field_data_field_appraisal']['field_appraisal_appraisal_date'] = array(
      'group' => t('Date'),
      'title' => t('Appraisal date'),
      'help' => t('Filter content according to appraisal date'),
      'filter' => array(
        'handler' => 'exfields_handler_filter_datetime',
        'empty field name' => t('Undated'),
        'is date' => TRUE,
      ),
    );
	
	
	
	$data['field_data_field_appraisal']['field_appraisal_appraisal_overall'] = array(
      'group' => t('Appraisal'),
      'title' => t('Appraisal overal score'),
      'help' => t('Filter content according to appraisal overal score'),
      'filter' => array(
        'handler' => 'exfields_handler_filter_overall',
        'empty field name' => t('Undated'),
        'is date' => FALSE,
      ),
    );
	
	
	//Provide select control for group selection filter
	
	$data['og_membership']['group']['filter']['handler'] = 'og_handler_filter_group_audience_gid';
	
	// Provides appropriate controlls for authorship fields
	
	// contribution role
	$data['field_data_field_contributors']['field_contributors_contribution_role']['filter']['handler'] = 'roles_filter_authorship_fields';
	$data['field_data_field_contributors']['field_contributors_contribution_role']['field']['handler'] = 'exfields_roles_handler_field';
	$data['field_data_field_contributors']['field_contributors_contribution_role']['field']['click sortable'] = TRUE;
	// contribution date
	$data['field_data_field_contributors']['field_contributors_contribution_date']['filter']['handler'] = 'exfields_handler_filter_timestamp';
	$data['field_data_field_contributors']['field_contributors_contribution_date']['filter']['is date'] = TRUE;
	$data['field_data_field_contributors']['field_contributors_contribution_date']['field']['handler'] = 'views_handler_field_date';
	$data['field_data_field_contributors']['field_contributors_contribution_date']['field']['is date'] = TRUE;
	$data['field_data_field_contributors']['field_contributors_contribution_date']['field']['click sortable'] = TRUE;
	
  	return $data;
}


/**
* Filter handler for group audience:gid
*/
class og_handler_filter_group_audience_gid extends views_handler_filter_many_to_one {

 function get_value_options(){
    $groups = og_load_multiple(og_get_all_group());
    foreach($groups as $group) {
      if ($group->access()) {
        $this->value_options[$group->gid] = $group->label;
      }
    }
	asort($this->value_options);
  }
}


/**
* Filter handler for contribution roles
*/
class roles_filter_authorship_fields extends views_handler_filter_many_to_one {

 function get_value_options(){
	 $this->value_options = _exfields_roles_array();
	 unset($this->value_options['']);
  }
}

/**
* Fieod handler for displaying full name of authorship role
*/
class exfields_roles_handler_field extends views_handler_field{
	
	function render($values) {
		$roles_options = _exfields_roles_array();
    	$value = $values->{$this->field_alias};
     	return check_plain($roles_options[$value]);
   	}
}