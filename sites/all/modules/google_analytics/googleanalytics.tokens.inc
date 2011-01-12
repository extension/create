<?php
// $Id: googleanalytics.tokens.inc,v 1.1.2.2 2011/01/02 15:35:59 hass Exp $

/**
 * @file
 * Builds placeholder replacement tokens for user-related data.
 */

/**
 * Implements hook_token_info().
 */
function googleanalytics_token_info() {
  $user['roles'] = array(
    'name' => t('User roles'),
    'description' => t('The roles the user account is a member of as comma separated list.'),
  );

  return array(
    'tokens' => array('user' => $user),
  );
}

/**
 * Implements hook_tokens().
 */
function googleanalytics_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'user' && !empty($data['user'])) {
    $account = $data['user'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Basic user account information.
        case 'roles':
          $roles = implode(',', $account->roles);
          $replacements[$original] = $sanitize ? check_plain($roles) : $roles;
          break;
      }
    }
  }

  return $replacements;
}
