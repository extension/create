<?php
// $Id: text.inc,v 1.1.2.4 2010/04/26 01:34:27 jareyero Exp $
/**
 * @file
 * Drupal Messaging Framework - Text filtering functions
 */

/**
 * Message text class.
 * 
 * A Text is a container for renderable elements. Renderable elements can be:
 * - An array for drupal_render()
 * - A simple string
 * - Other Text objects (Texts can be nested)
 */
class Messaging_Text {
  // Pre-built elements, needs to be built before render
  public $text = array();
  // Current template elements, renderable array
  public $elements;
  // Parent text
  protected $textparent;
  // Text format
  public $format = MESSAGING_FORMAT_PLAIN;
  
  /**
   * Construct from array of text parts
   */
  function __construct($elements = array(), $type = 'messaging_text') {
    $this->text = array('#type' => $type);
    foreach ($elements as $key => $item) {
      $this->add_item($key, $item);
    }
  }
  /**
   * Add item of unknown type
   */
  function add_item($name, $value) {
    if (is_string($value)) {
      return $this->add_string($name, $value);
    }
    elseif (is_object($value)) {
      return $this->add_text($name, $value);
    }
    elseif (is_array($value)) {
      return $this->add_element($name, $value);
    }
  }
  /**
   * Add element ready for drupal_render()
   */
  function add_element($name, $element) {
    $this->text[$name] = $element;
    return $this;
  }
  /**
   * Add string
   */
  function add_string($name, $string) {
    $element = array('#markup' => $string);
    return $this->add_element($name, $element);
  }
  /**
   * Add text object
   */
  function add_text($name, $text) {
    $text->set_parent($this);
    return $this->add_element($name, $text);
  }
  /**
   * Create text object and add. Return created object
   */
  function create_text($name) {
    $text = new Messaging_Text();
    $this->add_text($name, $text);
    return $text;
  }

  /**
   * Get text element from this template
   */
  function get_text($type, $options = array()) {
    if (isset($this->text[$type])) {
      return $this->text[$type];
    }
    else {
      return NULL;
    }
  }

  /**
   * Set parent text
   */
  function set_parent($template) {
    $this->textparent = $template;
    return $this;
  }
  /**
   * Render this as text
   */
  public function render($parts = NULL, $options = array()) {
    $elements = $this->build($parts, $options);
    return drupal_render($elements);
  }
  /**
   * Reset built elements
   */
  public function reset() {
    unset($this->elements);
  }
  /**
   * Build all elements, return array
   */
  public function build($parts = NULL, $options = array()) {
    $parts = $parts ? $parts : array_keys($this->text);
    $build = array();
    foreach ($parts as $key) {
      if (isset($this->elements[$key])) {
        $build[$key] = $this->elements[$key];
      }
      elseif($text = $this->get_text($key)) {
        $build[$key] = $this->elements[$key] = $this->text_build($text);
      }
    }
    return $build;
  }
  
  /**
   * Build a message text element
   */
  protected function text_build($element, $options = array()) {
    if (is_object($element)) {
      return $element->build($options);
    }
    elseif (is_string($element)) {
      return array(
        '#markup' => $element,
      );
    }
    elseif (is_array($element)) {
      return $this->element_build($element, $options);
    }  
  }
  /**
   * Build a message element, recursive
   */
  protected function element_build($element, $options = array()) {
    foreach (element_children($element) as $key) {
      $element[$key] = $this->text_build($element[$key], $options);
    }
    return $element;
  }
  /**
   * Converts strings to plain utf-8 single line
   */
  static function check_subject($text) {
    $text = check_plain($text);
    // taken from _sanitizeHeaders() in PEAR mail() : http://pear.php.net/package/Mail
    $text = preg_replace('=((0x0A/%0A|0x0D/%0D|\\n|\\r)\S).*=i', NULL, $text);
    return $text;
  }
  
  /**
   * Clean text of HTML stuff and optionally of line endings
   * 
   * @param $text
   *   Dirty HTML text to be filtered
   * @param $newline
   *   Optional string to be used as line ending
   */
  static function text_clean($text, $newline = NULL) {
    // HTML entities to plain text conversion.
    $text = decode_entities($text);  
    // Filters out all remaining HTML tags
    $text = filter_xss($text, array());
    // Optionally, replace new lines
    if (!is_null($newline)) {
      $text = str_replace("\n", $newline, $text);
    }
    // Trim out remaining beginning/ending spaces
    $text = trim($text);
    return $text;
  }
  
  /**
   * Truncate messages to given length.  Adapted from node_teaser() in node.module
   */
  static function text_truncate($text, $length) {
    // If we have a short message, return the message
    if (drupal_strlen($text) < $length) {
      return $text;
    }  
    // Initial slice.
    $teaser = truncate_utf8($text, $length);
    $position = 0;
    // Cache the reverse of the message.
    $reversed = strrev($teaser); 
    // split at paragraph boundaries.
    $breakpoints = array('</p>' => 0, '<br />' => 6, '<br>' => 4, "\n" => 1);
    // We use strpos on the reversed needle and haystack for speed.
    foreach ($breakpoints as $point => $offset) {
      $length = strpos($reversed, strrev($point));
      if ($length !== FALSE) {
        $position = - $length - $offset;
        return ($position == 0) ? $teaser : substr($teaser, 0, $position);
      }
    } 
    // When even the first paragraph is too long, we try to split at the end of
    // the last full sentence.
    $breakpoints = array('. ' => 1, '! ' => 1, '? ' => 1, ' ' => 0);
    $min_length = strlen($reversed);
    foreach ($breakpoints as $point => $offset) {
      $length = strpos($reversed, strrev($point));
      if ($length !== FALSE) {
        $min_length = min($length, $min_length);
        $position = 0 - $length - $offset;
      }
    }
    return ($position == 0) ? $teaser : substr($teaser, 0, $position);   
  }
}

/**
 * Base template class
 * 
 * A template is a text object that has associated objects and can do token replacement.
 * 
 * These templates have some known parts: subject, header, content, footer
 */
class Messaging_Template extends Messaging_Text {
  // Store multiple objects for token replacement
  protected $objects;
  // Options for string building and text replacement
  protected $options;
  /**
   * Construct from array of text parts
   */
  function __construct($elements = array(), $type = 'messaging_message') {
    parent::__construct($elements, $type);
  }
  /**
   * Get known template parts
   */
  protected function get_parts() {
    return array();
  }
  /**
   * Add object to the list
   */
  function add_object($type, $object) {
    $this->objects[$type] = $object;
    return $this;
  }
  /**
   * Get objects from this template (include parent's ones)
   */
  function get_objects() {
    $objects = isset($this->objects) ? $this->objects : array();
    if (isset($this->textparent)) {
      $objects += $this->textparent->get_objects();
    }
    return $objects;
  }
  /**
   * Set known template elements
   */
  public function set_elements($options = array()) {
    foreach ($this->get_parts() as $key) {
      if (!isset($this->text[$key])) {
        $this->text[$key] = $this->get_text($key, $options);
      }
    }
    return $this;
  }
  /**
   * Get element from elements or default texts
   */
  function get_element($type, $options = array()) {
    if (isset($this->elements[$type])) {
      return $this->elements[$type];
    }
    else {
      return $this->get_text($type, $options);
    }
  }

  /**
   * Build all elements, return array
   */
  public function build($parts = NULL, $options = array()) {
    if (!isset($this->elements)) {
      $this->set_elements($options);
    }
    return parent::build($parts, $options);
  }
  /**
   * Get text element from this template
   */
  public function get_text($type, $options = array()) {
    if (isset($this->text[$type])) {
      return $this->text[$type];
    }
    else {
      $options += $this->get_options();
      return $this->default_text($type, $options);
    }
  }
  /**
   * Get options for texts, translations, etc
   */
  function get_options() {
    if (!isset($this->options)) {
      $this->set_language(language_default());
    }
    return $this->options;
  }
  /**
   * Set language
   */
  function set_language($language) {
    $this->set_option('language', $language);
    $this->set_option('langcode', $language->language);
    $this->reset();
    return $this;
  }
  /**
   * Set options
   */
  function set_option($name, $value = TRUE) {
    $this->options[$name] = $value;
    return $this;
  }
  /**
   * Set user (and reset built elements)
   */
  function set_user($user) {
    $this->add_object('user', $user);
    $this->reset();
    return $this;
  }
  
  /**
   * Build a message element with optional text replacement
   */
  protected function element_build($element, $options = array()) {
    $element = parent::element_build($element, $options);
    if (!empty($element['#tokens']) && (!isset($options['replace']) || $options['replace'])) {
      $element = $this->element_replace($element, $options);
    }
    return $element;
  }
  /**
   * Perform token replace within an element
   */
  protected function element_replace($element, $options = array()) {
    if (!empty($element['#markup'])) {
      $element['#markup'] = $this->token_replace($element['#markup'], $options);
    }
    foreach (element_children($element) as $key) {
      $element[$key] = $this->element_replace($element[$key], $options);
    }
    return $element;
  }

  /**
   * Do token replacement with this template's objects
   */
  protected function token_replace($text, $options = array()) {
    return token_replace($text, $this->get_objects(), $options + $this->get_options());
  }
  /**
   * Default texts for this template, may need token replacement
   */
  protected function default_text($type, $options) {
    // Text not found, something went wrong with our template processing
    return t('Template text not found: @type.', array('@type' => $type), $options);
  }
}

/**
 * Template for a full message (subject, body, etc..)
 */
class Messaging_Message_Template extends Messaging_Template {
  /**
   * Set message elements
   */
  protected function get_parts() {
    return array('title', 'subject', 'header', 'content', 'footer');
  }
  /**
   * Build subject as array
   */
  function build_subject($options = array()) {
    return $this->build(array('title', 'subject'), $options) + array('#type' => 'messaging_subject');
  }
  /**
   * Message subject as string
   */
  public function get_subject($options = array()) {
    return drupal_render($this->build_subject($options));
  }
  /**
   * Build message body
   */
  public function build_body($options = array()) {
    return $this->build(array('header', 'content', 'footer'), $options) + array('#type' => 'messaging_body');
  }
  /**
   * Message body as string
   */
  public function get_body($options = array()) {
    return drupal_render($this->build_body($options));
  }
  /**
   * Message content, without header or footer
   */
  public function build_content($options = array()) {
    return $this->build(array('content'), $options) + array('#type' => 'messaging_text');
  }

  /**
   * Get Message_Object with this template linked
   */
  public function build_message($options = array()) {
    $message = new Messaging_Message();
    $message->set_template($this);
    return $message;
  }

  /**
   * Set destination (and reset built elements)
   */
  function set_destination($destination) {
    $this->add_object('destination', $destination);
    return $this->set_user($destination->get_user());
  }

  /**
   * Default texts for this template, may need token replacement
   */
  protected function default_text($type, $options) {
    switch ($type) {
      case 'title':
        return array(
          '#markup' => variable_get('site_name', 'Drupal'),
          '#prefix' => '[', '#suffix' => '] ',
        );
      case 'footer':
        return array(
          '#type' => 'link',
          '#title' => t('Message from @site_name.', array('@site_name' => variable_get('site_name', 'Drupal')), $options),
          '#href' => url('', array('absolute' => TRUE) + $options),
        );
      default:
        return parent::default_text($type, $options);
    }
  }
}

/**
 * Theme message
 */
function theme_messaging_message($variables) {
  $element = $variables['element'];
}
/**
 * Theme message subject
 */
function theme_messaging_subject($variables) {
  $element = $variables['element'];
  $element += array('#separator' => ' '); 
  return theme('messaging_text', array('element' => $element));
}

/**
 * Theme message body
 */
function theme_messaging_body($variables) {
  $element = $variables['element'];
  return theme('messaging_text', array('element' => $element));
}

/**
 * Theme message body
 */
function theme_messaging_list($variables) {
  $element = $variables['element'];
  $output = $element['#title'];
  $output .= $element['#list_prefix'];
  $output .= $element['#separator'];
  foreach (element_children($element) as $key) {
    $element[$key]['#prefix'] = $element['#item_prefix'];
    $element[$key]['#suffix'] = $element['#item_suffix'];
  }
  $output .= theme('messaging_text', array('element' => $element));
  $output .= $element['#list_suffix'];
  return $output;
}

/**
 * Theme messaging text
 */
function theme_messaging_text($variables) {
  $element = $variables['element'];
  $text = array();
  if (!empty($element['#children'])) {
    $text[] = $element['#children'];
  }
  foreach (element_children($element) as $key) {
    $text[] = is_array($element[$key]) ? drupal_render($element[$key]) : $element[$key];
  }
  return implode($element['#separator'], $text);
}