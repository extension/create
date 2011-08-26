<?php
/**
 * Custom filter handler for views, that handles DATETIME
 */
class exfields_handler_filter_datetime extends date_views_filter_handler {
  
  
  
  function op_between($field) {
    if ($this->operator == 'between') {
      $a = intval(strtotime($this->value['min'], 0));
      $b = intval(strtotime($this->value['max'], 0));
    }
    else {
      $a = intval(strtotime($this->value['max'], 0));
      $b = intval(strtotime($this->value['min'], 0));
    }

    if ($this->value['type'] == 'offset') {
      // changed from original
      $a = (integer)time() + (integer)sprintf('%+d', $a); // keep sign
      $b = (integer)time() + (integer)sprintf('%+d', $b); // keep sign
      // changed from original ends
    }
    // %s is safe here because strtotime scrubbed the input and we might
    // have a string if using offset.
    $this->query->add_where_expression($this->options['group'], "$field >= '".date("Y-m-d", $a)."'");
    $this->query->add_where_expression($this->options['group'], "$field <= '".date("Y-m-d", $b)."'");
  }
  
  

  function op_simple($field) {
    $value = $this->value['value'];
    if (!empty($this->value['type']) && $this->value['type'] == 'offset') {
      $this->query->add_where_expression($this->options['group'], "$field $this->operator DATE_ADD(NOW(), INTERVAL $value SECOND)");
    } else {
      $this->query->add_where_expression($this->options['group'], "$field $this->operator $value");
    }
  }
}