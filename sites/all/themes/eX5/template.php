<?php

function ex5_breadcrumb($variables){
	$breadcrumb = $variables['breadcrumb'];

	if (!empty($breadcrumb)) {
	// Provide a navigational heading to give context for breadcrumb links to
	// screen-reader users. Make the heading invisible with .element-invisible.
		$output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
		$breadcrumb = array_reverse($breadcrumb);
		$title = drupal_get_title();
		$output .= '<div class="breadcrumb">'. $title. ' &laquo; '. implode(' &laquo; ', $breadcrumb) . '</div>';
		return $output;
  	}
}


function ex5_preprocess_node(&$variables) {
	//
	
	$user_id = $variables['revision_uid'];
	
	$user_fields = user_load($user_id);
	//dprint_r($user_fields);
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

function ex5_menu_local_tasks(&$variables) {
  $output = '';
	$variables['primary'][count($variables['primary'])-1]['#link']['localized_options']['attributes']['class'][] = 'last';
	//dprint_r($variables);
  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}
