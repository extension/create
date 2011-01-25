<?php
// $Id: atom.pages.inc,v 1.12 2010/09/11 03:53:52 deekayen Exp $

/**
 * @file
 * Various non-administration page callbacks for the atom module.
 */

/**
 * Produces an atom 1.0 feed for the front page content.
 */
function atom_feed() {
  $query = db_select('node');
  $query->addField('node', 'nid');
  $query->condition('promote', 1);
  $query->condition('status', 1);
  $query->orderBy('sticky', 'DESC');
  $query->orderBy('created', 'DESC');
  $query->range(0, variable_get('atom_feed_entries', 15));
  $query->addTag('node_access');
  $nodes = $query->execute()->fetchCol();

  $feed_info = array();
  $feed_info['html_url'] = url('', array('absolute' => TRUE));
  $feed_info['atom_url'] = url('atom.xml', array('absolute' => TRUE));
  _atom_print_feed($nodes, $feed_info);
}

/**
 * Query for all blog feeds.
 */
function atom_blog_feed() {
  $query = db_select('node');
  $query->addField('node', 'nid');
  $query->condition('type', 'blog');
  $query->condition('status', 1);
  $query->orderBy('sticky', 'DESC');
  $query->orderBy('created', 'DESC');
  $query->range(0, variable_get('atom_feed_entries', 15));
  $query->addTag('node_access');
  $nodes = $query->execute()->fetchCol();

  $feed_info = array();
  $feed_info['title']    = t('!site_name blogs', array('!site_name' => variable_get('site_name', 'Drupal')));
  $feed_info['html_url'] = url('blog', array('absolute' => TRUE));
  $feed_info['atom_url'] = url('blog/atom.xml', array('absolute' => TRUE));
  _atom_print_feed($nodes, $feed_info);
}

/**
 * Query for node feeds.
 */
function atom_node_feed(stdClass $node) {
  if (!node_access('view', $node)) {
    return drupal_access_denied();
  }

  $nodes = array($node->nid);

  $feed_info = array();
  $feed_info['html_url'] = url('node/' . $node->nid, array('absolute' => TRUE));
  $feed_info['atom_url'] = url('node/' . $node->nid . '/atom.xml', array('absolute' => TRUE));
  _atom_print_feed($nodes, $feed_info);
}

/**
 * Query for a single blog feed.
 */
function atom_user_blog_feed(stdClass $account) {
  $query = db_select('node');
  $query->addField('node', 'nid');
  $query->condition('type', 'blog');
  $query->condition('uid', $account->uid);
  $query->condition('status', 1);
  $query->orderBy('sticky', 'DESC');
  $query->orderBy('created', 'DESC');
  $query->range(0, variable_get('atom_feed_entries', 15));
  $query->addTag('node_access');
  $nodes = $query->execute()->fetchCol();

  $feed_info = array();
  $feed_info['title']    = t("!name's blog", array('!name' => $account->name));
  $feed_info['subtitle'] = '';
  $feed_info['html_url'] = url('blog/' . $account->uid, array('absolute' => TRUE));
  $feed_info['atom_url'] = url("blog/' . $account->uid . '/atom.xml", array('absolute' => TRUE));
  _atom_print_feed($nodes, $feed_info);
}

/**
 * Query by taxonomy.
 */
function atom_taxonomy_feed(stdClass $term) {
  $nodes = taxonomy_select_nodes(array($term->tid), NULL, NULL, TRUE);

  $feed_info['title']    = $term->name;
  $feed_info['subtitle'] = $term->description;
  $feed_info['html_url'] = url('taxonomy/term/' . $term->tid, array('absolute' => TRUE));
  $feed_info['atom_url'] = url("taxonomy/term/' . $term->tid . '/atom.xml", array('absolute' => TRUE));
  _atom_print_feed($nodes, $feed_info);
}

function _atom_print_feed(array $nids, array $feed_info) {
  _atom_contrib_load();
  $feed_info['extra_ns'] = _atom_contrib_get_ns();
  $output = '';
  $last_mod = 0;

  $nodes = node_load_multiple($nids);
  foreach ($nodes as $node) {
    $item_text = '';

    $node->link = url("node/$node->nid", array('absolute' => TRUE));

    node_build_content($node, 'rss');

    // Allow modules to change $node, or add elements,
    // specifically for an atom feed.
    $extra = module_invoke_all('atom_feed', $node);

    if (!empty($node->content)) {
      // We render node contents and force links to be last.
      $links = drupal_render($node->content['links']);
      $item_text .= drupal_render($node->content) . $links;
    }

    $item = array(
      'published' => $node->created,
      'updated' => $node->changed,
      'author' => $node->name ? $node->name : variable_get('anonymous', 'Anonymous'),
      'content' => $item_text,
      // @todo How to have summary and/or full body?
    );

    // @todo Now that taxonomy terms are fields, this does not work.
    //if (module_exists('taxonomy')) {
    //  $terms = taxonomy_node_get_terms($node);
    //  foreach ($terms as $term) {
    //    $extra[] = array('key' => 'category', 'attributes' => array('term' => $term->name));
    //  }
    //}

    $output .= theme('atom_feed_item', array('title' => $node->title, 'link' => $node->link, 'item' => $item, 'extra' => $extra));
  }

  // Merge some default values.
  $feed_info += array(
    'title' => variable_get('site_name', 'Drupal'),
    'subtitle' => variable_get('site_slogan', ''),
  );

  $output = theme('atom_feed', array('feed_info' => $feed_info, 'items' => $output));

  drupal_add_http_header('Content-Type', 'application/atom+xml; charset=utf-8');
  print $output;
}

/**
 * Overall themed output.
 */
function theme_atom_feed($variables) {
  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= '<feed xmlns="http://www.w3.org/2005/Atom">' . "\n";
  $output .= '  <title type="text">' . check_plain($variables['feed_info']['title']) . "</title>\n";
  if ($variables['feed_info']['subtitle']) {
    $output .= '  <subtitle type="text">' . check_plain($variables['feed_info']['subtitle']) . "</subtitle>\n";
  }
  $output .= '  <link rel="alternate" type="text/html" href="' . check_plain($variables['feed_info']['html_url']) . '" />' . "\n";
  $output .= '  <link rel="self" type="application/atom+xml" href="' . check_plain($variables['feed_info']['atom_url']) . '" />' . "\n";
  //$output .= '  <generator version="' . VERSION . '" uri="http://drupal.org">Drupal</generator>'."\n";
  $output .= '  <id>' . check_plain($variables['feed_info']['atom_url']) . "</id>\n";
  $output .= '  <updated>' . gmdate(DATE_ATOM, REQUEST_TIME) . "</updated>\n";
  $output .= $variables['items'];
  $output .= "</feed>\n";
  return $output;
}

/**
 * Theming for a single entry in the feed.
 */
function theme_atom_feed_item($variables) {
  $variables['item'] += array(
    'id' => $variables['link'],
    'summary' => '',
    'content' => '',
    'author' => '',
  );

  $output = "  <entry>\n";
  $output .= "    <id>" . check_plain($variables['item']['id']) . "</id>\n";
  $output .= "    <link rel=\"alternate\" type=\"text/html\" href=\"" . check_plain($variables['link']) . "\" />\n";
  $output .= "    <published>" . gmdate(DATE_ATOM, $variables['item']['published']) . "</published>\n";
  $output .= "    <updated>" . gmdate(DATE_ATOM, $variables['item']['updated']) . "</updated>\n";
  $output .= "    <title type=\"text\">" . check_plain($variables['title']) . "</title>\n";
  if ($variables['item']['author']) {
    $output .= "    <author><name>" . check_plain($variables['item']['author']) . "</name></author>\n";
  }
  if ($variables['item']['summary']) {
    $output .= "    <summary type=\"xhtml\"><div xmlns=\"http://www.w3.org/1999/xhtml\">" . $variables['item']['summary'] . "</div></summary>\n";
  }
  if ($variables['item']['content']) {
    $output .= "    <content type=\"xhtml\"><div xmlns=\"http://www.w3.org/1999/xhtml\">" . $variables['item']['content'] . "</div></content>\n";
  }
  if ($variables['extra']) {
    $output .= format_xml_elements($variables['extra']);
  }
  $output .= "  </entry>\n";
  return $output;
}

/**
 * @return string any additional namespaces used by contrib modules.
 */
function _atom_contrib_get_ns() {
  _atom_contrib_load();

  $ns_array = module_invoke_all('atom_ns');
  return count($ns_array) > 0 ? "\n      " . implode("\n      ", $ns_array) : '';
}
