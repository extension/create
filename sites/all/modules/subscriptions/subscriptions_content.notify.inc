<?php

/**
 * @file
 * Subscriptions Content module (notify functions).
 */

/**
 * Implements hook_tokens().
 */
function subscriptions_content_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  //dpm($tokens, "subscriptions_content_tokens($type)");
  //dpm($data, "subscriptions_content_tokens($type)");
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the node.
        case 'node-html':
          $replacements[$original] = $node->nid;
          break;
      }
    }
  }

  if ($type == 'subs' && !empty($data['comment'])) {
    $comment = $data['comment'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'is-new':
          $replacements[$original] = (int) !empty($comment->_subscriptions_is_new);
          break;
        case 'is-updated':
          $replacements[$original] = (int) empty($comment->_subscriptions_is_new);
          break;
        case 'is-old':
          $replacements[$original] = 0;
          break;
        case 'is-published':
          $replacements[$original] = (int) ($comment->status == COMMENT_PUBLISHED);
          break;
      }
    }

    // files:field_FILE
    if (user_access('view uploaded files')) {
      $replacements += subscriptions_content_tokens_field_array($type, $tokens, $data, $options, 'comment', 'files', 'list<file>');
    }
  }
  elseif ($type == 'subs' && !empty($data['node'])) {
    $node = $data['node'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'is-new':
          $replacements[$original] = (int) !empty($node->_subscriptions_is_new);
          break;
        case 'is-updated':
          $replacements[$original] = (int) !empty($node->_subscriptions_is_updated);
          break;
        case 'is-old':
          $replacements[$original] = (int) (empty($node->_subscriptions_is_new) && empty($node->_subscriptions_is_updated));
          break;
        case 'is-published':
          $replacements[$original] = $node->status;
          break;
      }
    }

    if (($comments_tokens = token_find_with_prefix($tokens, 'comments')) && !empty($node->_subscriptions_comments)) {
      if (empty($node->_subscriptions_comments_rendered)) {
        foreach ($node->_subscriptions_comments as $comment) {
          $node->_subscriptions_comments_rendered[] = mail_edit_format($data['template']['subscriptions_comment_body'], $data + array('comment' => $comment), $options);
        }
      }
      $replacements += token_generate('array', $comments_tokens, array(
        'array' => (isset($node->_subscriptions_comments_rendered) ? $node->_subscriptions_comments_rendered : array()),
      ), array('sanitize' => FALSE) + $options);
    }

    // files:field_FILE
    if (user_access('view uploaded files')) {
      $replacements += subscriptions_content_tokens_field_array($type, $tokens, $data, $options, 'node', 'files', 'list<file>');
    }
  }

  return $replacements;
}

function subscriptions_content_tokens_field_array($type, $tokens, array $data, array $options, $data_type, $token, $token_type)
{
  $replacements = array();

  if ($files_tokens = token_find_with_prefix($tokens, $token)) {

    $url_options = array('absolute' => TRUE);
    if (isset($options['language'])) {
      $url_options['language'] = $options['language'];
      $language_code = $options['language']->language;
    }
    else {
      $language_code = NULL;
    }

    $tokenss = array();
    foreach ($files_tokens as $files_token => $original) {
      $files_field = strtok($files_token, ':');
      $tokenss[$files_field][substr($files_token, strlen($files_field) + 1)] = $original;
    }
    foreach ($tokenss as $files_field => $tokens) {
      if ($files = field_get_items($data_type, $data[$data_type], $files_field, $language_code)) {
        foreach ($files as $key => $file) {
          $file_objs[$key] = (object) $file;
        }
        if (!empty($file_objs)) {
          $replacements += token_generate($token_type, $tokens, array($token_type => $file_objs), $options);
        }
      }
    }
  }
  return $replacements;
}

/**
 * Returns a node if published, including any comments that are still queued, but
 * limited by the given subscriptions queue ID.
 *
 * @param int $nid
 * @param int|null $comment_load_sqid
 * @internal param array $subs
 *
 * @return object|null
 */
function _subscriptions_content_load($nid, $comment_load_sqid = NULL) {
  global $user;

  $node = node_load($nid, NULL, TRUE);
  // Note: we must not cache across users (access checking), and we take care
  // not to process the same node more than once (except for multiple batches
  // of comments), so we don't gain from caching nodes; on the contrary: we
  // might run out of memory!

  if (!empty($node)) {
    if (module_exists('comment')) {
      $published_comments_only = $limit_sqids = '';

      // Load comments like comment_get_thread().
      $query = db_select('comment', 'c');
      $query->addField('c', 'cid');
      $query
        ->condition('c.nid', $nid)
        ->addTag('node_access')
        ->addTag('comment_filter')
        ->addMetaData('node', $node);

      $query->join('subscriptions_queue', 'sq', (db_driver() != 'pgsql' ? 'c.cid = sq.load_args' : 'CAST(c.cid AS VARCHAR) = sq.load_args'));
      $query->addField('sq', 'sqid', '_subscriptions_sqid');
      $query->addField('sq', 'is_new', '_subscriptions_is_new');
      $query->condition('sq.uid', $user->uid);
      $query->condition('sq.load_function', 'subscriptions_content_load_comment');

      if (!user_access('administer comments')) {
        $query->condition('c.status', COMMENT_PUBLISHED);
      }
      $query->orderBy('c.cid', 'ASC');

      if (!empty($comment_load_sqid)) {
        // check for a later queued update notification (don't send comments past that one because it will go out as node/type with its own comments later!)
        if ($cutoff_sqid = db_query_range("SELECT sqid FROM {subscriptions_queue} WHERE module = 'node' AND field = 'nid' AND value = :value AND uid = :uid AND load_function = 'subscriptions_content_load_node' AND sqid > :sqid", 0, 1, array(
          ':value' => $nid,
          ':uid' => $user->uid,
          ':sqid' => $comment_load_sqid))->fetchField()
        ) {
          $query->condition('q.sqid', $cutoff_sqid, '<');
        }
      }

      $sqids = $cids = array();
      foreach ($query->execute() as $comment_info) {
        if ($comment_info && user_access('access comments') && !isset($comment_infos[$comment_info->cid])) {
          $cids[] = $comment_info->cid;
          $comment_infos[$comment_info->cid] = $comment_info;
        }
        $sqids[] = $comment_info->_subscriptions_sqid;
      }
      if ($sqids) {
        db_delete('subscriptions_queue')
          ->condition('sqid', $sqids, 'IN')
          ->execute();
      }

      if ($cids) {
        $comments = comment_load_multiple($cids);
        foreach ($comments as $cid => $comment) {
          $comment->_subscriptions_sqid = $comment_infos[$cid]->_subscriptions_sqid;
          $comment->_subscriptions_is_new = $comment_infos[$cid]->_subscriptions_is_new;
        }
        $node->_subscriptions_comments = $comments;
      }
    }
    return $node;
  }
  return NULL;
}

