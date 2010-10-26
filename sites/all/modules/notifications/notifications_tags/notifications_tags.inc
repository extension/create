<?php
// $Id: notifications_subscription.class.inc,v 1.1.2.35 2010/07/21 10:55:30 jareyero Exp $
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */
class Notifications_Taxonomy_Term_Subscription extends Notifications_Subscription_Simple {
  /**
   * Get proper name
   */
  public function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    elseif ($term = $this->get_field('term:tid')->drupal_object()) {
      return t('Content tagged with @term', array('@term' => $term->name));
    }
    else {
      parent::get_name();
    }
  }
}

/**
 * Taxonomy term object
 */
class Notifications_Taxonomy_Term extends Notifications_Object {
  public $type = 'taxonomy_term';

  /**
   * Title
   */
  public function get_title() {
    return t('Taxonomy term');
  }
  /**
   * Load object
   */  
  public static function object_load($value) {
    return taxonomy_term_load($value);
  }
  /**
   * Get name for object
   */
  public static function object_name($term) {
    return $term->name;
  }
  /**
   * Get value for object
   */
  public static function object_value($term) {
    return $term->tid;
  }
}

/**
 * Taxonomy term field
 */
class Notifications_Taxonomy_Term_Field extends Notifications_Field_Autocomplete {
  public $type = 'term:tid';
  public $object_type = 'taxonomy_term';
  /**
   * Get field name
   */
  function get_title() {
    return t('Term');
  }
  /**
   * Get description
   */
  function get_description() {
    return t('Taxonomy term');
  }
  /**
   * Get system path
   */
  function get_path() {
    return isset($this->value) ? 'taxonomy/term/' . $this->value : ''; 
  }
   /**
   * Get autocomplete path
   */
  function autocomplete_path() {
    return 'notifications_tags/autocomplete/simple';
  }

  /**
   * Parse value from autocomplete, get the first matching term
   */
  function autocomplete_parse($string) {
    if ($terms = taxonomy_get_term_by_name($string)) {
      $term = reset($terms);
      return $term->tid;
    }
  }
}

class Notifications_Taxonomy_Vocabulary_Subscription extends Notifications_Subscription_Simple {
  /**
   * Get proper name
   */
  public function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    elseif ($vocabulary = $this->get_field('vocabulary:vid')->drupal_object()) {
      return t('Terms in @name', array('@name' => $vocabulary->name));
    }
    else {
      parent::get_name();
    }
  }
}


/**
 * Taxonomy vocabulary object
 */
class Notifications_Taxonomy_Vocabulary extends Notifications_Object {
  public $type = 'taxonomy_vocabulary';

  /**
   * Title
   */
  public function get_title() {
    return t('Vocabulary');
  }
  
  /**
   * Load object
   */
  public static function object_load($value) {
    return taxonomy_vocabulary_load($value);
  }
  /**
   * Get name for object
   */
  public static function object_name($vocabulary) {
    return $vocabulary->name;
  }
  /**
   * Get value for object
   */
  public static function object_value($vocabulary) {
    return $vocabulary->vid;
  }
}

/**
 * Taxonomy term field
 */
class Notifications_Taxonomy_Vocabulary_Field extends Notifications_Field_Select {
  public $type = 'vocabulary:vid';
  public $object_type = 'taxonomy_vocabulary';
  protected static $vocabulary_names;
  /**
   * Get field name
   */
  function get_title() {
    return t('Vocabulary');
  }

  /**
   * Get description
   */
  function get_description() {
    return t('Taxonomy vocabulary');
  }

  /**
   * Get system path
   */
  function get_path() {
    return isset($this->value) ? 'taxonomy/vocabulary/' . $this->value : ''; 
  }

  /**
   * List of options
   */
  function select_options() {
    if (!isset($this->vocabulary_names)) {
      //foreach (taxonomy_get_vocabularies() as $vocab) {
      $this->vocabulary_names = db_query('SELECT vid, name FROM {taxonomy_vocabulary}')->fetchAllKeyed();
    }
    return $this->vocabulary_names;    
  }
}

/**
 * Node events
 */
class Notifications_Taxonomy_Term_Create_Event extends Notifications_Event {
  /**
   * Set main object id and vocabulary id when term is added
   */
  public function set_object($object) {
    if ($object->type == 'taxonomy_term') {
      $this->oid = $object->get_value();
      $this->add_object('taxonomy_vocabulary', $object->get_object()->vid);
    }
    return parent::set_object($object);
  }

  /**
   * Trigger node event
   */
  public function trigger() {
    $term = $this->get_object('taxnomy_term');
    if ($result = parent::trigger()) {
      watchdog('action', 'Triggered notifications for new taxonomy term @name..', array('@name' => $term->name));
    }
    return $result;
  }
}

/**
 * Message templates for node events. Base class.
 */
class Notifications_Taxonomy_Term_Create_Template extends Notifications_Message_Template {
  /**
   * Default texts for this template, may need token replacement
   */
  function default_text($type, $options) {
    switch ($type) {
      case 'subject':
        return t('Update for [vocabulary:name]', array(), $options);
      case 'content':
        return array(
          '#type' => 'messaging_text',
          'header' => t('Update for [vocabulary:name]:', array(), $options),
          'teaser' => 'A new term has been created: [term:name]',
          'more' => t('Read more [term:url]', array(), $options),
        );
      case 'digest':
        return t('New term [term:name] created for vocabulary [vocabulary:name].', array(), $options);
      default:
        return parent::default_text($type, $options);
    }
  }
}