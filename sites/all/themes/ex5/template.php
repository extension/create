<?php

// Add javascript needed for the right column accordion
drupal_add_library('system', 'ui.accordion');
drupal_add_js('jQuery(document).ready(function(){jQuery(".region-sidebar-second").accordion({ autoHeight: false});});', 'inline');

//Reverse order of the links in the breadcrumb and change the bullet between them
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

//Adding some variables for the comment template
function ex5_preprocess_comment(&$variables) {
	$user_id = $variables['elements']['#comment']->uid;
	$user_fields = user_load($user_id);
	$variables['firstname'] = (
								isset($user_fields->field_first_name['und']['0']['value'])
								?		
									$user_fields->field_first_name['und']['0']['value']
								:
									'');
	$variables['lastname'] = (isset($user_fields->field_last_name['und']['0']['value'])?$user_fields->field_last_name['und']['0']['value']:'');
	if($variables['firstname'] == ''){
		$variables['formatted_user'] = l($user_fields->name, 'user/'.$user_fields->uid);
	}else{
		$variables['formatted_user'] = l($variables['firstname'].' '.$variables['lastname'], 'user/'.$user_fields->uid);
	}
  	$variables['updated_formatted_date'] = date("F j, Y", $variables['revision_timestamp']);
	$variables['updated'] = 'Last updated: '.$variables['updated_formatted_date'].' by '.$variables['formatted_user'];
	
	
	$date = DateTime::createFromFormat('D, m/d/Y - H:i', $variables['created']);
	
	
	$variables['datetime'] = format_date($date->getTimestamp(), 'custom', 'c');
}


function ex5_preprocess_page(&$variables){
	// Add preview link to content nodes
	if(isset($variables['node']) && 
						($variables['node']->type == 'article' || 
							$variables['node']->type == 'news' || 
							$variables['node']->type == 'faq')
							){
		$preview_url = theme_get_setting('preview_url');
		$preview_url = ($preview_url == ''?'http://www.demo.extension.org/preview/page/create/':$preview_url);
		$variables['tabs']['#primary'][] = array(
												'#theme'=>'menu_local_task',
												'#link' => array(
																'href'=> $preview_url.$variables['node']->nid,
																'title' => 'Preview'),
																'description' => 'Preview current page on the demo public site.',
												);
	}
	
}

// Providing variables for displaying modified time of the last revision and the full name of the author
function ex5_preprocess_node(&$variables) {
	//
	
	$user_id = $variables['revision_uid'];
	
	$user_fields = user_load($user_id);
	//dprint_r($variables);
	$variables['firstname'] = (isset($user_fields->field_first_name['und']['0']['value'])?$user_fields->field_first_name['und']['0']['value']:'');
	$variables['lastname'] = (isset($user_fields->field_last_name['und']['0']['value'])?$user_fields->field_last_name['und']['0']['value']:'');
	if($variables['firstname'] == ''){
		$variables['formatted_user'] = l($user_fields->name, 'user/'.$user_fields->uid);
	}else{
		$variables['formatted_user'] = l($variables['firstname'].' '.$variables['lastname'], 'user/'.$user_fields->uid);
	}
  	$variables['updated_formatted_date'] = date("F j, Y", $variables['revision_timestamp']);
	$variables['updated'] = 'Last updated: '.$variables['updated_formatted_date'].' by '.$variables['formatted_user'];
	
	$variables['datetime'] = format_date($variables['created'], 'custom', 'c');
  if (variable_get('node_submitted_' . $variables['node']->type, TRUE)) {
    $variables['submitted'] = t('Submitted by !username on !datetime',
      array(
        '!username' => $variables['name'],
        '!datetime' => '<time datetime="' . $variables['datetime'] . '" pubdate="pubdate">' . $variables['date'] . '</time>',
      )
    );
  }
  else {
    $variables['submitted'] = '';
  }
  $variables['unpublished'] = '';
  if (!$variables['status']) {
    $variables['unpublished'] = '<div class="unpublished">' . t('Unpublished') . '</div>';
  }
	
}

// Provide "last" class to the last member of this menu and include a preview link in it.
function ex5_menu_local_tasks(&$variables) {
  $output = '';
 //dsm($variables);
  $variables['primary'][count($variables['primary'])-1]['#link']['localized_options']['attributes']['class'][] = 'last';
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

// make the markup of the tree simple
function ex5_menu_tree($variables){
	return '<ul class="menu">' . $variables['tree'] . '</ul>';
}

//Remove local actions from some pages
function ex5_menu_local_action($variables) {
  $link = $variables['element']['#link'];
  if($link['title']=='Edit Panel'){
	  $output = '';
  }else{	
	  $output = '<li>';
	  if (isset($link['href'])) {
		$output .= l($link['title'], $link['href'], isset($link['localized_options']) ? $link['localized_options'] : array());
	  }
	  elseif (!empty($link['localized_options']['html'])) {
		$output .= $link['title'];
	  }
	  else {
		$output .= check_plain($link['title']);
	  }
	  $output .= "</li>\n";
  }
  return $output;
}


// Addng Global meta tags for implementing the html5 boilerplate template
function ex5_preprocess_html(&$vars) {
    // Create our meta variable for additional meta tags in the header
    $vars['meta'] = '';

    // SEO optimization, add in the node's teaser, or if on the homepage, the site slogan as a
    // description of the page that appears in search engines
    $vars['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
    // Grab the site name while we're at it
    $vars['site_name'] = filter_xss_admin(variable_get('site_name', ''));


    // Set the charset to UTF-8
    $vars['meta'] .= '<meta charset="UTF-8">' . "\n";

    // Meta description goodness. Still used by Google to give page descriptions in search results
    if ($vars['is_front'] != '') {
        $vars['meta'] .= '<meta name="description" content="' . drupal_html5_boilerplate_trim_text($vars['site_slogan']) . '" />' . "\n";
    } else if (isset($vars['node']->teaser) && $vars['node']->teaser != '') {
        $vars['meta'] .= '<meta name="description" content="' . drupal_html5_boilerplate_trim_text($vars['node']->teaser) . '" />' . "\n";
    } else if (isset($vars['node']->body) && $vars['node']->body != '') {
        $vars['meta'] .= '<meta name="description" content="' . drupal_html5_boilerplate_trim_text($vars['node']->body) . '" />' . "\n";
    }

    // Set the author. This could be changed to also look for a node author and change it accordingly
    $vars['meta'] .= '<meta name="author" content="' . $vars['site_name'] . '">' . "\n";

    // SEO optimization, if the node has tags, use these as keywords for the page
    if (isset($vars['node']->taxonomy)) {
        $keywords = array();
        foreach ($vars['node']->taxonomy as $term) {
            $keywords[] = $term->name;
        }
        $vars['meta'] .= '<meta name="keywords" content="' . implode(',', $keywords) . '" />' . "\n";
    }

    // SEO optimization, avoid duplicate titles in search indexes for pager pages
    if (isset($_GET['page']) || isset($_GET['sort'])) {
        $vars['meta'] .= '<meta name="robots" content="noindex,follow" />' . "\n";
    }

    // Add in the optimized mobile viewport: j.mp/bplateviewport
    $vars['meta'] .= '<meta name="viewport" content="width=device-width, initial-scale=1.0" />' . "\n";

    // Optional method to display an Apple touch icon. Disabled by default.
    // $vars['meta'] .= '<link rel="apple-touch-icon" href="/apple-touch-icon.png">';
    //
    // We're adding these additional javascripts to the header because they need to be loaded before
    // the page and I don't know of a better way to do it. Anyone is welcome to apply a better approach.
    //
    // Let's assign our path instead of calling the function over and over.
    $path_prefix = path_to_theme();

    // Pull in the touch icons and show some Apple love
    $vars['meta'] .= '<link rel="apple-touch-icon" href="/' . $path_prefix . '/apple-touch-icon.png">' . "\n";

    // Plant the modernizr js. We're planting it this way because we want to still have Drupal optimize
    // pages by keeping the script loading before the closing <body> tag.
    $vars['meta'] .= '<script src="/' . $path_prefix . '/js/libs/modernizr-1.7.min.js"></script>' . "\n";

    $vars['belatedpng'] = '<!--[if lt IE 7 ]>' . "\n" . '<script src="/' . $path_prefix . '/js/libs/dd_belatedpng.js"></script>' . "\n" . '<script> DD_belatedPNG.fix(\'img, .png_bg\'); </script>' . "\n" . '<![endif]-->';

   
}

function ex5_trim_text($text, $length = 150) {
    // remove any HTML or line breaks so these don't appear in the text
    $text = trim(str_replace(array("\n", "\r"), ' ', strip_tags($text)));
    $text = trim(substr($text, 0, $length));
    $lastchar = substr($text, -1, 1);
    // check to see if the last character in the title is a non-alphanumeric character, except for ? or !
    // if it is strip it off so you don't get strange looking titles
    if (preg_match('/[^0-9A-Za-z\!\?]/', $lastchar)) {
        $text = substr($text, 0, -1);
    }
    // ? and ! are ok to end a title with since they make sense
    if ($lastchar != '!' && $lastchar != '?') {
        $text .= '...';
    }
    return $text;
}