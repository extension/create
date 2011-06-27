<?php
// $Id: kaltura_views.views_default.inc,v 1.1.2.1 2010/08/09 04:33:33 univate Exp $

/**
 * Implementation of hook_views_default_views().
 *
 * We provide the following default views, which the user can override
 * Recently uploaded videos
 * Recently uploaded photos
 * Most viewed items
 * Highest rated items
 * My Kaltura Items - shows the items of the current user
 */
function kaltura_views_views_default_views() {
  $views = array();
/*
* View 'kaltura_list_entries'
*/
$view = new view;
$view->name = 'kaltura_list_entries';
$view->description = '';
$view->tag = '';
$view->base_table = 'node_kaltura';
$view->human_name = 'kaltura list entries';
$view->core = 7;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'mini';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'kaltura_thumbnail_url' => 'kaltura_thumbnail_url',
  'kaltura_title' => 'kaltura_title',
  'kaltura_description' => 'kaltura_description',
  'kaltura_tags' => 'kaltura_tags',
  'kaltura_created_date' => 'kaltura_created_date',
  'kaltura_edit_metadata' => 'kaltura_edit_metadata',
);
$handler->display->display_options['style_options']['default'] = 'kaltura_created_date';
$handler->display->display_options['style_options']['info'] = array(
  'kaltura_thumbnail_url' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'kaltura_description' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_tags' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_created_date' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
  ),
  'kaltura_edit_metadata' => array(
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: kaltura: kaltura item thumbnail */
$handler->display->display_options['fields']['kaltura_thumbnail_url']['id'] = 'kaltura_thumbnail_url';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['field'] = 'kaltura_thumbnail_url';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['label'] = 'Thumbnail';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['thumbnail_width'] = '50';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['thumbnail_height'] = '50';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['with_preview'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['preview_width'] = '300';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['preview_height'] = '240';
/* Field: kaltura: Kaltura title */
$handler->display->display_options['fields']['kaltura_title']['id'] = 'kaltura_title';
$handler->display->display_options['fields']['kaltura_title']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_title']['field'] = 'kaltura_title';
$handler->display->display_options['fields']['kaltura_title']['label'] = 'Title';
$handler->display->display_options['fields']['kaltura_title']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['text'] = '<div title="[kaltura_title]">[kaltura_title]</div>';
$handler->display->display_options['fields']['kaltura_title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_title']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_title']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_title']['empty_zero'] = 0;
/* Field: kaltura: Kaltura description */
$handler->display->display_options['fields']['kaltura_description']['id'] = 'kaltura_description';
$handler->display->display_options['fields']['kaltura_description']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_description']['field'] = 'kaltura_description';
$handler->display->display_options['fields']['kaltura_description']['label'] = 'Description';
$handler->display->display_options['fields']['kaltura_description']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['text'] = '<div title="[kaltura_description]">[kaltura_description]<div>';
$handler->display->display_options['fields']['kaltura_description']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['trim'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['max_length'] = '200';
$handler->display->display_options['fields']['kaltura_description']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_description']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_description']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_description']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_description']['empty_zero'] = 0;
/* Field: kaltura: kaltura item tags */
$handler->display->display_options['fields']['kaltura_tags']['id'] = 'kaltura_tags';
$handler->display->display_options['fields']['kaltura_tags']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_tags']['field'] = 'kaltura_tags';
$handler->display->display_options['fields']['kaltura_tags']['label'] = 'Tags';
$handler->display->display_options['fields']['kaltura_tags']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['text'] = '<div title="[kaltura_tags]">[kaltura_tags]</div>';
$handler->display->display_options['fields']['kaltura_tags']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['empty_zero'] = 0;
/* Field: kaltura: kaltura media created date */
$handler->display->display_options['fields']['kaltura_created_date']['id'] = 'kaltura_created_date';
$handler->display->display_options['fields']['kaltura_created_date']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_created_date']['field'] = 'kaltura_created_date';
$handler->display->display_options['fields']['kaltura_created_date']['label'] = 'Created date';
$handler->display->display_options['fields']['kaltura_created_date']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['text'] = '<ul>
<li>Title: [kaltura_title]</li>
<li>Description: [kaltura_description]</li>
<li>Tags: [kaltura_tags]</li>
<li>Created Date: [kaltura_created_date]</li>
</ul>';
$handler->display->display_options['fields']['kaltura_created_date']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['element_type'] = 'div';
$handler->display->display_options['fields']['kaltura_created_date']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['element_wrapper_type'] = 'div';
$handler->display->display_options['fields']['kaltura_created_date']['element_wrapper_class'] = 'details';
$handler->display->display_options['fields']['kaltura_created_date']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['date_format'] = 'long';
/* Field: kaltura: Kaltura edit metadata */
$handler->display->display_options['fields']['kaltura_edit_metadata']['id'] = 'kaltura_edit_metadata';
$handler->display->display_options['fields']['kaltura_edit_metadata']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_edit_metadata']['field'] = 'kaltura_edit_metadata';
$handler->display->display_options['fields']['kaltura_edit_metadata']['label'] = 'Action';
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_edit_metadata']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_edit_metadata']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_edit_metadata']['empty_zero'] = 0;
/* Sort criterion: kaltura: Kaltura title */
$handler->display->display_options['sorts']['kaltura_title']['id'] = 'kaltura_title';
$handler->display->display_options['sorts']['kaltura_title']['table'] = 'node_kaltura';
$handler->display->display_options['sorts']['kaltura_title']['field'] = 'kaltura_title';
$handler->display->display_options['sorts']['kaltura_title']['expose']['label'] = 'title';
/* Sort criterion: kaltura: kaltura media created date */
$handler->display->display_options['sorts']['kaltura_created_date']['id'] = 'kaltura_created_date';
$handler->display->display_options['sorts']['kaltura_created_date']['table'] = 'node_kaltura';
$handler->display->display_options['sorts']['kaltura_created_date']['field'] = 'kaltura_created_date';
$handler->display->display_options['sorts']['kaltura_created_date']['expose']['label'] = 'created date';
$handler->display->display_options['sorts']['kaltura_created_date']['granularity'] = 'minute';
/* Filter criterion: kaltura: Kaltura text */
$handler->display->display_options['filters']['kaltura_text']['id'] = 'kaltura_text';
$handler->display->display_options['filters']['kaltura_text']['table'] = 'node_kaltura';
$handler->display->display_options['filters']['kaltura_text']['field'] = 'kaltura_text';
$handler->display->display_options['filters']['kaltura_text']['exposed'] = TRUE;
$handler->display->display_options['filters']['kaltura_text']['expose']['operator_id'] = '';
$handler->display->display_options['filters']['kaltura_text']['expose']['use_operator'] = FALSE;
$handler->display->display_options['filters']['kaltura_text']['expose']['operator'] = 'kaltura_text_op';
$handler->display->display_options['filters']['kaltura_text']['expose']['identifier'] = 'kaltura_text';
/* Filter criterion: kaltura: kaltura entry media type */
$handler->display->display_options['filters']['kaltura_media_type']['id'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['table'] = 'node_kaltura';
$handler->display->display_options['filters']['kaltura_media_type']['field'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['value'] = array(
  1 => '1',
  2 => '2',
  5 => '5',
);
$handler->display->display_options['filters']['kaltura_media_type']['exposed'] = TRUE;
$handler->display->display_options['filters']['kaltura_media_type']['expose']['operator_id'] = 'kaltura_media_type_op';
$handler->display->display_options['filters']['kaltura_media_type']['expose']['operator'] = 'kaltura_media_type_op';
$handler->display->display_options['filters']['kaltura_media_type']['expose']['identifier'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['expose']['multiple'] = 1;
$handler->display->display_options['filters']['kaltura_media_type']['expose']['reduce'] = 0;
$translatables['kaltura_list_entries'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Thumbnail'),
  t('Title'),
  t('<div title="[kaltura_title]">[kaltura_title]</div>'),
  t('Description'),
  t('<div title="[kaltura_description]">[kaltura_description]<div>'),
  t('Tags'),
  t('<div title="[kaltura_tags]">[kaltura_tags]</div>'),
  t('Created date'),
  t('<ul>
<li>Title: [kaltura_title]</li>
<li>Description: [kaltura_description]</li>
<li>Tags: [kaltura_tags]</li>
<li>Created Date: [kaltura_created_date]</li>
</ul>'),
  t('Action'),
  t('title'),
  t('created date'),
);

  $views[$view->name] = $view;
/*******************************************************/

$view = new view;
$view->name = 'kaltura_existing';
$view->description = '';
$view->tag = '';
$view->base_table = 'node_kaltura';
$view->human_name = 'kaltura_existing';
$view->core = 7;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'mini';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'kaltura_thumbnail_url' => 'kaltura_thumbnail_url',
  'kaltura_title' => 'kaltura_title',
  'kaltura_description' => 'kaltura_description',
  'kaltura_tags' => 'kaltura_tags',
  'kaltura_created_date' => 'kaltura_created_date',
  'kaltura_edit_metadata' => 'kaltura_edit_metadata',
);
$handler->display->display_options['style_options']['default'] = 'kaltura_created_date';
$handler->display->display_options['style_options']['info'] = array(
  'kaltura_thumbnail_url' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'kaltura_description' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_tags' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_created_date' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
  ),
  'kaltura_edit_metadata' => array(
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: kaltura: kaltura item thumbnail */
$handler->display->display_options['fields']['kaltura_thumbnail_url']['id'] = 'kaltura_thumbnail_url';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['field'] = 'kaltura_thumbnail_url';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['label'] = 'Thumbnail';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['thumbnail_width'] = '50';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['thumbnail_height'] = '50';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['with_preview'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['preview_width'] = '300';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['preview_height'] = '240';
/* Field: kaltura: Kaltura title */
$handler->display->display_options['fields']['kaltura_title']['id'] = 'kaltura_title';
$handler->display->display_options['fields']['kaltura_title']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_title']['field'] = 'kaltura_title';
$handler->display->display_options['fields']['kaltura_title']['label'] = 'Title';
$handler->display->display_options['fields']['kaltura_title']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['text'] = '<div title="[kaltura_title]">[kaltura_title]</div>';
$handler->display->display_options['fields']['kaltura_title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_title']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_title']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_title']['empty_zero'] = 0;
/* Field: kaltura: Kaltura description */
$handler->display->display_options['fields']['kaltura_description']['id'] = 'kaltura_description';
$handler->display->display_options['fields']['kaltura_description']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_description']['field'] = 'kaltura_description';
$handler->display->display_options['fields']['kaltura_description']['label'] = 'Description';
$handler->display->display_options['fields']['kaltura_description']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['text'] = '<div title="[kaltura_description]">[kaltura_description]</div>';
$handler->display->display_options['fields']['kaltura_description']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['trim'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['max_length'] = '200';
$handler->display->display_options['fields']['kaltura_description']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_description']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_description']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_description']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_description']['empty_zero'] = 0;
/* Field: kaltura: kaltura item tags */
$handler->display->display_options['fields']['kaltura_tags']['id'] = 'kaltura_tags';
$handler->display->display_options['fields']['kaltura_tags']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_tags']['field'] = 'kaltura_tags';
$handler->display->display_options['fields']['kaltura_tags']['label'] = 'Tags';
$handler->display->display_options['fields']['kaltura_tags']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['text'] = '<div title="[kaltura_tags]">[kaltura_tags]</div>';
$handler->display->display_options['fields']['kaltura_tags']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['empty_zero'] = 0;
/* Field: kaltura: kaltura media created date */
$handler->display->display_options['fields']['kaltura_created_date']['id'] = 'kaltura_created_date';
$handler->display->display_options['fields']['kaltura_created_date']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_created_date']['field'] = 'kaltura_created_date';
$handler->display->display_options['fields']['kaltura_created_date']['label'] = 'Created date';
$handler->display->display_options['fields']['kaltura_created_date']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['text'] = '<ul>
<li>Title: [kaltura_title]</li>
<li>Description: [kaltura_description]</li>
<li>Tags: [kaltura_tags]</li>
<li>Created Date: [kaltura_created_date]</li>
</ul>';
$handler->display->display_options['fields']['kaltura_created_date']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['element_type'] = 'div';
$handler->display->display_options['fields']['kaltura_created_date']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['element_wrapper_type'] = 'div';
$handler->display->display_options['fields']['kaltura_created_date']['element_wrapper_class'] = 'details';
$handler->display->display_options['fields']['kaltura_created_date']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['date_format'] = 'long';
/* Field: kaltura: kaltura entry media type */
$handler->display->display_options['fields']['kaltura_media_type']['id'] = 'kaltura_media_type';
$handler->display->display_options['fields']['kaltura_media_type']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_media_type']['field'] = 'kaltura_media_type';
$handler->display->display_options['fields']['kaltura_media_type']['label'] = '';
$handler->display->display_options['fields']['kaltura_media_type']['exclude'] = TRUE;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['kaltura_media_type']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_media_type']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_media_type']['separator'] = '';
$handler->display->display_options['fields']['kaltura_media_type']['format_plural'] = 0;
/* Field: kaltura: kaltura entry as text */
$handler->display->display_options['fields']['kaltura_text_entryid']['id'] = 'kaltura_text_entryid';
$handler->display->display_options['fields']['kaltura_text_entryid']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_text_entryid']['field'] = 'kaltura_text_entryid';
$handler->display->display_options['fields']['kaltura_text_entryid']['label'] = '';
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['text'] = '<a class="kentry_add" id="[kaltura_text_entryid]" rel="[kaltura_media_type]" name="FnAmE" title="Insert">Insert</a>';
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_text_entryid']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_text_entryid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['kaltura_text_entryid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_text_entryid']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_text_entryid']['empty_zero'] = 0;
/* Sort criterion: kaltura: Kaltura title */
$handler->display->display_options['sorts']['kaltura_title']['id'] = 'kaltura_title';
$handler->display->display_options['sorts']['kaltura_title']['table'] = 'node_kaltura';
$handler->display->display_options['sorts']['kaltura_title']['field'] = 'kaltura_title';
$handler->display->display_options['sorts']['kaltura_title']['expose']['label'] = 'title';
/* Sort criterion: kaltura: kaltura media created date */
$handler->display->display_options['sorts']['kaltura_created_date']['id'] = 'kaltura_created_date';
$handler->display->display_options['sorts']['kaltura_created_date']['table'] = 'node_kaltura';
$handler->display->display_options['sorts']['kaltura_created_date']['field'] = 'kaltura_created_date';
$handler->display->display_options['sorts']['kaltura_created_date']['expose']['label'] = 'created date';
$handler->display->display_options['sorts']['kaltura_created_date']['granularity'] = 'minute';
/* Contextual filter: kaltura: kaltura entry media type */
$handler->display->display_options['arguments']['kaltura_media_type']['id'] = 'kaltura_media_type';
$handler->display->display_options['arguments']['kaltura_media_type']['table'] = 'node_kaltura';
$handler->display->display_options['arguments']['kaltura_media_type']['field'] = 'kaltura_media_type';
$handler->display->display_options['arguments']['kaltura_media_type']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['kaltura_media_type']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['kaltura_media_type']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['kaltura_media_type']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['kaltura_media_type']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['kaltura_media_type']['specify_validation'] = 1;
$handler->display->display_options['arguments']['kaltura_media_type']['validate']['type'] = 'numeric';
$handler->display->display_options['arguments']['kaltura_media_type']['break_phrase'] = 0;
$handler->display->display_options['arguments']['kaltura_media_type']['not'] = 0;
/* Filter criterion: kaltura: Kaltura text */
$handler->display->display_options['filters']['kaltura_text']['id'] = 'kaltura_text';
$handler->display->display_options['filters']['kaltura_text']['table'] = 'node_kaltura';
$handler->display->display_options['filters']['kaltura_text']['field'] = 'kaltura_text';
$handler->display->display_options['filters']['kaltura_text']['exposed'] = TRUE;
$handler->display->display_options['filters']['kaltura_text']['expose']['operator_id'] = '';
$handler->display->display_options['filters']['kaltura_text']['expose']['label'] = 'search';
$handler->display->display_options['filters']['kaltura_text']['expose']['use_operator'] = FALSE;
$handler->display->display_options['filters']['kaltura_text']['expose']['operator'] = 'kaltura_text_op';
$handler->display->display_options['filters']['kaltura_text']['expose']['identifier'] = 'kaltura_text';
/* Filter criterion: kaltura: kaltura entry media type */
$handler->display->display_options['filters']['kaltura_media_type']['id'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['table'] = 'node_kaltura';
$handler->display->display_options['filters']['kaltura_media_type']['field'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['value'] = array(
  1 => '1',
  2 => '2',
  5 => '5',
);
$handler->display->display_options['filters']['kaltura_media_type']['exposed'] = TRUE;
$handler->display->display_options['filters']['kaltura_media_type']['expose']['operator_id'] = 'kaltura_media_type_op';
$handler->display->display_options['filters']['kaltura_media_type']['expose']['operator'] = 'kaltura_media_type_op';
$handler->display->display_options['filters']['kaltura_media_type']['expose']['identifier'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['expose']['multiple'] = 1;
$handler->display->display_options['filters']['kaltura_media_type']['expose']['reduce'] = 0;
$translatables['kaltura_existing'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Thumbnail'),
  t('Title'),
  t('<div title="[kaltura_title]">[kaltura_title]</div>'),
  t('Description'),
  t('<div title="[kaltura_description]">[kaltura_description]</div>'),
  t('Tags'),
  t('<div title="[kaltura_tags]">[kaltura_tags]</div>'),
  t('Created date'),
  t('<ul>
<li>Title: [kaltura_title]</li>
<li>Description: [kaltura_description]</li>
<li>Tags: [kaltura_tags]</li>
<li>Created Date: [kaltura_created_date]</li>
</ul>'),
  t('.'),
  t('<a class="kentry_add" id="[kaltura_text_entryid]" rel="[kaltura_media_type]" name="FnAmE" title="Insert">Insert</a>'),
  t('title'),
  t('created date'),
  t('All'),
  t('search'),
);

  $views[$view->name] = $view;
/********************** default views *****************/

/*************** most popular videos  *****************/
$view = new view;
$view->name = 'kaltura_most_popular_videos';
$view->description = 'Displays the thumbnails of the videos that were most played. Placing the mouse over the thumbnail will create a quick preview of';
$view->tag = '';
$view->base_table = 'node_kaltura';
$view->human_name = 'kaltura most popular videos';
$view->core = 7;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'mini';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'kaltura_thumbnail_url' => 'kaltura_thumbnail_url',
  'kaltura_title' => 'kaltura_title',
  'kaltura_description' => 'kaltura_description',
  'kaltura_tags' => 'kaltura_tags',
  'kaltura_created_date' => 'kaltura_created_date',
  'kaltura_plays' => 'kaltura_plays',
);
$handler->display->display_options['style_options']['default'] = 'kaltura_plays';
$handler->display->display_options['style_options']['info'] = array(
  'kaltura_thumbnail_url' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'kaltura_description' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_tags' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_created_date' => array(
    'sortable' => 0,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
  ),
  'kaltura_plays' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: kaltura: kaltura item thumbnail */
$handler->display->display_options['fields']['kaltura_thumbnail_url']['id'] = 'kaltura_thumbnail_url';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['field'] = 'kaltura_thumbnail_url';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['label'] = 'Thumbnail';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['thumbnail_width'] = '50';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['thumbnail_height'] = '50';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['with_preview'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['preview_width'] = '300';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['preview_height'] = '240';
/* Field: kaltura: Kaltura title */
$handler->display->display_options['fields']['kaltura_title']['id'] = 'kaltura_title';
$handler->display->display_options['fields']['kaltura_title']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_title']['field'] = 'kaltura_title';
$handler->display->display_options['fields']['kaltura_title']['label'] = 'Title';
$handler->display->display_options['fields']['kaltura_title']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['text'] = '<div title="[kaltura_title]">[kaltura_title]</div>';
$handler->display->display_options['fields']['kaltura_title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_title']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_title']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_title']['empty_zero'] = 0;
/* Field: kaltura: Kaltura description */
$handler->display->display_options['fields']['kaltura_description']['id'] = 'kaltura_description';
$handler->display->display_options['fields']['kaltura_description']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_description']['field'] = 'kaltura_description';
$handler->display->display_options['fields']['kaltura_description']['label'] = 'Description';
$handler->display->display_options['fields']['kaltura_description']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['text'] = '<div title="[kaltura_description]">[kaltura_description]<div>';
$handler->display->display_options['fields']['kaltura_description']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['trim'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['max_length'] = '200';
$handler->display->display_options['fields']['kaltura_description']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_description']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_description']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_description']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_description']['empty_zero'] = 0;
/* Field: kaltura: kaltura item tags */
$handler->display->display_options['fields']['kaltura_tags']['id'] = 'kaltura_tags';
$handler->display->display_options['fields']['kaltura_tags']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_tags']['field'] = 'kaltura_tags';
$handler->display->display_options['fields']['kaltura_tags']['label'] = 'Tags';
$handler->display->display_options['fields']['kaltura_tags']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['text'] = '<div title="[kaltura_tags]">[kaltura_tags]</div>';
$handler->display->display_options['fields']['kaltura_tags']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['empty_zero'] = 0;
/* Field: kaltura: kaltura media created date */
$handler->display->display_options['fields']['kaltura_created_date']['id'] = 'kaltura_created_date';
$handler->display->display_options['fields']['kaltura_created_date']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_created_date']['field'] = 'kaltura_created_date';
$handler->display->display_options['fields']['kaltura_created_date']['label'] = 'Created date';
$handler->display->display_options['fields']['kaltura_created_date']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['text'] = '<ul>
<li>Title: [kaltura_title]</li>
<li>Description: [kaltura_description]</li>
<li>Tags: [kaltura_tags]</li>
<li>Created Date: [kaltura_created_date]</li>
</ul>';
$handler->display->display_options['fields']['kaltura_created_date']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['element_type'] = 'div';
$handler->display->display_options['fields']['kaltura_created_date']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['element_wrapper_type'] = 'div';
$handler->display->display_options['fields']['kaltura_created_date']['element_wrapper_class'] = 'details';
$handler->display->display_options['fields']['kaltura_created_date']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['date_format'] = 'long';
/* Field: kaltura: kaltura item number of plays */
$handler->display->display_options['fields']['kaltura_plays']['id'] = 'kaltura_plays';
$handler->display->display_options['fields']['kaltura_plays']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_plays']['field'] = 'kaltura_plays';
$handler->display->display_options['fields']['kaltura_plays']['label'] = 'number of plays';
$handler->display->display_options['fields']['kaltura_plays']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_plays']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_plays']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_plays']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_plays']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['format_plural'] = 0;
/* Sort criterion: kaltura: kaltura item number of plays */
$handler->display->display_options['sorts']['kaltura_plays']['id'] = 'kaltura_plays';
$handler->display->display_options['sorts']['kaltura_plays']['table'] = 'node_kaltura';
$handler->display->display_options['sorts']['kaltura_plays']['field'] = 'kaltura_plays';
$handler->display->display_options['sorts']['kaltura_plays']['order'] = 'DESC';
$handler->display->display_options['sorts']['kaltura_plays']['expose']['label'] = 'kaltura item number of plays';
/* Filter criterion: kaltura: kaltura entry media type */
$handler->display->display_options['filters']['kaltura_media_type']['id'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['table'] = 'node_kaltura';
$handler->display->display_options['filters']['kaltura_media_type']['field'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['value'] = array(
  1 => '1',
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'kaltura-most-popular-videos';
$translatables['kaltura_most_popular_videos'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Thumbnail'),
  t('Title'),
  t('<div title="[kaltura_title]">[kaltura_title]</div>'),
  t('Description'),
  t('<div title="[kaltura_description]">[kaltura_description]<div>'),
  t('Tags'),
  t('<div title="[kaltura_tags]">[kaltura_tags]</div>'),
  t('Created date'),
  t('<ul>
<li>Title: [kaltura_title]</li>
<li>Description: [kaltura_description]</li>
<li>Tags: [kaltura_tags]</li>
<li>Created Date: [kaltura_created_date]</li>
</ul>'),
  t('number of plays'),
  t('.'),
  t(','),
  t('kaltura item number of plays'),
  t('Page'),
);

  $views[$view->name] = $view;

/****************** most poular audio ****************/
$view = new view;
$view->name = 'kaltura_most_popular_audio';
$view->description = '';
$view->tag = 'kaltura';
$view->base_table = 'node_kaltura';
$view->human_name = 'kaltura most popular audio';
$view->core = 7;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'mini';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'kaltura_thumbnail_url' => 'kaltura_thumbnail_url',
  'kaltura_title' => 'kaltura_title',
  'kaltura_description' => 'kaltura_description',
  'kaltura_tags' => 'kaltura_tags',
  'kaltura_created_date' => 'kaltura_created_date',
  'kaltura_plays' => 'kaltura_plays',
);
$handler->display->display_options['style_options']['default'] = 'kaltura_plays';
$handler->display->display_options['style_options']['info'] = array(
  'kaltura_thumbnail_url' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'kaltura_description' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_tags' => array(
    'align' => '',
    'separator' => '',
  ),
  'kaltura_created_date' => array(
    'sortable' => 0,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
  ),
  'kaltura_plays' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: kaltura: kaltura item thumbnail */
$handler->display->display_options['fields']['kaltura_thumbnail_url']['id'] = 'kaltura_thumbnail_url';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['field'] = 'kaltura_thumbnail_url';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['label'] = 'Thumbnail';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['thumbnail_width'] = '50';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['thumbnail_height'] = '50';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['with_preview'] = 1;
$handler->display->display_options['fields']['kaltura_thumbnail_url']['preview_width'] = '300';
$handler->display->display_options['fields']['kaltura_thumbnail_url']['preview_height'] = '240';
/* Field: kaltura: Kaltura title */
$handler->display->display_options['fields']['kaltura_title']['id'] = 'kaltura_title';
$handler->display->display_options['fields']['kaltura_title']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_title']['field'] = 'kaltura_title';
$handler->display->display_options['fields']['kaltura_title']['label'] = 'Title';
$handler->display->display_options['fields']['kaltura_title']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['text'] = '<div title="[kaltura_title]">[kaltura_title]</div>';
$handler->display->display_options['fields']['kaltura_title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_title']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_title']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_title']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_title']['empty_zero'] = 0;
/* Field: kaltura: Kaltura description */
$handler->display->display_options['fields']['kaltura_description']['id'] = 'kaltura_description';
$handler->display->display_options['fields']['kaltura_description']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_description']['field'] = 'kaltura_description';
$handler->display->display_options['fields']['kaltura_description']['label'] = 'Description';
$handler->display->display_options['fields']['kaltura_description']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['text'] = '<div title="[kaltura_description]">[kaltura_description]<div>';
$handler->display->display_options['fields']['kaltura_description']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['trim'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['max_length'] = '200';
$handler->display->display_options['fields']['kaltura_description']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_description']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_description']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_description']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_description']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_description']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_description']['empty_zero'] = 0;
/* Field: kaltura: kaltura item tags */
$handler->display->display_options['fields']['kaltura_tags']['id'] = 'kaltura_tags';
$handler->display->display_options['fields']['kaltura_tags']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_tags']['field'] = 'kaltura_tags';
$handler->display->display_options['fields']['kaltura_tags']['label'] = 'Tags';
$handler->display->display_options['fields']['kaltura_tags']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['text'] = '<div title="[kaltura_tags]">[kaltura_tags]</div>';
$handler->display->display_options['fields']['kaltura_tags']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_tags']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_tags']['empty_zero'] = 0;
/* Field: kaltura: kaltura media created date */
$handler->display->display_options['fields']['kaltura_created_date']['id'] = 'kaltura_created_date';
$handler->display->display_options['fields']['kaltura_created_date']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_created_date']['field'] = 'kaltura_created_date';
$handler->display->display_options['fields']['kaltura_created_date']['label'] = 'Created date';
$handler->display->display_options['fields']['kaltura_created_date']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['text'] = '<ul>
<li>Title: [kaltura_title]</li>
<li>Description: [kaltura_description]</li>
<li>Tags: [kaltura_tags]</li>
<li>Created Date: [kaltura_created_date]</li>
</ul>';
$handler->display->display_options['fields']['kaltura_created_date']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['element_type'] = 'div';
$handler->display->display_options['fields']['kaltura_created_date']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['element_wrapper_type'] = 'div';
$handler->display->display_options['fields']['kaltura_created_date']['element_wrapper_class'] = 'details';
$handler->display->display_options['fields']['kaltura_created_date']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_created_date']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_created_date']['date_format'] = 'long';
/* Field: kaltura: kaltura item number of plays */
$handler->display->display_options['fields']['kaltura_plays']['id'] = 'kaltura_plays';
$handler->display->display_options['fields']['kaltura_plays']['table'] = 'node_kaltura';
$handler->display->display_options['fields']['kaltura_plays']['field'] = 'kaltura_plays';
$handler->display->display_options['fields']['kaltura_plays']['label'] = 'number of plays';
$handler->display->display_options['fields']['kaltura_plays']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['external'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['trim'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['kaltura_plays']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['kaltura_plays']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['alter']['html'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['element_label_colon'] = 1;
$handler->display->display_options['fields']['kaltura_plays']['element_default_classes'] = 1;
$handler->display->display_options['fields']['kaltura_plays']['hide_empty'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['empty_zero'] = 0;
$handler->display->display_options['fields']['kaltura_plays']['format_plural'] = 0;
/* Sort criterion: kaltura: kaltura item number of plays */
$handler->display->display_options['sorts']['kaltura_plays']['id'] = 'kaltura_plays';
$handler->display->display_options['sorts']['kaltura_plays']['table'] = 'node_kaltura';
$handler->display->display_options['sorts']['kaltura_plays']['field'] = 'kaltura_plays';
$handler->display->display_options['sorts']['kaltura_plays']['order'] = 'DESC';
$handler->display->display_options['sorts']['kaltura_plays']['expose']['label'] = 'kaltura item number of plays';
/* Filter criterion: kaltura: kaltura entry media type */
$handler->display->display_options['filters']['kaltura_media_type']['id'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['table'] = 'node_kaltura';
$handler->display->display_options['filters']['kaltura_media_type']['field'] = 'kaltura_media_type';
$handler->display->display_options['filters']['kaltura_media_type']['value'] = array(
  5 => '5',
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'kaltura-most-popular-audio';
$translatables['kaltura_most_popular_audio'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Thumbnail'),
  t('Title'),
  t('<div title="[kaltura_title]">[kaltura_title]</div>'),
  t('Description'),
  t('<div title="[kaltura_description]">[kaltura_description]<div>'),
  t('Tags'),
  t('<div title="[kaltura_tags]">[kaltura_tags]</div>'),
  t('Created date'),
  t('<ul>
<li>Title: [kaltura_title]</li>
<li>Description: [kaltura_description]</li>
<li>Tags: [kaltura_tags]</li>
<li>Created Date: [kaltura_created_date]</li>
</ul>'),
  t('number of plays'),
  t('.'),
  t(','),
  t('kaltura item number of plays'),
  t('Page'),
);

  $views[$view->name] = $view;

  return $views;
}
