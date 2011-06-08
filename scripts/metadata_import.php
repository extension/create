<?php

  $wikidb = drush_get_option('wikidb');
  $drupaldb = drush_get_option('drupaldb');
  
  if(!isset($wikidb)) {
    $wikidb = 'prod_copwiki';
  }
  
  if(!isset($drupaldb)) {
    $drupaldb = 'prod_create';
  }
  
  $wiki_db_select = "SELECT CONCAT('http://cop.extension.org/wiki/',$wikidb.page.page_title) as wiki_link, $wikidb.bettameta.Lifecycle_Contribute_Role as contrib_roles, $wikidb.bettameta.Lifecycle_Contribute_Entity as contrib_entities, $wikidb.bettameta.Lifecycle_Contribute_Date as contrib_dates FROM $wikidb.page,$wikidb.bettameta where $wikidb.page.page_id = $wikidb.bettameta.page_id and $wikidb.page.page_namespace = 0";
  
  $cross_db_select = "SELECT $drupaldb.url_alias.source as nid, wiki_data.wiki_link, wiki_data.contrib_roles, wiki_data.contrib_entities, wiki_data.contrib_dates from $drupaldb.url_alias, ($wiki_db_select) as wiki_data WHERE wiki_data.wiki_link = $drupaldb.url_alias.alias";

  $query_result = db_query($cross_db_select);
  
  foreach($query_result as $record) {
    $node_id = $record->nid;
    $roles = unserialize(stripslashes($record->contrib_roles));
    $entities = unserialize(stripslashes($record->contrib_entities));
    $dates = unserialize(stripslashes($record->contrib_dates));
    # everything must be present, I'm expecting a role, a person, and an index
    $metadata_items = array();
    foreach($roles as $index => $role_value) {
      if(!empty($role_value)) {
        if(!empty($entities[$index])) {
          $entity_value = $entities[$index];
          if(!empty($dates[$index])) {
            $date_value = strtotime($dates[$index]);
            $metadata_items[] = array('contribution_role' => $role_value,'contribution_author' => $entity_value, 'contribution_date' => $date_value);
          } 
        } 
      } 
    }
    $node = node_load($node_id, NULL, TRUE);
    $node->field_contributors = array(LANGUAGE_NONE => $metadata_items);
    node_save($node);
    print("Processed Node $node_id\n");
  }
  
