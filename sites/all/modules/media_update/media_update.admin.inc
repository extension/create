<?php
/**
 * Implements hook_form_FORM_ID_alter().
 * 
 *  Adds a checkbox to the file management form allowing users to replace not
 *  just the file entity, but also the physical file if desired. Replacement
 *  defaults to off.
 */
function media_update_form_system_file_system_settings_alter(&$form, &$form_state) {
  $media_update_settings = media_update_get_settings();
  $form['media_update'] = array(
    '#title' => t('Media Update'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );
  $form['media_update']['replace'] = array(
    '#type' => 'checkbox',
    '#title' => t('When editing a file, replace the file if the name is the same?'),
    '#default_value' => $media_update_settings['replace'],
  );
}