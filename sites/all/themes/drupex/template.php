<?php

//preprocess node and add variables for the time node is updated and full name of the user made the last revision

function drupex_preprocess_node(&$variables) {
	$user_id = $variables['revision_uid'];
	$user_fields = user_load($user_id);
	
	$variables['firstname'] = (isset($user_fields->field_first_name['und']['0']['value'])?$user_fields->field_first_name['und']['0']['value']:'');
	$variables['lastname'] = (isset($user_fields->field_last_name['und']['0']['value'])?$user_fields->field_last_name['und']['0']['value']:'');
	if($variables['firstname'] == ''){
		$variables['formatted_user'] = l($user_fields->name, 'user/'.$user_fields->uid);
	}else{
		$variables['formatted_user'] = l($variables['firstname'].' '.$variables['lastname'], 'user/'.$user_fields->uid);
	}
  	$variables['updated_formatted_date'] = date("F j, Y", $variables['revision_timestamp']);
	$variables['updated'] = 'Last updated: '.$variables['updated_formatted_date'].' by '.$variables['formatted_user'];
	
}