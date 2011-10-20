<?php

/**
 * @file
 * Subscriptions module mail templates constants.
 */

define('SUBSCRIPTIONS_DIGEST_MAILKEY', 'subscriptions_mail_digest');

function subscriptions_mail_template_raw($key, $langcode) {
  switch ($key) {

    case 'SEP':      return // SUBSCRIPTIONS_DEFAULT_SEPARATOR
t('| ------------------------------------------------------------------------', array(), array('langcode' => $langcode));

    case 'SUBJ':     return // SUBSCRIPTIONS_DEFAULT_SUBJECT
t('[[site:name]] [subs:type] subscription for [current-user:name]: [node:title]', array(), array('langcode' => $langcode));

    case 'FOOT':     return // SUBSCRIPTIONS_DEFAULT_FOOTER
t('| This is an automated message. Please do NOT reply to the sender address!
| To manage your subscriptions go to
| [subs:manage-url]
', array(), array('langcode' => $langcode));

    case 'BODY':     return // SUBSCRIPTIONS_DEFAULT_BODY
t('| Greetings, [current-user:name].
|
| Your subscription on [site:name]
| notifies you of {{[subs:is-new]==0?:a new post:
}}{{[subs:is-updated]#an updated post:
}}{{[subs:is-old]#new comments:
}}|
{{![subs:is-published]#| ***** This post is unpublished! *****
}}{{![term:name]?:| Category: [term:name]
}}{{![user:name]?:| Author: [user:name]
}}| Title: [node:title]
&nbsp;
{{[subs:is-old]?{{[node:summary]?[node:summary]:[node:body]}}:[node:body]}}
| LINK: [node:url]
{{!![subs:files:field_files:count]#| Attached files:
{{[subs:files:field_files:count]#| [subs:files:field_files:value:#0:url]
}}}}{{![subs:comments:count]?!SEPARATOR
:|
| Comments: [subs:comments:count]
!SEPARATOR
[subs:comments:join:
]
}}| Direct unsubscribe link ([subs:type]):
| [subs:unsubscribe-url]
!FOOTER', array('!SEPARATOR' => subscriptions_mail_template_raw('SEP', $langcode), '!FOOTER' => subscriptions_mail_template_raw('FOOT', $langcode)), array('langcode' => $langcode));

    case 'CITEM':    return // SUBSCRIPTIONS_DEFAULT_COMMENT_ITEM
t('| {{[subs:is-new]?New:Updated}} {{[subs:is-published]?:UNPUBLISHED }}comment:
| Author: [comment:author:name]
| Title: [comment:title]
&nbsp;
[comment:body]
| LINK: [comment:url]
{{!![subs:files:field_files:count]#| Attached files:
{{[subs:files:field_files:count]#| [subs:files:field_files:value:#0:url]
}}}}!SEPARATOR', array('!SEPARATOR' => subscriptions_mail_template_raw('SEP', $langcode)), array('langcode' => $langcode));

    case 'DSUBJ':    return // SUBSCRIPTIONS_DEFAULT_DIGEST_SUBJECT
t('[[site:name]] Subscription Digest for [current-user:name]', array(), array('langcode' => $langcode));

    case 'DBODY':    return // SUBSCRIPTIONS_DEFAULT_DIGEST_BODY
t('| Greetings, [current-user:name].
|
| Your subscriptions on [site:name]
| notify you of the following changes since the previous digest:
|
[subs:nodes]!SEPARATOR
!FOOTER', array('!SEPARATOR' => subscriptions_mail_template_raw('SEP', $langcode), '!FOOTER' => subscriptions_mail_template_raw('FOOT', $langcode)), array('langcode' => $langcode));

    case 'DITEM':    return // SUBSCRIPTIONS_DEFAULT_DIGEST_ITEM_BODY
t('{{[user:name]!=?:| Author: [user:name]
}}| Title: [node:title]
{{[subs:is-new]==0?:[node:summary]
}}| Link: [node:url]
| Direct unsubscribe link ([subs:type]):
| [subs:unsubscribe-url]
{{[subs:comments:count]>0?:[subs:comments]}}', array(), array('langcode' => $langcode));

    case 'DITEMCMT': return // SUBSCRIPTIONS_DEFAULT_DIGEST_ITEM_BODY_COMMENT
t('| Comment: [comment:title]
', array(), array('langcode' => $langcode));

  }
}

function subscriptions_mail_template($key, $langcode) {
  static $templates;
  if (!isset($templates[$langcode][$key])) {
    $templates[$langcode][$key] = subscriptions_mail_template_raw($key, $langcode);
  }
  return $templates[$langcode][$key];
}

/**
 * Retrieve a mail template, either from the mail_edit table or take
 * our default template in this file, passing it through t().
 *
 * @param string $mailkey
 *  the full mailkey, used as id in the mail_edit table
 * @param string $langcode
 *  the language code, used as language in the mail_edit table or for
 *  translating out default template
 * @param string $part
 *  either 'subject' or 'body', used to indicate the column to retrieve
 *  from the mail_edit table
 * @param string $key
 *  the key for calling subscriptions_mail_template() to retrieve the
 *  default template, if there's no match in the mail_edit table
 */
function subscriptions_mail_template_load($mailmod, $mailkey, $langcode, $part, $key) {
  if (module_exists('mail_edit')) {
    $id = $mailmod . '_' . $mailkey;
    static $cache = array();
    if (!isset($cache[$langcode][$id])) {
      $query = db_select('mail_edit', 'me')
        ->fields('me')
        ->condition('me.id', $id)
        ->condition('me.language', $langcode);
      $cache[$langcode][$id] = $query->execute()->fetchObject();
    }
    if (isset($cache[$langcode][$id]->$part)) {
      return $cache[$langcode][$id]->$part;
    }
  }
  return subscriptions_mail_template($key, $langcode);
}

