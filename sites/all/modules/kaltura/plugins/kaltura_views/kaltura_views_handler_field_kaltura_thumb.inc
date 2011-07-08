<?php
// $Id: kaltura_views_handler_field_kaltura_thumb.inc,v 1.1.2.1 2010/08/09 04:33:33 univate Exp $

/*
 * Declaration of new field handler that extends the basic field handler of views module
 * We want to theme the thumbnail field ourselvs.
 */
class kaltura_views_handler_field_kaltura_thumb extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['thumbnail_width'] = array('default' => '80');
    $options['thumbnail_height'] = array('default' => '45');
    $options['with_preview']= array('default' => '');
    $options['preview_width'] = array('default' => '');
    $options['preview_height'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['thumbnail_width'] = array(
        '#type' => 'textfield',
        '#title' => t('Thumbnail Width'),
        '#size' => 5,
        '#default_value' => !empty($this->options['thumbnail_width']) ? $this->options['thumbnail_width'] : '80',
    );
    $form['thumbnail_height'] = array(
        '#type' => 'textfield',
        '#title' => t('Thumbnail Height'),
        '#size' => 5,
        '#default_value' => !empty($this->options['thumbnail_height']) ? $this->options['thumbnail_height'] : '45',
    );
    $form['with_preview'] = array(
      '#type' => 'checkbox',
      '#title' => t('with poping preview'),
      '#default_value' =>  !empty($this->options['with_preview']) ? $this->options['with_preview'] : 0,
    );
    $form['preview_width'] = array(
        '#type' => 'textfield',
        '#title' => t('Preview Width'),
        '#size' => 5,
        '#default_value' => !empty($this->options['preview_width']) ? $this->options['preview_width'] : '',
    );
    $form['preview_height'] = array(
        '#type' => 'textfield',
        '#title' => t('Preview Height'),
        '#size' => 5,
        '#default_value' => !empty($this->options['preview_height']) ? $this->options['preview_height'] : '',
    );
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    $width = !empty($this->options['thumbnail_width']) ? $this->options['thumbnail_width'] : 0;
    $height = !empty($this->options['thumbnail_height']) ? $this->options['thumbnail_height'] : 0;
    $vars['path'] = $value . '/width/' . $width . '/height/' . $height;
    $thumb = theme('image', $vars);
    if ($this->options['with_preview']) {
      $vars['path'] = $value . '/width/' . $width . '/height/' . $height;
      $vars['attributes'] = array('class' => 'k-prev-rotate');
      $thumb = theme('image', $vars);
      $prev_width = !empty($this->options['preview_width']) ? $this->options['preview_width'] : 0;
      $prev_height = !empty($this->options['preview_height']) ? $this->options['preview_height'] : 0;
      $prev_vars['path'] = $value . '/width/' . $prev_width . '/height/' . $prev_height;
      $prev_vars['attributes'] = array('class' => 'k-preview', 'style' => 'display: none;');
      $prev = theme('image', $prev_vars);
      $out = '<div class="thumb-with-prev">';
      $out .= $thumb;
      $out .= $prev;
      $out .= '</div>';
      return $out;
    }
    return $thumb;

  }
}


