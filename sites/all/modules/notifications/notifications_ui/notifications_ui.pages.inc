<?php
// $Id: notifications_ui.pages.inc,v 1.1.2.3.6.8.2.1 2011/01/18 01:36:24 jareyero Exp $
/**
 * @file
 *   User pages for User Interface for subscriptions modules
 */
 
/**
 * Page callback: Add subscription form
 */ 
function notifications_ui_page_add_subscription($type = NULL) {
  $account = $GLOBALS['user'];
  $subscription = notifications_subscription($type)
    ->instance()
    ->set_user($account);
  return drupal_get_form('notifications_subscription_add_form', $subscription);
}
