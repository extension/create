<?php
// $Id:$

/**
 * @file media_flickr/includes/media_flickr.styles.inc
 * Styles definitions for Media: Flickr.
 */

/**
 * Implementation of Styles module hook_styles_default_containers().
 */
function media_flickr_styles_default_containers() {
	// We append Flickr to the file containers.
	return array(
	  'file' => array(
	    'containers' => array(
	      'media_flickr' => array(
	        'class' => 'MediaFlickrStyles',
	        'name' => 'media_flickr',
	        'label' => t('Flickr'),
	        'preview' => 'media_flickr_preview_style',
	      ),
	    ),
	  ),
	);
}

/**
 * Implementation of Styles module hook_styles_default_presets().
 */
function media_flickr_styles_default_presets() {
	return array(
    'file' => array(
      'containers' => array(
        'media_flickr' => array(
          'default preset' => 'linked_thumbnail',
          'styles' => array(
            'thumbnail' => array(
              'default preset' => 'linked_thumbnail',
            ),
            'square_thumbnail' => array(
              'default preset' => 'linked_square_thumbnail',
            ),
            'medium' => array(
              'default preset' => 'linked_medium',
            ),
            'large' => array(
              'default preset' => 'linked_large',
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
                'name' => 'image_style',
                'settings' => array(
                  'image_style' => 'square_thumbnail',
                ),
              ),
              array(
                'name' => 'thumbnail',
                'settings' => array(),
              ),
            ),
            'linked_medium' => array(
              array(
                'name' => 'link_to_media',
                'settings' => array(),
              ),
              array(
                'name' => 'image_style',
                'settings' => array(
                  'image_style' => 'medium',
                ),
              ),
              array(
                'name' => 'thumbnail',
                'settings' => array(),
              ),
            ),
            'linked_large' => array(
            	array(
            	  'name' => 'link_to_media',
            	  'settings' => array(),
            	),
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
        ),
      ),
    ),
  );
}

class MediaFlickrStyles extends FileStyles {

	function get_image_uri() {
		if ($image_uri = $this->get('image_uri')) {
			return $image_uri;
		}
		$object = $this->get_object();
		if ($object->uri) {
			$wrapper = file_stream_wrapper_get_instance_by_uri($object->uri);
			return $wrapper->getLocalThumbnailPath();
		}
	}
	function flickr_image($effect) {
		$variables = array(
			'uri' => $this->get_uri(),
			'width' => $this->get_width(),
			'height' => $this->get_height(),
		);
		$variables = array_merge($variables, $this->variables);
		$this->set_output(theme('media_flickr_embed', $variables));
	}
}
