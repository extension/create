<?php

/**
 * Add body classes if certain regions have content.
 */
function drupex_preprocess_html(&$variables) {
  if (!empty($variables['page']['global_nav_bar'])) {
    $variables['classes_array'][] = 'global_nav_bar';
  }
}
