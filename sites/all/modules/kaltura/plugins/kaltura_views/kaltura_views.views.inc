<?php
// $Id: kaltura_views.views.inc,v 1.1.2.2 2010/08/28 10:21:00 xuriz Exp $

/*
 * Implementation of hook_views_data().
 *
 * Provides the views module with data about node_kaltura table
 * Enables the user to add fields to a view and filter/sort by some fields.
 * Some of the fields have only a field handler, which means they could be added to the "fields" list of the view
 * Others have a filter handler so the view could be filtered by them
 *
 * The structure of the return value applies the views2 api
 */
function kaltura_views_data() {

    //dsm(field_info_formatter_types());
    //$formatters = _field_view_formatter_options('field_kaltura');
    //dsm($formatters);
  $data['node_kaltura'] = array(
    'table' => array(
      'group' => 'kaltura',
      'title' => 'kaltura',
      'help' => t('table that holds kaltura entries'),
      'base' => array(
        'field' => 'kaltura_entryid',
        'title' => t('Kaltura entry'),
        'help' => t('All Kaltura media entries'),
      ),
    ),
    'kaltura_media_type' => array(
      'title' => t('kaltura entry media type'),
      'help' => t('see kaltura documentation for values'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'kaltura_views_handler_filter_media_type',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric'
      )
    ),
    'kaltura_plays' => array(
      'title' => t('kaltura item number of plays'),
      'help' => t('kaltura item number of plays'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_views' => array(
      'title' => t('kaltura item number of views'),
      'help' => t('see kaltura documentation for values'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_tags' => array(
      'title' => t('kaltura item tags'),
      'help' => t('kaltura item tags'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_duration' => array(
      'title' => t('kaltura item duration'),
      'help' => t('kaltura item duration'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_duration',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_total_rank' => array(
      'title' => t('kaltura item total rank'),
      'help' => t('total rank is rank multiplied by 1000'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_votes' => array(
      'title' => t('kaltura item number of votes'),
      'help' => t('see kaltura documentation for values'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_votes',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_rank' => array(
      'title' => t('kaltura item rank'),
      'help' => t('see kaltura documentation for values'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click_sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'kaltura_thumbnail_url' => array(
      'title' => t('kaltura item thumbnail'),
      'help' => t('kaltura item thumbnail'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_thumb',
      ),
    ),
    'kaltura_source_link' => array(
      'title' => t('kaltura item source link'),
      'help' => t('kaltura item source link'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),
    'kaltura_download_url' => array(
      'title' => t('kaltura item download link'),
      'help' => t('kaltura item download link'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_url',
      ),
    ),
    'kaltura_media_date' => array(
      'title' => t('kaltura image item media date (EXIF data)'),
      'help' => t('kaltura image item media date (EXIF data)'),
      'field' => array(
        'handler' => 'views_handler_field_date',
      ),
    ),
    'kaltura_created_date' => array(
      'title' => t('kaltura media created date'),
      'help' => t('kaltura media created date'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
    ),
    'kaltura_entryid' => array(
      'title' => t('kaltura media'),
      'help' => t('Kaltura entry'),
      'field' => array(
        'handler' => 'kaltura_views_handler_field_kaltura_media',
      ),
    ),
    'kaltura_text_entryid' => array(
      'title' => t('kaltura entry as text'),
      'help' => t('Kaltura entry as as text'),
      'field' => array(
        'field' => 'kaltura_entryid',
        'handler' => 'views_handler_field',
      ),
    ),
    'kaltura_edit_metadata' => array(
      'title' => t('Kaltura edit metadata'),
      'help' => t('Kaltura edit metadata link'),
      'field' => array(
        'field' => 'kaltura_entryid',
        'handler' => 'kaltura_views_handler_field_kaltura_edit_meta',
      ),
    ),
    'kaltura_text' => array(
      'title' => t('Kaltura text'),
      'help' => t('All entry free text'),
      'filter' => array(
        'field' => 'kaltura_title',
        'handler' => 'kaltura_views_handler_filter_kaltura_text'
      ),
    ),
    'kaltura_title' => array(
      'title' => t('Kaltura title'),
      'help' => t('Kaltura entry title'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string'
      ),
    ),
    'kaltura_description' => array(
      'title' => t('Kaltura description'),
      'help' => t('Kaltura entry description'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string'
      ),
    )
  );
  return $data;
}

