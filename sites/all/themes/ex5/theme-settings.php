<?php

// Provide field on the theme settings page for preview page url

function ex5_form_system_theme_settings_alter(&$form, $form_state) {
	
	$default = theme_get_setting('preview_url');
	
	$settting = ($default == ''?'http://www.demo.extension.org/preview/page/create/':$default);
	
 	$form['preview_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Preview url'),
		'#size' => 80,
		'#default_value' => $settting,
		'#description' => t('Preview url on the public site for content types that are published there.'),
	);
}

