<?php
// $Id: kaltura_views_handler_filter_media_type.inc,v 1.1.2.1 2010/08/09 04:33:33 univate Exp $

/*
 * Declaration of new filter handler class that extends the basic operator class of views module
 * We want to supply our set of options on which we will filter results
 */
class kaltura_views_handler_filter_media_type extends views_handler_filter_in_operator {

  /*
   * we use only one function to supply our own set of values
   */
  function get_value_options() {
    $this->value_options = array( 1 => t('Video'), 2 => t('Image'), 5 => t('Audio'));
  }

  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    $form['kaltura_media_type']['#type'] = 'checkboxes';
    $form['kaltura_media_type']['#default_value'] = array(1 => 1, 2 => 2, 5 => 5);
    //$form['kaltura_media_type']['#default_value'] = 'All';
    //unset($form['kaltura_media_type']['#default_value']);
    unset($form['kaltura_media_type']['#multiple']);
    unset($form['kaltura_media_type']['#size']);
  }

  function store_exposed_input($input, $status) {
    parent::store_exposed_input($input, $status);
    //FAPI set 0 as a vlue for each unchecked checkbox so we need to unset it so we woun't get a form error
    $display_id = ($this->view->display_handler->is_defaulted('filters')) ? 'default' : $this->view->current_display;
    if ($status) {
      $session = &$_SESSION['views'][$this->view->name][$display_id];
      if (!empty($session[$this->field])) {
      foreach ($session[$this->field] as $key => $val) {
        if ($val === 0) {
          unset($session[$this->field][$key]);
        }
      }
      }
    }
  }


}


