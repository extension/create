<?php

/**
 * @file
 * A sample implementation of Mail Editor hooks for Logintoboggan module.
 *
 * Since the mail_edit module is arranged around the full key (including the calling module)
 * it can only support user_ keys at this time.
 *
 * However, the Logintoboggan module (http://drupal.org/project/logintoboggan) replaces many of the
 * mail messages and sends them itself. As a result, mail_edit is unable to deal with them.
 *
 * This simple file allows logintoboggan to work. I think a more generic approach would
 * work better (not keying things to the full mailkey ('module' + '_' + 'key') but rather
 * on the key itself.
 *
 */

/**
 * Implements hook_mailkeys().
 */
function logintoboggan_mailkeys() {
  if (module_exists('logintoboggan')) {
    return user_mailkeys() + array(
      'logintoboggan_resend_validation' => t('Resent validation message'),
    );
  }
}

/**
 * Implements hook_mail_edit_text().
 */
function logintoboggan_mail_edit_text($mailkey, $langcode) {
  return user_mail_edit_text($mailkey, $langcode);
}

/**
 * Implements hook_mail_edit_token_types().
 */
function logintoboggan_mail_edit_token_types($mailkey) {
  return user_mail_edit_token_types($mailkey);
}
