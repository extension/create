<?php

function drupal_html5_boilerplate_preprocess_html(&$vars) {
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

    // Insert optional Yahoo profiling code. Comment out for production use.
    // $vars['meta'] .= '<script src="/' . $path_prefix . '/js/profiling/yahoo-profiling.min.js"></script>' . "\n";
    // $vars['meta'] .= '<script src="/' . $path_prefix . '/js/profiling/config.js"></script>' . "\n";

    // Insert qunit tests. Disabled by default
    // $vars['meta'] .= '<script src="/' . $path_prefix . '/js/qunit/qunit.js"></script>' . "\n";
    // $vars['meta'] .= '<script src="/' . $path_prefix . '/js/tests.js"></script>' . "\n";
    // $vars['meta'] .= '<link rel="stylesheet" href="/' . $path_prefix . '/css/qunit.css" media="screen">'. "\n";
    //
    // After enabling the libraries above you'll need to add this markup somehwere in your page output.
    //
    // <h1 id="qunit-header">QUnit Test Suite</h1>
    // <h2 id="qunit-banner"></h2>
    // <div id="qunit-testrunner-toolbar"></div>
    // <h2 id="qunit-userAgent"></h2>
    // <ol id="qunit-tests"></ol>
    // <div id="qunit-fixture">test markup</div>
}

function drupal_html5_boilerplate_preprocess_node(&$vars) {
  $vars['datetime'] = format_date($vars['created'], 'custom', 'c');
  if (variable_get('node_submitted_' . $vars['node']->type, TRUE)) {
    $vars['submitted'] = t('Submitted by !username on !datetime',
      array(
        '!username' => $vars['name'],
        '!datetime' => '<time datetime="' . $vars['datetime'] . '" pubdate="pubdate">' . $vars['date'] . '</time>',
      )
    );
  }
  else {
    $vars['submitted'] = '';
  }
  $vars['unpublished'] = '';
  if (!$vars['status']) {
    $vars['unpublished'] = '<div class="unpublished">' . t('Unpublished') . '</div>';
  }
}

function drupal_html5_boilerplate_trim_text($text, $length = 150) {
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