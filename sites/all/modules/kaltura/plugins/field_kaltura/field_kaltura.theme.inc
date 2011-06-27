<?php
// $Id$
/*
 * @file
 * themeing functions for kaltura fields
 */
function theme_kaltura_metadata($metadata) {
  foreach ($metadata as $key => $val) {
    $key = preg_replace('/_/', ' ', $key);
    $met[] = $key . ' :' . $val;
  }
  $out = theme('item_list', array('items' => $met, 'title' => t('Kaltura metadata'), 'type' => 'ul'));
  return $out;

}
function theme_kaltura_player_default($vars) {
  if (variable_get('kaltura_add_html5', 1) == 1 && !empty($vars['embed']['js'])) {
    drupal_add_js($vars['embed']['js'], 'external');
    drupal_add_js('mw.setConfig( \'EmbedPlayer.EnableIframeApi\', false );', 'inline');
  }
  $out = '<div class="kaltura">';
  $out .= '<h3 class="kaltura-title">' . check_plain($vars['title']) . '</h3>';
  $out .= '<div class="kaltura-embed">' . $vars['embed']['html'] . '</div>';
  $out .= '<div class="kaltura-desc">' . check_plain($vars['desc']) . '</div>';
  $out .= '</div>';
  return $out;

}

function theme_kaltura_player($vars) {
  if (variable_get('kaltura_add_html5', 1) == 1 && !empty($vars['embed']['js'])) {
    drupal_add_js($vars['embed']['js'], 'external');
    drupal_add_js('mw.setConfig( \'EmbedPlayer.EnableIframeApi\', false );', 'inline');
  }
  $out = '<div class="kaltura">';
  $out .= '<div class="kaltura-embed">' . $vars['embed']['html'] . '</div>';
  $out .= '</div>';
  return $out;

}

function theme_kaltura_player_title($vars) {
  if (variable_get('kaltura_add_html5', 1) == 1 && !empty($vars['embed']['js'])) {
    drupal_add_js($vars['embed']['js'], 'external');
    drupal_add_js('mw.setConfig( \'EmbedPlayer.EnableIframeApi\', false );', 'inline');
  }
  $out = '<div class="kaltura">';
  $out .= '<h3 class="kaltura-title">' . check_plain($vars['title']) . '</h3>';
  $out .= '<div class="kaltura-embed">' . $vars['embed']['html'] . '</div>';
  $out .= '</div>';
  return $out;

}

function theme_kaltura_player_meta($vars) {
  if (variable_get('kaltura_add_html5', 1) == 1 && !empty($vars['embed']['js'])) {
    drupal_add_js($vars['embed']['js'], 'external');
    drupal_add_js('mw.setConfig( \'EmbedPlayer.EnableIframeApi\', false );', 'inline');
  }
  $out = '<div class="kaltura">';
  $out .= '<h3 class="kaltura-title">' . check_plain($vars['title']) . '</h3>';
  $out .= '<div class="kaltura-embed">' . $vars['embed']['html'] . '</div>';
  $out .= '<div class="kaltura-desc">' . check_plain($vars['desc']) . '</div>';
  $out .= '<div class="kaltura-metadata">' . theme('kaltura_metadata', $vars['meta']) . '</div>';
  $out .= '</div>';
  return $out;

}
function theme_kaltura_thumbnail_default($vars) {
  $rot = ($vars['rotate']) ? 'k-rotate' : 'k-no-rotate';
  $out = '<div class="kaltura">';
  $out .= '<h3 class="kaltura-title">' . check_plain($vars['title']) . '</h3>';
  $out .= '<div class="kaltura-thumb">' . theme('image', array('path' => $vars['thub'], 'title' => $vars['title'], 'attributes' => array('class' => $rot))) . '</div>';
  $out .= '</div>';
  return $out;

}

function theme_kaltura_thumbnail($vars) {
  $rot = ($vars['rotate']) ? 'k-rotate' : 'k-no-rotate';
  $out = '<div class="kaltura">';
  $out .= '<div class="kaltura-thumb">' . theme('image', array('path' => $vars['thub'], 'title' => $vars['title'], 'attributes' => array('class' => $rot))) . '</div>';
  $out .= '</div>';
  return $out;

}

function theme_kaltura_thumbnail_title_desc($vars) {
  $rot = ($vars['rotate']) ? 'k-rotate' : 'k-no-rotate';
  $out = '<div class="kaltura">';
  $out .= '<h3 class="kaltura-title">' . check_plain($vars['title']) . '</h3>';
  $out .= '<div class="kaltura-thumb">' . theme('image', array('path' => $vars['thub'], 'title' => $vars['title'], 'attributes' => array('class' => $rot))) . '</div>';
  $out .= '<div class="kaltura-desc">' . check_plain($vars['desc']) . '</div>';
  $out .= '</div>';
  return $out;

}

function theme_kaltura_thumbnail_meta($vars) {
  $rot = ($vars['rotate']) ? 'k-rotate' : 'k-no-rotate';
  $out = '<div class="kaltura">';
  $out .= '<h3 class="kaltura-title">' . check_plain($vars['title']) . '</h3>';
  $out .= '<div class="kaltura-thumb">' . theme('image', array('path' => $vars['thub'], 'title' => $vars['title'], 'attributes' => array('class' => $rot))) . '</div>';
  $out .= '<div class="kaltura-desc">' . check_plain($vars['desc']) . '</div>';
  $out .= '<div class="kaltura-metadata">' . theme('kaltura_metadata', $vars['meta']) . '</div>';
  $out .= '</div>';
  return $out;

}
