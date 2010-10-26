<?php
// $Id: messaging.manage.inc,v 1.1.2.3.2.7.2.12 2010/07/21 10:55:30 jareyero Exp $

/**
 * @file
 *   Common functions for managing destinations
 */


/**
 * Manage destination form. Edit subscriptions for a destination
 */
function messaging_manage_destination_form($form_state, $destination) {
  module_load_include('inc', 'messaging', 'messaging.pages');
  messaging_include('destination.inc');
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    $form = messaging_multiple_delete_confirm($form_state, array_filter($form_state['values']['subscriptions']));
    return $form;
  }
  //$form = messaging_destination_view_subform($destination);
  $form['description'] = messaging_destination_subform($destination);
  $form['admin'] = messaging_destination_manage_subform($destination);
  $form['admin'] += messaging_manage_subscriptions_form_options($destination);
  $form['extra'] = messaging_manage_destination_extra($destination); 
  return $form;
}

/**
 * Manage destination extra options for administrators
 */
function messaging_manage_destination_extra($destination) {
  if (user_access('administer messaging') && function_exists('messaging_anonymous_manage_links')) {
    foreach (_messaging_anonymous_manage_links('destination', $destination) as $link) {
      $url = url($link['href'], $link['options']);
      $rows[] = array(
        $link['title'],
        l($url, $url),
      );
    }
    $form['links'] = array(
      '#type' => 'item',
      '#title' => t('Anonymous links for this destination'),
      '#value' => theme('table', array(), $rows),
    );
    return $form;
  }
  else {
    return array();
  }
}