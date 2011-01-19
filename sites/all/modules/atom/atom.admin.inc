<?php
// $Id: atom.admin.inc,v 1.3 2010/09/11 03:53:52 deekayen Exp $

/**
 * @file
 * Administrative page callbacks for the atom module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function atom_settings_form() {
  $form['atom_feed_entries'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of entries to include in feeds'),
    '#default_value' => 15,
    '#options' => drupal_map_assoc(range(1, 100)),
  );
  $form['atom_display_summary'] = array(
    '#type' => 'checkbox',
    '#title' => t('Output the summary (teaser)'),
    '#default_value' => TRUE,
  );
  $form['atom_display_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Output the full content (body)'),
    '#default_value' => TRUE,
  );

  _atom_contrib_load();
  $extra = module_invoke_all('atom_admin_settings');
  if (count($extra) > 0) {
    $form['extra'] = $extra;
    $form['extra']['#type'] = 'fieldset';
    $form['extra']['#title'] = t('Extra module settings');
  }

  return system_settings_form($form);
}
