<?php
// $Id: file_styles.styles.inc,v 1.1.2.3 2010/10/23 16:07:15 aaron Exp $

/**
 * @file includes/styles/file_styles.styles.inc
 * Implementations of various Styles hooks.
 */

/**
 * Implementation of Styles module hook_styles_default_containers().
 */
function file_styles_styles_default_containers() {
  $containers = array();
  foreach (array('image', 'audio', 'video', 'default') as $type) {
    $containers[$type] = array(
      'class' => 'FileStyles',
    );
  }
  return array(
    'file' => array(
      'admin' => array(
        'path' => 'admin/config/media/file-styles',
        'access arguments' => array('administer file styles'),
      ),
      'filter callback' => 'file_styles_styles_filter',
      'help' => t('Each of the following containers defines a set of styles that will be applied when a file is of the specified type. For instance, if a file field allows images and videos, a specific style might be defined for \'Thumbnail\', that will display a cropped image when a JPEG is given, or a thumbnail linking to a shadowboxed video when an MPEG is stored.'),
      'containers' => $containers,
    ),
  );
}

/**
 * Implementation of Styles module hook_styles_default_styles().
 */
function file_styles_styles_default_styles() {
  return array(
    'file' => array(
      'styles' => array(
        'square_thumbnail' => array(
          'label' => 'Square thumbnail',
          'description' => 'A 120x120 square thumbnail for browsing media by an editor.',
        ),
        'thumbnail' => array(
          'label' => 'Thumbnail',
          'description' => 'Small thumbnails representing the media.',
        ),
        'large' => array(
          'label' => 'Large',
          'description' => 'A large format of the media.',
        ),
        'medium' => array(
          'label' => 'Medium',
          'description' => 'A medium format of the media.',
        ),
        'original' => array(
          'label' => 'Original',
          'description' => 'The original format of the media.',
        ),
      ),
    ),
  );
}

/**
 * Implementation of Styles module hook_styles_default_presets().
 */
function file_styles_styles_default_presets() {
  $containers = array();

  // @TODO: The real deal for video/audio/etc.
  foreach (array('image', 'audio', 'video', 'default') as $type) {
    $containers[$type] = array(
      'default preset' => 'unlinked_thumbnail',
      'styles' => array(
        'thumbnail' => array(
          'default preset' => 'linked_thumbnail',
        ),
        'square_thumbnail' => array(
          'default preset' => 'linked_square_thumbnail',
        ),
        'large' => array(
          'default preset' => 'large',
        ),
      ),
      'presets' => array(
        'unlinked_thumbnail' => array(
          array(
            'name' => 'thumbnail',
            'settings' => array(),
          ),
        ),
        'linked_thumbnail' => array(
          array(
            'name' => 'link_to_media',
            'settings' => array(),
          ),
          array(
            'name' => 'thumbnail',
            'settings' => array(),
          ),
        ),
        'linked_square_thumbnail' => array(
          array(
            'name' => 'link_to_media',
            'settings' => array(),
          ),
          array(
            'name' => 'resize',
            'settings' => array(
              'width' => 120,
              'height' => 120,
            ),
          ),
          array(
            'name' => 'thumbnail',
            'settings' => array(),
          ),
        ),
        'large' => array(
          array(
            'name' => 'image_style',
            'settings' => array(
              'image_style' => 'large',
            ),
          ),
          array(
            'name' => 'thumbnail',
            'settings' => array(),
          ),
        ),
      ),
    );
  }
  return array(
    'file' => array(
      'containers' => $containers,
    ),
  );
}