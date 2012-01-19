<?php
/**
 * @file
 * Modifies the files settings admin form to provide additional setting options.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 *  Adds a checkbox to the file management form allowing users to replace not
 *  just the file entity, but also the physical file if desired. Replacement
 *  defaults to off.
 */
function media_update_form_system_file_system_settings_alter(&$form, &$form_state) {
  $form['media_update'] = array(
    '#title'  => t('Media Update'),
    '#type'   => 'item',
    '#value'  => '',
  );
  $form['media_update'][MEDIA_UPDATE_REPLACE_SAME] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Replace existing file when uploading a file with the same name.'),
    '#description'   => t('When checked, will remove the previous version of the file and replace it with the new version.') .
                        '<br />' .
                        t('When not checked, the original file will remain in the files folder and the new file will be renamed.'),
    '#default_value' => variable_get(MEDIA_UPDATE_REPLACE_SAME, 0),
  );
  $form['media_update'][MEDIA_UPDATE_REPLACE_DIFFERENT] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Replace (remove) existing file when uploading a file with a different name.'),
    '#description'   => t('When checked, will remove the previous version of the file and replace it with the new version.') .
                        '<br />' .
                        t('When not checked, the original file will remain in the files folder along with the new file.'),
    '#default_value' => variable_get(MEDIA_UPDATE_REPLACE_DIFFERENT, 0),
  );
}