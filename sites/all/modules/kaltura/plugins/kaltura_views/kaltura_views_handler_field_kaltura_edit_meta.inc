<?php
// $Id: kaltura_views_handler_field_kaltura_thumb.inc,v 1.1.2.1 2010/08/09 04:33:33 univate Exp $

/*
 * Declaration of new field handler that extends the basic field handler of views module
 *
 */
class kaltura_views_handler_field_kaltura_edit_meta extends views_handler_field {
  function pre_render() {
    ctools_include('modal');
    ctools_include('ajax');
    ctools_modal_add_js();
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    drupal_add_js(array(
      'metadata-modal-style' => array(
          'modalSize' => array(
          'type' => 'scale',
          'width' => .4,
          'height' => .6,
        ),
      ),
    ), 'setting');
    return ctools_modal_text_button('Edit Metadata', 'kaltura/nojs/metadata_editor/' . $value, 'Edit Metadata', 'ctools-modal-metadata-modal-style');
  }
}


