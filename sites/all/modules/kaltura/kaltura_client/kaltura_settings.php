<?php
/*
 * @file
 * This file defines some data needed by the module
 *
 * changing data in this file might cause the module to stop working
 * or working not as expected.
 *
 * It is strongly advised not to change the settings in this file
 *
 */
define('ENTRY_TYPE_ROUGHCUT', 2);
define('WORK_WITH_KSHOW', FALSE);

  define('KalturaSettings_SERVER_URL', "http://www.kaltura.com");
  define('KalturaSettings_UICONF_ID', 600);
  define('KalturaSettings_BASE_WIDGET_ID', 600);
  define('KalturaSettings_ANONYMOUS_USER_ID', "Anonymous");
  define('KalturaSettings_CW_UICONF_ID', 4396241);
  define('KalturaSettings_CW_UICONF_ID_AUDIO', 4396051);
  define('KalturaSettings_CW_UICONF_ID_IMAGE', 4396061);
  define('KalturaSettings_CW_UICONF_ID_VIDEO', 4396041);

  define('KalturaSettings_DEFAULT_VIEWPLAYLIST_UICONF', 1292302);

  define('KalturaSettings_CW_UICONF_ID_SIMPLE', 1103);
//  define('KalturaSettings_CW_UICONF_ID_SIMPLE', 1002613);

 define('KalturaSettings_DEFAULT_EDITOR', 'Simple_Editor');

  define('KalturaSettings_SE_UICONF_ID', 603);
  define('KalturaSettings_SE_URL','kaltura/simple_editor');
  define('KalturaSettings_SE_WIDTH',890);
  define('KalturaSettings_SE_HEIGHT',546);
  define('KalturaSettings_SE_CUSTOM',1);

  define('KalturaSettings_AE_UICONF_ID',1000865);
  define('KalturaSettings_AE_URL','kaltura/advanced_editor');
  define('KalturaSettings_AE_WIDTH',830);
  define('KalturaSettings_AE_HEIGHT',690);
  define('KalturaSettings_AE_CUSTOM',2);

  define('KalturaSettings_CW_COMMENTS_UICONF_ID', 4396231);
  define('KalturaSettings_DRUPAL_STATS_URL', "http://corp.kaltura.com/stats/drupal/");
  define('KalturaSettings_DEFAULT_VIDEO_PLAYER_UICONF', 'dark');
  define('KalturaSettings_DEFAULT_AUDIO_PLAYER_UICONF', 'dark');
  define('KalturaSettings_DEFAULT_VIEWPLAYLIST_PLAYER_UICONF', 'dark');
  define('KalturaSettings_DEFAULT_RC_PLAYER_UICONF', 'dark');
  define('KalturaSettings_DEFAULT_COMMENT_PLAYER_UICONF', 'dark');
  define('CDN_HOST', 'http://cdnbakmi.kaltura.com/');

class KalturaSettings
{
  var $kdp_widgets = array(
      'audio' => array(
        'dark' => array( 'view_uiconf' => '605', 'remix_uiconf' => '604', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '607', 'remix_uiconf' => '606', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
      'viewplaylist' => array(
        'dark' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
      'video' => array(
        'dark' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
      'roughcut' => array(
        'dark' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
      'comment' => array(
        'dark' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'dark-player.jpg' ),
        'gray' => array( 'view_uiconf' => '48501', 'remix_uiconf' => '48501', 'preview_image' => 'gray-player.jpg' ),
        'white-blue' => array( 'view_uiconf' => '609', 'remix_uiconf' => '608', 'preview_image' => 'white-blue-player.jpg' ),
      ),
  );

  var $media_types_map = array(
    1 => 'Video',
    2 => 'Photo',
    5 => 'Audio',
    6 => 'Remix',
  );
}
