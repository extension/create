<?php

/**
 *  @file
 *  Create a Flickr Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $youtube = new ResourceYouTubeStreamWrapper('flickr://u/[user-code]/p/[photo-code]');
 */
class MediaFlickrStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://www.flickr.com/';
  protected $parameters = array('p','u');
  function interpolateUrl() {
    if (isset($this->parameters['p'])) {
      return $this->base_url . 'photos/' . check_plain($this->parameters['u']) . '/' . check_plain($this->parameters['p']);
    }
  }

  function getTarget($f) {
    return FALSE;
  }
  /*
   * get the url of the original image
   */
  function getExternalUrl(){
	module_load_include('inc', 'media_flickr', 'media_flickr.utilities');
	$url =  _media_flickr_photo_url(basename($this->uri));
	return $url;
  }
  function getOriginalThumbnailPath() {
    module_load_include('inc', 'media_flickr', 'media_flickr.utilities');
    $url =  _media_flickr_photo_url(basename($this->uri));
    return $url;
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-flickr/u/' . check_plain($parts['u']) . '/p/' . check_plain($parts['p']) . '.jpg';
    if (!file_exists($local_path)) {
	  $dirname = drupal_dirname($local_path);
	  file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
	  @copy($this->getOriginalThumbnailPath(), $local_path);
	}
	return $local_path;
  }
  static function getMimeType($uri, $mapping = NULL) {
    return 'image/jpeg';
  }
}
