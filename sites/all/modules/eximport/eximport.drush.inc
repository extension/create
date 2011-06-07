<?php
/**
* Implements hook_drush_help().
*/
function eximport_drush_help($command) {
  switch ($command) {
    case 'drush:import':
      return dt('Import an xml file into drupal');
    case 'drush:rewrite_links':
      return dt('Rewrite imported urls');
  }
}

/**
* Implements hook_drush_command().
*/
function eximport_drush_command() {
  $items = array();
  $items['import'] = array(
    'description' => dt('Run my command.'),
    'arguments'   => array(
      'arg1'    => dt('The name of the xml file to import (this file must exist in the imports directory)'),
    ),
    'examples' => array(
      'Standard example' => 'drush import cop-0.xml',
    ),
    'aliases' => array('importwikidump'),
  );
  $items['rewrite_links'] = array(
    'description' => dt('Run my command.'),
    'examples' => array(
      'Standard example' => 'drush rewrite_links',
    ),
    'aliases' => array('rewritelinks'),
  );
  return $items;
}

/**
* Callback function for drush import. 
* Callback is called by using drush_hook_command() where
* hook is the name of the module (MYMODULE) and command is the name of
* the Drush command with all "-" characters converted to "_" characters (my_command)
*
* @param $arg1
*   name of the xml file in the import directory that you want to import
*/
function drush_eximport_import($arg1 = NULL) {
  //check if the argument was passed in and just print it out
  if (isset($arg1)) {
   drush_print($arg1);
   eximport_import_from_file($arg1);
  }

  //log to the command line with an OK status
  drush_log('  finished import', 'ok');
}

/**
* Callback function for drush rewrite_links. 
*/
function drush_eximport_rewrite_links() {

  eximport_rewrite_imported_urls();
  //log to the command line with an OK status
  drush_log(' finished rewriting imported links', 'ok');
  
}

?>