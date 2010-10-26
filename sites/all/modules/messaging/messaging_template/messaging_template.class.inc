<?php
// $Id: messaging_message.class.inc,v 1.1.2.17 2010/06/21 12:05:28 jareyero Exp $
/**
 * @file
 * Drupal Messaging Framework - Message template
 */

class Messaging_Template {
  // Template name
  public $template;
  // Language object
  public $language;
  // Send method name
  public $method;
  // Aditional options for token replacement
  public $token_options = array();
  protected $objects = array();
  protected $text_parts;
  protected $text_keys;
  protected $text;
  
  public function __construct($template, $language = NULL, $send_method = NULL, $keys = array('subject', 'header', 'main', 'footer')) {
    $this->template = $template;
    $this->language = $language;
    $this->method = $send_method;
    $this->text_keys = $keys;
  }
  
  /**
   * Build message object
   */
  public function build_message($method = NULL, $language = NULL) {
    $method = $method ? $method : $this->method;
    $language = $language ? $language : $this->language;    
    $text = $this->build_text($method, $language);
    // Get subject out of text and build the message array
    if (isset($text['subject'])) {
      $subject = $text['subject'];
      unset($text['subject']);
    }
    else {
      $subject = '';
    }
    $message = new Messaging_Message(array('method' => $method, 'subject' => $subject, 'body' => $text));
    $message->set_language($language);
    return $message;
  }
  
  /**
   * Build text parts with object replacement
   */
  public function build_text($method = NULL, $language = NULL) {
    $language = $language ? $language : $this->language;
    $text = $this->build_template($method, $language);
    return messaging_template_text_replace($text, $this->objects, $language, $this->token_options);
  }
  
  /**
   * Get raw template parts
   */
  public function build_template($method = NULL, $language = NULL) {
    $method = $method ? $method : $this->method;
    $language = $language ? $language : $this->language;
    if (!isset($this->text_parts[$method][$language->language])) {
      $parts = array();
      foreach ($this->text_keys as $key) {
        $parts[$key] = isset($this->text[$key]) ? $this->text[$key] : messaging_template_text_part($this->template, $key, $method, $language);
      }
      $this->text_parts[$method][$language->language] = $parts;
    }
    return $this->text_parts[$method][$language->language];
  }
  
  /**
   * Set user, which will set method, language, and user object
   * 
   * @return TRUE if all the related properties set
   */
  public function set_user($account, $method = NULL, $language = NULL) {
    $method = $method ? $method : messaging_method_default($account);
    if ($method) {
      $this->method = $method;
      $this->language = $language ? $language : user_preferred_language($account);
      $this->set_object('user', $account);
      return TRUE;
    }
  }

  /**
   * Set/replace single object
   */
  public function set_object($type, $object) {
    if ($object) {
      $this->objects[$type] = $object;
    }
    elseif (isset($this->objects[$type])) {
      unset($this->objects[$type]);
    }
  }
  /**
   * Set object list
   */
  public function set_objects($objects) {
    $this->objects = $objects;
  }
  /**
   * Set predefined text so we don't use the template for that part
   */
  public function set_text($key, $text) {
    $this->text[$key] = $text;
  }

}