<?php
// $Id: text.inc,v 1.1.2.4 2010/04/26 01:34:27 jareyero Exp $
/**
 * @file
 * Drupal Messaging Framework - Text filtering functions
 */

/**
 * Message text template
 */
class Messaging_Text {
  protected $text = array();
  protected $build;
  protected $render;
  public $format = MESSAGING_FORMAT_PLAIN;
  
  /**
   * Construct from array of text parts
   */
  function __construct($elements = array()) {
    $this->text = $elements;
  }
  /**
   * Add item of unknown type
   */
  function add_item($name, $value) {
    if (is_string($value)) {
      $this->add_string($name, $value);
    }
    elseif (is_object($value) || is_array($value)) {
      $this->add_element($name, $value);
    }
  }
  /**
   * Add element ready for drupal_render()
   */
  function add_element($name, $element) {
    $this->text[$name] = $element;
  }
  /**
   * Add string
   */
  function add_string($name, $string) {
    $element = array('#markup' => $string);
    return $this->add_element($name, $selement);
  }
  /**
   * Add text object
   */
  function add_text($name, $text = NULL) {
    $text = isset($text) ? $text : new Messaging_Text();
    
  }
  /**
   * Render into text
   */
  function render() {
    return $this->render_text($this->text);
  }
  /**
   * Build into array for drupal_render()
   */
  function build() {
    
  }
  /**
   * Render text recursively
   */
  function render_text(&$text) {
    foreach (element_children($text) as $key) {
      if (is_object($text[$key])) {
        $text[$key] = $value->render();
      }
      else {
        $text[$key] = $this->render_text($text[$key]);
      }
    }
    return drupal_render($text);
  }

  /**
   * Converts strings to plain utf-8 single line
   */
  static function check_subject($text) {
    $text = messaging_text_check_plain($text, NULL);
    // taken from _sanitizeHeaders() in PEAR mail() : http://pear.php.net/package/Mail
    $text = preg_replace('=((0x0A/%0A|0x0D/%0D|\\n|\\r)\S).*=i', NULL, $text);
    return $text;
  }
  
  /**
   * Composes message from different parts, recursively and applies filter
   * 
   * Filter is applied now only once
   * 
   * @param $text
   *   Simple string or array of message parts
   *   It may have named elements like #prefix and #text
   *   or it may be single strings to render straight forward
   * @param $glue
   *   Text to glue all lines together
   * @param $filter
   *   Input format to apply to the results
   */
  static function text_render($text, $glue = '', $format = NULL, $filter = NULL) {
    return messaging_text_render($text, $glue, $format, $filter);
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
    return messaging_text_clean($text, $newline);
  }
  
  /**
   * Truncate messages to given length.  Adapted from node_teaser() in node.module
   */
  static function text_truncate($text, $length) {
    return messaging_text_truncate($text, $length);
  }
}

/**
 * HTML to text simple filtering.
 * - Replace some tags with line endings: p, br, hr, li, h1, h2, h3, h4
 * Strip out all HTML tags and decode entities
 * 
 * @param $text
 *   Text to clean up
 * @param $break
 *   Optional character to replace tags for line breaks
 * @param $list
 *   Optional item list marker, may be a dash '-' or an asterisk '*'
 */
function messaging_text_check_plain($text, $break = "\n", $list = '- ') {
  // This have to be done before the filtering because tag markers may have been previously parsed with check_plain
  $text = str_replace(array('&lt;', '&gt;'), array('<', '>'), $text);
  // Clean up the HTML and replace some tags with line endings
  if (isset($break)) {
    $text = _filter_htmlcorrector($text);
    $text = str_replace(array('</p>', '<br />', '<hr />', '</li>', '</h1>', '</h2>', '</h3>', '</h4>'), $break, $text);
    if (isset($list)) {
      $text = str_replace('<li>', $list, $text);
    }
  }
  // Final text clean up
  return messaging_text_clean($text);
}


/**
 * Clean text of HTML stuff and optionally of line endings
 * 
 * @param $text
 *   Dirty HTML text to be filtered
 * @param $newline
 *   Optional string to be used as line ending
 */
function messaging_text_clean($text, $newline= NULL) {
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
 * Apply filter to message text
 */
function messaging_text_filter($text, $filter) {
  if (!$filter) {
    // Not filtering for this method
    return $text;
  }
  elseif ($function = messaging_text_filter_info($filter, 'filter callback')) {
    return $function($text);
  }
  else {
    // Filter not valid or not available anymore. Apply plaintext filter.
    return messaging_text_check_plain($text);
  }
}

/**
 * Get information of available filters for message texts
 */
function messaging_text_filter_info($name = NULL, $property = NULL) {
  $info = messaging_info('text filters');
  return messaging_array_info($info, $name, $property);
}

/**
 * Get built in filters info, will be provided on messaging_messaging('filter info')
 */
function _messaging_text_filter_info() {
  $filters['messaging_plaintext'] = array(
    'name' => t('Plain text'),
    'description' => t('Filters out all HTML tags and replaces HTML entities by characters, respects HTML line breaks.'),
    'filter callback' => 'messaging_text_check_plain',
  );
  $filters['messaging_html'] = array(
    'name' => t('Safe HTML'),
    'description' => t('Filters out dangerous HTML tags, fixes faulty HTML, converts line breaks into HTML, and turns web and e-mail addresses into clickable links.'),
    'filter callback' => 'messaging_text_format_html',
  );
  $filters['messaging_htmlfast'] = array(
    'name' => t('Fast HTML'),
    'description' => t('Converts line breaks into HTML. Use this one in case your templates are already formatted and filtered and you are possitive you don\'t need further filtering.'),
    'filter callback' => 'messaging_text_format_htmlfast',
  ); 
  $filters['drupal_html2text'] = array(
    'name' => t('HTML to text'),
    'description' => t('Replaces HTML tags and entities with plain text formatting, moving links at the end. This one is only advised for plain text emails.'),
    'filter callback' => 'drupal_html_to_text',
  );
     
  return $filters;  
}

/**
 * Default html filter. 
 * 
 * We use a fake 'messaging' format so all variables affecting this filtering can be overridden
 * - filter_html_messaging = FILTER_HTML_STRIP || FILTER_HTML_ESCAPE
 * - allowed_html_messaging, defaults to '<a> <em> <strong> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd>'
 * - filter_url_length_messaging, defaults to 72
 */
function messaging_text_format_html($text, $format = 'messaging') {
  // Turn URLs into links
  $text = _filter_url($text, $format);
  // Filter out dangerous HTML.
  $text = _filter_html($text, $format);
  // Convert line breaks
  $text = _filter_autop($text);
  // Fix faulty HTML
  $text = _filter_htmlcorrector($text);
  return $text;
}

/**
 * HTML quick filter
 * 
 * This one only fixes line endings when everything else is already formatted and filtered. 
 */
function messaging_text_format_htmlfast($text) {
  // Convert line breaks
  return _filter_autop($text);
}

/**
 * Composes message from different parts, recursively and applies filter
 * 
 * Filter is applied now only once
 * 
 * @param $text
 *   Simple string or array of message parts
 *   It may have named elements like #prefix and #text
 *   or it may be single strings to render straight forward
 * @param $glue
 *   Text to glue all lines together
 * @param $filter
 *   Input format to apply to the results
 */
function messaging_text_render($text, $glue = '', $format = NULL, $filter = NULL) {
  $output = '';

  if (is_array($text)) {
    if (isset($text['#prefix'])) {
      $output .= $text['#prefix'].$glue;
      unset($text['#prefix']);
    }
    if (isset($text['#text'])) {
      $output .= $text['#text'];
      return $output;
    }
    foreach (element_children($text) as $key) {
      // The filter is not passed along
      $text[$key] = messaging_text_render($text[$key], $glue);
    }
    $output .= implode($glue, $text);
  } else {
    $output .= $text;
  }
  
  // The format and the final filter are applied now only once
  if (isset($format)) {
    $output = messaging_text_check_markup($output, $format);
  }
  if (isset($filter)) {
    $output = messaging_text_filter($output, $filter);
  }
  return $output;
}

/**
 * Truncate messages to given length.  Adapted from node_teaser() in node.module
 */
function messaging_text_truncate($text, $length) {
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

/**
 * This is a fast version of check_markup
 * 
 * The differences with regular check_markup are:
 * - It uses a static cache instead of querying the database
 * - It doesn't check for permissions
 * 
 * @see check_markup()
 * 
 * @param $text
 * @param $format
 */
function messaging_text_check_markup($text, $format = FILTER_FORMAT_DEFAULT) {
  $cache = &drupal_static(__FUNCTION__);
  // When $check = TRUE, do an access check on $format.
  if (isset($text)) {
    $format = filter_resolve_format($format);

    // Check for a cached version of this piece of text.
    $cache_id = $format .':'. md5($text);

    if (!isset($cache[$cache_id])) {
      // Convert all Windows and Mac newlines to a single newline,
      // so filters only need to deal with one possibility.
      $text = str_replace(array("\r\n", "\r"), "\n", $text);
  
      // Get a complete list of filters, ordered properly.
      $filters = filter_list_format($format);
  
      // Give filters the chance to escape HTML-like data such as code or formulas.
      foreach ($filters as $filter) {
        $text = module_invoke($filter->module, 'filter', 'prepare', $filter->delta, $format, $text, $cache_id);
      }
  
      // Perform filtering.
      foreach ($filters as $filter) {
        $text = module_invoke($filter->module, 'filter', 'process', $filter->delta, $format, $text, $cache_id);
      }
  
      $cache[$cache_id] = $text;
    }

    return $cache[$cache_id];
  }
  else {
    $text = t('n/a');
  }

  return $text;
}

// Some other function wrappers, for backwards compatibility with old sending methods

/**
 * HTML to text simple filtering.
 */
function messaging_check_plain($text, $break = NULL) {
  return messaging_text_check_plain($text, $break);
}

/**
 * Converts strings to plain utf-8 single line
 */
function messaging_check_subject($text) {
  return Messaging_Method::check_subject($text);
}

/**
 * Build a simple text with message subject and body
 */
function messaging_text_build($message, $glue = ' ') {
  return $message->get_text($glue);
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