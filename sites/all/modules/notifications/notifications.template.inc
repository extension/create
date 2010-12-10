<?php
// $Id: notifications_subscription.class.inc,v 1.1.2.35 2010/07/21 10:55:30 jareyero Exp $
/**
 * @file
 * Drupal Notifications Framework - Templates
 */

/**
 * Message template. This should be able to produce a full message by itself
 */
class Notifications_Message_Template extends Messaging_Message_Template {
  // Subscriptions that triggered this notification, indexed by sid
  protected $subscriptions = array();
  // Events that triggered this notifications, indexed by eid
  protected $events = array();
  // Event content
  protected $content;
  /**
   * Constructor, from notifications event
   */
  public function set_event($event) {
    $this->content = $event->get_content();
    $this->objects = $event->get_objects();
    $this->objects['event'] = $event;
    return $this;
  }
  /**
   * Get content objects for building renderable array
   */
  protected function get_content() {
    return isset($this->content) ? $this->content : array();
  }
  /**
   * Add event object and its corresponding template
   */
  function add_event($event) {
    $this->events[$event->eid] = $event;
    $this->add_template('event:' . $event->eid, $event->get_template());
  }
  /**
   * Get objects as Drupal objects (Removing Notifications object wrapper)
   */
  function get_objects() {
    $objects = array();
    foreach (parent::get_objects() as $type => $object) {
      $objects[$type] = is_a($object, 'Notifications_Object') ? $object->get_object() : $object;
    }
    return $objects;
  }

  /**
   * Default texts for this template, may need token replacement
   */
  protected function default_text($type, $options) {
    switch ($type) {
      case 'subject':
        return array(
          '#tokens' => TRUE,
          '#markup' => t('Notification for [user:name] from !site_name', array('!site_name' => variable_get('site_name', 'Drupal')), $options),
        );
      case 'header':
        return array(
          '#tokens' => TRUE,
          '#markup' => t("Greetings [user:name],", array(), $options),
        );
      case 'content':
        return t("A item to which you are subscribed has been updated", array(), $options);
      case 'footer':
        return array(
          '#type' => 'messaging_text', '#tokens' => TRUE,
          'from' => t('This is an automatic message from !site_name', array('!site_name' => variable_get('site_name', 'Drupal')), $options),
          'subscriptions' => t('To manage your subscriptions, browse to [notifications:subscriptions-manage]', array(), $options),
          'unsubscribe' => t('You can unsubscribe at [user:unsubscribe-url]', array(), $options),
        );
      case 'break':
        return array('#type' => 'messaging_break');
      default:
        return parent::default_text($type, $options);
    }
  }  
}