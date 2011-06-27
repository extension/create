<?php
// $Id:$

/**
 *this filter extends the one filed string filter to add all of the text fileds in the kaltura db
 *
 */
class kaltura_views_handler_filter_kaltura_text extends views_handler_filter {
  // exposed filter options
  var $always_multiple = TRUE;
  //var $always_required = TRUE;

  function option_defintion() {
    $options = parent::option_defintion();

    $options['expose']['contains']['required'] = array('default' => TRUE);

    return $options;
  }


  function value_form(&$form, &$form_state) {

    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];
      $source = 'edit-' . drupal_html_id($this->options['expose']['operator_id']);
    }
      $form['value'] = array(
        '#type' => 'textfield',
        '#title' => t('Value'),
        '#size' => 60,
        '#default_value' => $this->value,
        //'#dependency' => array($source => 'contains'),
      );
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value;
      }
    if (!isset($form['value'])) {
      // Ensure there is something in the 'value'.
      $form['value'] = array(
        '#type' => 'value',
        '#value' => NULL
      );
    }
  }


  function query() {
    //parent::query();
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";
    $field1 = "$this->table_alias.kaltura_tags";
    $field2 = "$this->table_alias.kaltura_description";
    $concat = "($field OR  $field1 OR  $field2)";
    $placeholder = $this->placeholder();
    $this->query->add_where_expression($this->options['field'], "$field LIKE $placeholder", array($placeholder => '%' . db_like($this->value) . '%'));
    $this->query->add_where_expression($this->options['field'], "$field1 LIKE $placeholder", array($placeholder => '%' . db_like($this->value) . '%'));
    $this->query->add_where_expression($this->options['field'], "$field2 LIKE $placeholder", array($placeholder => '%' . db_like($this->value) . '%'));
    $this->query->set_where_group('OR', $this->options['field']);
  }


}
