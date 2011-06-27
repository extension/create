<?php
// $Id: kaltura_views_handler_field_kaltura_votes.inc,v 1.1.2.1 2010/08/09 04:33:33 univate Exp $

/**
 *
 */
class kaltura_views_handler_field_kaltura_votes extends views_handler_field {
  function render($values) {
    $votes = $values->{$this->field_alias};
    return t('@votes counts', array('@votes' => $votes));
  }
}
