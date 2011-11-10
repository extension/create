<?php

class exfields_handler_filter_overall extends views_handler_filter_in_operator {
  var $value_form_type = 'radios';
 
 
 
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Appraisal overal score');
      
      $this->value_options = array('1'=>t('1'),'2'=> t('2'),'3'=> t('3'));
    }
  }
}