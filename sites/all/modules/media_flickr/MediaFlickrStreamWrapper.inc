<?php
// $Id: MediaFlickrStreamWrapper.inc,v 1.1.2.2 2010/09/19 00:19:06 JacobSingh Exp $

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

  function interpolateUrl() {
    if (isset($this->parameters['p'])) {
      return $this->base_url . 'photos/' . check_plain($this->parameters['u']) . '/' . check_plain($this->parameters['p']);
    }
  }
  
  function getTarget($f) {
    return FALSE;  
  }
  
  static function getMimeType($uri, $mapping = NULL) {
    return 'image/jpeg';
  }
}
