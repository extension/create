<?php

/**
 * @file
 * Subscriptions_mail module integration with Mail Editor module.
 */

/**
 * Implements hook_mail_edit_text().
 *
 * Provide default templates for mail_edit.
 *
 * @param $mailkey
 * @param $language
 *
 * @return array
 *
 * @ingroup hooks
 */
 function _subscriptions_mail_mail_edit_text($mailkey, $language) {
  $langcode = $language->language;
  _subscriptions_mail_module_load_include('templates.inc');
  $return = array();
  // TODO: Implement a hook to allow other modules to provide their default templates.
  if (substr($mailkey, 0, 5) == 'node-') {
    $return['subject'] = subscriptions_mail_template('SUBJ', $langcode);
    $return['body'] = subscriptions_mail_template('BODY', $langcode);
    $return['subscriptions_comment_body'] = subscriptions_mail_template('CITEM', $langcode);
  }
  else {
    $return['subject'] = subscriptions_mail_template('DSUBJ', $langcode);
    $return['body'] = subscriptions_mail_template('DBODY', $langcode);
  }
  $return['always'] = TRUE;
  return $return;
}

/**
 * Implements hook_mail_edit_token_types().
 *
 * Returns the available token types.
 *
 * @param $mailkey
 * @param array $options
 * @return array
 *
 * @ingroup hooks
 */
function _subscriptions_mail_mail_edit_token_types($mailkey, $options = array()) {
  global $language;
  $massage_tokens = &drupal_static('subscriptions_mail_massage_tokens');
  $massage_tokens = FALSE;

  if (module_exists('token')) {
    //dpm(token_get_info(), "TRIGGER HACK");
    // Clear token.module's caches.
    cache_clear_all("info:{$language->language}", 'cache_token');
    $token_get_info = &drupal_static('token_get_info');
    $token_get_info = NULL;
    $token_info = &drupal_static('token_info');
    $token_info = NULL;
    // We want to poison token.module's caches now!
    $massage_tokens = TRUE;
    // Trigger a refresh of token.module's caches.
    token_get_info();
    //dpm(token_get_info(), "HACK DONE");
    // Clear the permanent cache, keeping token.module's static caches.
    cache_clear_all("info:{$language->language}", 'cache_token');
  }

  $token_types = module_invoke_all('subscriptions', 'token_types', substr($mailkey, 19));
  $token_types = array_merge($token_types, ($mailkey == 'subscriptions_mail_digest' ? array('subs') : array('user', 'subs')));
  return $token_types;
}

/**
 * Implements hook_tokens().
 *
 * @param $type
 * @param $tokens
 * @param array $data
 * @param array $options
 *
 * @return array
 */
function subscriptions_mail_tokens($type, $tokens, array $data = array(), array $options = array())
{
  global $user;

  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();
  if ($type == 'subs' && isset($data['subs'])) {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'type':
          $replacements[$original] = $data['subs']['type'];
          break;
        case 'manage-url':
          $replacements[$original] = url('user/' . $user->uid . '/subscriptions', $url_options);
          break;
        case 'unsubscribe-url':
          $replacements[$original] = url($data['subs']['unsubscribe_path'], $url_options);
          break;
        case 'module':
          $replacements[$original] = $data['subs']['module'];
          break;
        case 'field':
          $replacements[$original] = $data['subs']['field'];
          break;
        case 'value':
          $replacements[$original] = $data['subs']['value'];
          break;
        case 'forum':
          if (isset($data['node']['forum_tid'])) {
            $replacements[$original] = render(taxonomy_term_view(taxonomy_term_load($data['node']['forum_tid']), 'full', $language_code));
          }
          break;
      }
    }

    // Chained token relationships.
    if ($url_tokens = token_find_with_prefix($tokens, 'manage-url')) {
      $replacements += token_generate('url', $url_tokens, array('path' => 'user/' . $user->uid . '/subscriptions'), $options);
    }
    if ($url_tokens = token_find_with_prefix($tokens, 'unsubscribe-url')) {
      $replacements += token_generate('url', $url_tokens, array('path' => $data['subs']['unsubscribe_path']), $options);
    }
    if ($forum_tokens = token_find_with_prefix($tokens, 'forum')) {
      $replacements += token_generate('term', $forum_tokens, array('term' => taxonomy_term_load($data['node']['forum_tid'])), $options);
    }
  }
  return $replacements;
}

/**
 * Implements hook_mail_edit_form_extra().
 *
 * Customize Mail Editor's edit template page.
 *
 * @param array $form
 * @param array $form_state
 * @param $mailkey
 * @param array $template
 */
function _subscriptions_mail_mail_edit_form_extra(array &$form, array &$form_state, $mailkey, array $template) {
  $form['mail']['body']['#description'] = t('The mail body contains one node of the named type. Recommended tokens: <ul><li>@t1, except for @t2,</li> <li>@t3, @t4, and @t5,</li> <li>!t6, if comments should be included.</li></ul>', array(
    '@t1' => '[subs:*]',
    '@t2' => '[subs:nodes]',
    '@t3' => '[node:*]',
    '@t4' => '[user:*]',
    '@t5' => '[current-user:*]',
    '!t6' => '<code>{{[subs:comments:count]#...[subs:comments:value:#0]...}}</code>',
  ));
  if (isset($template['subscriptions_comment_body'])) {
    $form['mail']['subscriptions_comment_body'] = array(
      '#title'         => t('Comment'),
      '#type'          => 'textarea',
      '#default_value' => $template['subscriptions_comment_body'],
      '#rows'          => 5,
      '#description'   => t('This template formats one comment for inclusion in the @Body template above. Recommended tokens:<ul><li>@t1, @t2, @t3, @t4, and @t5, if available,</li> <li>@t6 and @t7.</li></ul>', array(
        '@Body'        => t('Body'),
        '@t1'          => '[subs:is-new]',
        '@t2'          => '[subs:is-updated]',
        '@t3'          => '[subs:is-published]',
        '@t4'          => '[subs:type]',
        '@t5'          => '[subs:files:field_FILES]',
        '@t6'          => '[comment:*]',
        '@t7'          => '[comment:author:*]',
      )),
      '#weight'        => 5,
    );
  }
}

/**
 * Implements hook_form_alter().
 *
 * Adds the digest parts to the subscriptions_mail_digest mail_edit page.
 *
 * @param $form
 * @param $form_state
 *
 * @ingroup hooks
 * @ingroup form
 */
function _subscriptions_mail_form_mail_edit_template_form_alter(&$form, &$form_state) {
  $mailkey = $form['id']['#value'];
  $type_ = 'subscriptions_mail_node-type-';
  if (strpos($mailkey, $type_) === 0) {
    $form['content_type'] = array(
      '#type'  => 'hidden',
      '#value' => substr($mailkey, strlen($type_)),
    );
  }

  if (isset($form_state['template_preview'])) {
    $form['template_preview'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Template preview'),
      '#collapsible' => TRUE,
      '#weight'      => -5,
    )
    + $form_state['template_preview'];
  }

  $form['preview'] = array(
    '#type'   => 'submit',
    '#value'  => t('Preview template'),
    '#submit' => array('subscriptions_mail_preview'),
    '#weight' => 7,
  );
}

/**
 * Submit handler for the 'Preview template' button.
 *
 * @param $form array
 * @param $form_state array
 */
function _subscriptions_mail_preview(array $form, array &$form_state) {
  $values = $form_state['values'];
  if (isset($values['preview_mode'])) {
    variable_set('subscriptions_preview_mode', $values['preview_mode']);
  }
  $form_state['template_preview'] = _subscriptions_mail_preview_form($values);
  $form_state['rebuild'] = TRUE;
}

/**
 * Returns the template preview subform.
 *
 * @param $values array
 *
 * @return array
 */
function _subscriptions_mail_preview_form(array $values) {
  $language = language_initialize($values['language']);
  $data = array();
  $query = db_select('node', 'n')
    ->fields('n', array('nid'));
  if (isset($values['content_type'])) {
    $query
      ->condition('type', $values['content_type']);
  }
  $nid = $query
    ->orderBy('n.nid', 'DESC')
    ->addTag('node_access')
    ->execute()
    ->fetchField();
  $data['node'] = $node = node_load($nid);
  $node->_subscriptions_is_new = TRUE;

  $cids = comment_get_thread($node, COMMENT_MODE_FLAT, 5);
  $node->_subscriptions_comments = array();
  if (!empty($cids)) {
    foreach (comment_load_multiple($cids) as $comment) {
      // Fake every other comment as new.
      $comment->_subscriptions_is_new = $comment->cid%2;
      $node->_subscriptions_comments[] = $comment;
    }
  }

  if (isset($node->forum_tid)) {
    $data['term'] = taxonomy_term_load($node->forum_tid);
  }

  $data['user'] = user_load(!empty($node->revision_uid) ? $node->revision_uid : $node->uid);
  $data['subs'] = array(
    'type'             => (isset($values['content_type']) ? t('content type') : t('thread')),
    'unsubscribe_path' => 's/del/...',
    'module'           => 'module',
    'field'            => 'field',
    'value'            => 'value',
   );
  $data['template'] = $values;

  $subject = mail_edit_format($values['subject'], $data, array('language' => $language));
  $body = mail_edit_format($values['body'], $data, array('language' => $language));
  switch ($preview_mode = variable_get('subscriptions_preview_mode','text')) {
    case 'text':
      $body = drupal_html_to_text($body);
    case 'html-raw':
      $body = check_plain($body);
      $body = '<pre>' . $body . '</pre>';
      break;
    case 'html-rendered':
      $body = '<div>' . filter_xss_admin($body) . '</div>';
      break;
  }
  $form['preview_subject'] = array(
    '#markup' => '<h4><u>' . $subject . "</u></h4><br />",
  );
  $form['preview_body'] = array(
    '#markup' => $body,
  );
  $form['preview_mode'] = array(
    '#type'          => 'select',
    '#prefix'        => '<div style="float: left; margin-right: 2em;">',
    '#suffix'        => '</div>',
    '#title'         => t('Preview mode'),
    '#default_value' => $preview_mode,
    '#options'       => array(
      'text'           => t('Text (default)'),
      'html-raw'       => t('Raw HTML'),
      'html-rendered'  => t('Rendered HTML'),
    ),
  );
  $form['preview_mode_info'] = array(
    '#type' => 'item',
    '#description' => t("The input to drupal_mail() is HTML. Drupal's default mail system converts it to text before sending it out as an email message. Sending HTML mail is possible by installing an alternative mail system and converting the template to HTML.", array('@drupal_mail', 'drupal_mail()')),
  );
  return $form;
}

