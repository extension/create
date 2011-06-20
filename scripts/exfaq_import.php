<?php
  $db_login = drush_get_option('faq_login');
  $db_pw = drush_get_option('faq_pw');
  $db_name = drush_get_option('db_name');

  if (!isset($db_login) || !isset($db_pw) || !isset($db_name)) {
    drush_print("You must provide a login, password and database name");
    return;
  }

  $faq_database = array(
        'database' => $db_name,
        'username' => $db_login, 
        'password' => $db_pw, 
        'host' => 'localhost', 
        'driver' => 'mysql', 
  );

  // connect to the faq database to pull imported faqs
  Database::addConnectionInfo('db_faq_import', 'default', $faq_database);
  db_set_active('db_faq_import');
  $revisions_query = db_select('revisions', 'rev');
  $revisions_query->addField('rev', 'question_id');
  $revisions_query->addField('rev', 'question_text');
  $revisions_query->addField('rev', 'answer');
  $revisions_query->addField('rev', 'id', 'revision_id');
  $revisions_query->addField('rev', 'current');  
  $revisions_query->addField('rev', 'created_at');  
	$revisions_query->addField('u', 'login');
	$revisions_query->addField('q', 'published_revision_id');
	$revisions_query->addField('q', 'from_aaeid');
	$revisions_query->join('users', 'u', 'rev.user_id = u.id');
	$revisions_query->join('questions', 'q', 'rev.question_id = q.id');
  // if we order by question_id and then by current, the last revision for each question will be the 
  // one that is the current revision (current = 1) as drupal recognizes the last revision brought in for a node 
  // as the current revision in drupal
  $revisions_query->orderBy('rev.question_id, rev.current', 'ASC');
  $revisions_to_import = $revisions_query->execute();
  
  // import comments
  $comments_query = db_select('reviews')
                    ->fields('reviews');
  $comments_query->addField('revisions', 'question_id', 'faq_id');
  $comments_query->addField('u', 'login', 'login');
  $comments_query->join('users', 'u', 'reviews.user_id = u.id');
  $comments_query->join('revisions', null, 'reviews.revision_id = revisions.id');
  $comments_query->orderBy('reviews.created_at', 'ASC');
  $comments_to_import = $comments_query->execute();
  
  // query to get faqs for pulling workflow
  $workflow_query = db_select('questions', 'q')
                    ->fields('q');
  
  $faq_workflows_to_import = $workflow_query->execute();
  
  // query to get faq workflow events
  $workflow_events_query = db_select('question_events', 'qe')
                           ->fields('qe');
  $workflow_events_query->addField('u', 'login', 'user_login');
  $workflow_events_query->join('users', 'u', 'qe.user_id = u.id');
  $workflow_events_query->condition('description', array('edited public site tags', 'reverted'), 'NOT IN');                       
  $workflow_events_to_import = $workflow_events_query->execute();
  
  // query to get both published and personal tags for faqs
  $combined_tags_query = db_select('taggings', 'tgs');
  $combined_tags_query->addField('tgs', 'taggable_id', 'question_id');
  $combined_tags_query->addField('tgs', 'created_at');
  $combined_tags_query->condition(db_and()
                                  ->condition('tagging_kind', array(1,2), 'IN')
                                  ->condition('taggable_type', 'Question'));                      
  $combined_tags_query->groupBy('question_id');
  $combined_tags_query->addExpression('GROUP_CONCAT(DISTINCT tgs.tag_display)', 'tag_names');
  $combined_taglist = $combined_tags_query->execute()->fetchAllAssoc('question_id');
  
  // set back to the default db for the site  
  db_set_active(); 
  
  // DO THE FAQ IMPORT      
  foreach ($revisions_to_import as $revision_to_import) {
    // First, see if we've imported a faq with the same question_id
    $imported_faq = db_query('SELECT * FROM {faq_drupal_mapping} WHERE faq_id = :faqid', array(':faqid' => $revision_to_import->question_id))->fetchObject();
    $author_of_revision = find_faq_author($revision_to_import->login);
    
    $GLOBALS['user'] = $author_of_revision;
    $prepared_node = prepare_node($revision_to_import, $imported_faq, $author_of_revision, $combined_taglist);
    node_save($prepared_node);
    
    drush_print("Imported faq:" . $prepared_node->nid);
    
    // fix timestamps
    db_query("UPDATE {node} SET changed = :changed WHERE nid = :nid",
                array(':changed' => strtotime((string)$revision_to_import->created_at), ':nid' => $prepared_node->nid));
    db_query("UPDATE {node_revision} SET `timestamp` = :tstamp WHERE nid = :nid AND vid = :vid", 
                array(':tstamp' => strtotime((string)$revision_to_import->created_at), ':nid' => $prepared_node->nid, ':vid' => $prepared_node->vid));
    
    // if this is the first revision of this faq, record the mapping of faq id to newly created node id
    if($imported_faq == false) {
      $fields = array(
        'node_id' => $prepared_node->nid,
        'faq_id' => $revision_to_import->question_id,
        'created' => REQUEST_TIME);
      db_insert('faq_drupal_mapping')->fields($fields)->execute();
    }
    
    // record the faq revision to drupal revision mapping
    $mapping_fields = array(
      'node_id' => $prepared_node->nid,
      'faq_revision_id' => $revision_to_import->revision_id,
      'drupal_revision_id' => $prepared_node->vid,  
    );
    db_insert('faq_drupal_revision_mapping')->fields($mapping_fields)->execute();
    
    // if this revision was a published revision, find the faq to node mapping and record the published revision id 
    // for later workflow processing.
    if($revision_to_import->revision_id == $revision_to_import->published_revision_id) {
      db_query("UPDATE {faq_drupal_mapping} SET published_revision_id = :published_revision_id WHERE node_id = :nid AND faq_id = :faq_id ", array(':published_revision_id' => $prepared_node->vid, ':nid' => $prepared_node->nid, ':faq_id' => $revision_to_import->question_id));
    }
    
    // if we're on the current revision, record the current revision in the faq to node reference table for reference
    if($revision_to_import->current == true) {
      db_query("UPDATE {faq_drupal_mapping} SET current_revision_id = :current_revision_id WHERE node_id = :nid AND faq_id = :faq_id ", array(':current_revision_id' => $prepared_node->vid, ':nid' => $prepared_node->nid, ':faq_id' => $revision_to_import->question_id));
    }
    
  }
   
  // MAKE THIS WORKFLOW BUSINESS HAPPEN
  
  // query the lookup table
  $node_reference_query = db_select('faq_drupal_mapping', 'fdm')
                          ->fields('fdm');
  $node_faq_references = $node_reference_query->execute()->fetchAllAssoc('faq_id');
  
  foreach($faq_workflows_to_import as $faq_workflow) {
    // look up faq to node mapping to get node id, current revision id, and published revision id
    $reference_record = $node_faq_references[$faq_workflow->id];
    
    $status_number = get_workflow_status_number($faq_workflow->status);
    $status_text_values = exworkflow_get_workflow_states();
    $status_text = $status_text_values[$status_number];
    $draft_status_number = get_workflow_draft_status_number($faq_workflow->draft_status);
    $draft_status_text = (is_null($draft_status_number)) ? null : $status_text_values[$draft_status_number];
    
    $fields = array(
      'node_id' => $reference_record->node_id,
      'active'  => ($faq_workflow->status == 'archived') ? 0 : 1,
      'current_revision_id' => $reference_record->current_revision_id,
      'review_count' => (is_null($faq_workflow->review_count)) ? 0 : $faq_workflow->review_count,
      'status' => $status_number,
      'status_text' => $status_text,
      'draft_status' => $draft_status_number,
      'draft_status_text' => $draft_status_text,
      'published_at' => (is_null($faq_workflow->published_at)) ? null : strtotime((string)$faq_workflow->published_at),
      'unpublished_at' => (is_null($faq_workflow->unpublished_at)) ? null : strtotime((string)$faq_workflow->unpublished_at),
      'published_revision_id' => $reference_record->published_revision_id,
      'created' => REQUEST_TIME, 
      'changed' => REQUEST_TIME, 
    );
    db_insert('node_workflow')->fields($fields)->execute();
    drush_print("Imported workflow for:" . $reference_record->node_id);
  }
  
  // IMPORT WORKFLOW EVENTS
  
  // query the lookup table to map revisions
  $revision_reference_query = db_select('faq_drupal_revision_mapping', 'revision_mapping')
                          ->fields('revision_mapping');
  $revision_references = $revision_reference_query->execute()->fetchAllAssoc('faq_revision_id');
  
  // query the node_workflow table for reference to node_workflow_id
  $workflow_lookup_query = db_select('node_workflow', 'nw')
                           ->fields('nw');
  $workflow_records = $workflow_lookup_query->execute()->fetchAllAssoc('node_id');
  
  // get list of possible workflow events from the exworkflow module
  $workflow_events_list = exworkflow_get_workflow_events();
  
  foreach($workflow_events_to_import as $workflow_event) {
    // get revision mapping for the workflow event revision
    $revision_mapping = $revision_references[$workflow_event->revision_id];
    
    // get workflow event number and textual description
    $event_id = get_workflow_event_number($workflow_event->description);
    $event_description = $workflow_events_list[$event_id];
    
    $fields = array(
      'node_id' => $revision_mapping->node_id,
      'node_workflow_id' => $workflow_records[$revision_mapping->node_id]->nwid,
      'user_id' => find_faq_author($workflow_event->user_login)->uid,
      'revision_id' => $revision_mapping->drupal_revision_id,
      'event_id' => $event_id,
      'description' => $event_description,
      'created' => strtotime((string)$workflow_event->created_at),
    );
    db_insert('node_workflow_events')->fields($fields)->execute();
    drush_print("Imported workflow event for:" . $revision_mapping->node_id);
  }
  
  // DO THE COMMENTS IMPORT
  foreach($comments_to_import as $comment) {
    // first, find related node to this comment
    $related_faq = db_query('SELECT * FROM {faq_drupal_mapping} WHERE faq_id = :faqid', array(':faqid' => $comment->faq_id))->fetchObject();
    if($related_faq == false) {
      continue;
    }
    
    $comment_author = find_faq_author($comment->login);
    
    if(($comment->comments == null) || (trim($comment->comments) == '')){
      if(($comment->rating != null) && (trim($comment->rating) != '')){
        $comment_body_text = $comment->rating;
      }
      else {
        continue;
      }
    }
    else {
      $comment_body_text = $comment->comments;
    }
    
    $new_comment = prepare_comment($related_faq, $comment_author, $comment, $comment_body_text);
    
    // prepare it for saving
    comment_submit($new_comment);
    // save it baby!
    comment_save($new_comment);
    // fix timestamps, set created and changed to the created time and updated time of the original comment
    db_query("UPDATE {comment} SET changed = :changed, created = :created WHERE cid = :cid",
                array(':changed' => strtotime((string)$comment->updated_at), 
                      ':created' => strtotime((string)$comment->created_at),   
                      ':cid' => $new_comment->cid));
                      
    drush_print("Imported comment for:" . $related_faq->node_id);
  }
  

function prepare_node($imported_revision, $already_saved_faq, $revision_author, $combined_taglist) {
  if($already_saved_faq != false) {
    $node = node_load($already_saved_faq->node_id, NULL, TRUE);
    # Drupal hack to tell node_save that we have another revision on it's way in the import, because 
    # node_save is expecting for 'some' revision value to be set, it doesn't appear that it cares what, 
    # just that it will save a new revision if the property is set.
    $node->revision = '1';
  }
  else {
    $node = new stdClass();
    $node->type = 'faq';
    
    if($imported_revision->from_aaeid != null) {
      $node->field_from_aaeid = array(und => array(0 => array('value' => $imported_revision->from_aaeid)));
    }
    
    node_object_prepare($node);
	
    $node->language = LANGUAGE_NONE;
    $node->created = strtotime((string)$imported_revision->created_at);
  }
  
  $node->uid = $revision_author->uid;
  
  if(strlen($imported_revision->question_text) > 255) {
    $node->field_original_question = array(und => array(0 => array('value' => $imported_revision->question_text)));
    $title_overflow = true;
  }
  else {
    $title_overflow = false;
  }
  
  # make safe import of utf8, limit 255 chars for title and wordsafe is on
  $node->title = truncate_utf8($imported_revision->question_text, 255, TRUE);
  
  // make this tagging deal happen
  // only do this for the current revision b/c we only want to apply tags to a node once
  if($imported_revision->current == true) {  
    $groups = array();
    $group_tags_query = db_select('group_resource_tags', 'grt');
    $group_tags_query->addField('grt', 'resource_tag_name', 'tag_name');
    $group_tags_query->addField('grt', 'nid');
    $group_tags_list = $group_tags_query->execute()->fetchAllAssoc('tag_name');  
    
    if(count($combined_taglist) > 0) {
      $tags_to_import = $combined_taglist[$imported_revision->question_id]->tag_names;
      $tag_array = explode(',', $tags_to_import);
    }
    else {
      $tag_array = array();
    }
    
    // add special overflow tag if the title of the question was greater than 255 characters long
    if($title_overflow == true) {
      $tag_array[] = 'title overflow'; 
    }
    
    foreach($tag_array as $import_tag) {
      $formatted_tag = trim(mb_strtolower($import_tag,'UTF-8'));
      
      // check and see if it's a group tag
      if(array_key_exists($formatted_tag, $group_tags_list)) {
        $group_tag = array('gid' => $group_tags_list[$formatted_tag]->nid);
        if (!in_array($group_tag, $groups)){
          $groups[] = $group_tag;
        }
      }
      // else just save it as a tag
      else {
        $tid = 'autocreate';
    		$terms = taxonomy_get_term_by_name($formatted_tag);

    		if ($terms) {
    		  $term = array_shift($terms);
    		  if ($term) {
    			  $tid = $term->tid;
    		  }
    		}

    		$tag_entry = array('vid' => '1',
    						   'tid' => $tid,
    						   'name' => $formatted_tag);
  
    		$taglist_to_save[] = $tag_entry; 
      } 
    }
    
    if(isset($groups) && (count($groups) > 0)) {
      $node->group_audience = array(LANGUAGE_NONE => $groups);
    }
    
    if(isset($taglist_to_save) && (count($taglist_to_save) > 0)) {
      $node->field_tags = array(LANGUAGE_NONE => $taglist_to_save);
    }
    
  }
  
  $node->changed = strtotime((string)$imported_revision->created_at);
  
  // create the body array
  $body_array = array(LANGUAGE_NONE =>
                  array(0 =>
                    array('summary' => '',
                          'value' => $imported_revision->answer,
                          'format' => 3, // full html
                          )));
  
  
  
  $node->body = $body_array;
  return $node;
}

function prepare_comment($related_faq, $comment_author, $comment, $comment_body_text) {
  // setup truncated comment title taken from comment body
  // taken from comment module and modified
  $comment_subject = truncate_utf8($comment_body_text, 29, TRUE);  

  $new_comment = new stdClass();
  $new_comment->nid = $related_faq->node_id;
  $new_comment->pid = 0; // no parent comment
  $new_comment->uid = $comment_author->uid;
  $new_comment->mail = null;
  $new_comment->name = $comment_author->name;
  $new_comment->created = strtotime((string)$comment->created_at);
  $new_comment->status = COMMENT_PUBLISHED;
  $new_comment->language = LANGUAGE_NONE;
  $new_comment->subject = $comment_subject;
  $new_comment->comment_body[$new_comment->language][0]['value'] = $comment_body_text;
  $new_comment->comment_body[$new_comment->language][0]['format'] = 3; // full html
  return $new_comment;
}

function find_faq_author($user_login) {
  $user_to_return = user_load_by_name($user_login);
  
  if (! $user_to_return) {
    $user_to_return = user_load_by_name('eXtension');
  }
  
  return $user_to_return;
}

function get_workflow_status_number($faq_status) {
  if($faq_status == 'archived') {
    $status_number = array_search('draft', exworkflow_search_workflow_states());
  }
  else {
    $status_number = array_search(strtolower($faq_status), exworkflow_search_workflow_states());
  }
  
  return $status_number;
}

function get_workflow_draft_status_number($faq_draft_status) {
  // setup a mapping of draft status text from faq to drupal
  // based off the exworkflow workflow states defined in the exworkflow module.
  switch ($faq_draft_status) {
    case null:
      return null;
      break;
    case 'draft':
      return 1;
      break;
    case 'awaiting approval':
      return 2;
      break;
    case 'awaiting copy edit':
      return 3;
      break;
    case 'ready for publish':
      return 4;
      break;
  }
}

function get_workflow_event_number($faq_workflow_description) {
  // get mapping of faq event workflow description to drupal's event workflow description
  switch ($faq_workflow_description) {
    case 'moved back to draft':
      return array_search('moved back to draft', exworkflow_get_workflow_events());
      break;
    case 'marked ready for review':
      return array_search('marked ready for review', exworkflow_get_workflow_events());
      break;
    case 'approved':
      return array_search('reviewed', exworkflow_get_workflow_events());
      break;
    case 'copy edited':
      return array_search('copy edited', exworkflow_get_workflow_events());
      break;
    case 'published':
      return array_search('published', exworkflow_get_workflow_events());
      break;
    case 'unpublished':
      return array_search('unpublished', exworkflow_get_workflow_events());
      break;
    case 'made inactive':
      return array_search('made inactive', exworkflow_get_workflow_events());
      break;
  }
}

?>