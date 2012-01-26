<?php
drupal_add_js('misc/collapse.js');
$structure = array(

	array( 	'section'=> 'Characteristics', 
			'elements' => array(
				array(	'ranking'=>'field_customer_driven',
						'note' => 'field_customer_driven_note'),
				array(	'ranking'=>'field_open_social_listening',
						'note' => 'field_open_social_listening_note'),
				array(	'ranking'=>'field_attentive_and_responsive',
						'note' => 'field_attentive_responsive_note'),
				array(	'ranking'=>'field_discoverable_ranking',
						'note' => 'field_discoverable_note'),
				array(	'ranking'=>'field_innovative_ranking',
						'note' => 'field_innovative_note'),
				array(	'ranking'=>'field_agile_ranking',
						'note' => 'field_agile_note'),
				array(	'ranking'=>'field_interactive_ranking',
						'note' => 'field_interactive_note')
				)
			
			
			
		),
	array( 	'section'=> 'Behaviors', 
			'elements' => array(
				array(	'ranking'=>'field_user_focused',
						'note' => 'field_user_focused_note'),
				array(	'ranking'=>'field_scholarly_ranking',
						'note' => 'field_scholarly_note'),
				array(	'ranking'=>'field_actively_listening',
						'note' => 'field_acitve_listening_note'),
				array(	'ranking'=>'field_curator_ranking',
						'note' => 'field_currator_note'),
				array(	'ranking'=>'field_focused_topics_ranking',
						'note' => 'field_focused_topics_note'),
				array(	'ranking'=>'field_innovators_ranking',
						'note' => 'field_innovators_note'),
				array(	'ranking'=>'field_flow_ranking',
						'note' => 'field_flow_note'),
				array(	'ranking'=>'field_adopters_ranking',
						'note' => 'field_adopters_note')
				)
			
			
			
		),
	array( 	'section'=> 'Resources', 
			'elements' => array(
				array(	'ranking'=>'field_project_manager_ranking',
						'note' => 'field_project_manager_note'),
				array(	'ranking'=>'field_effective_leadership',
						'note' => 'field_effective_leadership_note'),
				array(	'ranking'=>'field_funding_ranking',
						'note' => 'field_funding_note'),
				array(	'ranking'=>'field_emerging_topics',
						'note' => 'field_topics_note'),
				array(	'ranking'=>'field_feedback_ranking',
						'note' => 'field_feedback_note'),
				array(	'ranking'=>'field_plan_ranking',
						'note' => 'field_plan_note'),
				array(	'ranking'=>'field_partnerships_ranking',
						'note' => 'field_partnerships_note'),
				array(	'ranking'=>'field_membership_ranking',
						'note' => 'field_membership_note'),
				array(	'ranking'=>'field_teams_ranking',
						'note' => 'field_teams_note')
				)
			
			
			
		),
	array( 	'section'=> 'Activities', 
			'elements' => array(
				array(	'ranking'=>'field_engaging_content_ranking',
						'note' => 'field_engaging_content_note'),
				array(	'ranking'=>'field_spaces_ranking',
						'note' => 'field_spaces_note'),
				array(	'ranking'=>'field_sensemaking_ranking',
						'note' => 'field_sensemaking_note'),
				array(	'ranking'=>'field_conversations_ranking',
						'note' => 'field_conversations_note'),
				array(	'ranking'=>'field_fundraising_ranking',
						'note' => 'field_fundraising_note'),
				array(	'ranking'=>'field_evaluation_ranking',
						'note' => 'field_evaluation_note'),
				array(	'ranking'=>'field_marketing_ranking',
						'note' => 'field_marketing_note'),
				array(	'ranking'=>'field_performance_ranking',
						'note' => 'field_performance_note'),
				array(	'ranking'=>'field_webcasts_ranking',
						'note' => 'field_webcasts_note'),
				array(	'ranking'=>'field_online_events_ranking',
						'note' => 'field_online_events_note'),
				array(	'ranking'=>'field_conferences_ranking',
						'note' => 'field_conferences_note'),
				array(	'ranking'=>'field_animations_ranking',
						'note' => 'field_animations_note')
				)
			
			
			
		),
	

);


 $gr = og_load($content['group_audience']['#items'][0]['gid']);
 
 
 //$fld = field_info_instance('node', 'field_animations_ranking', 'optimization');
 
 
 //dsm($fld);
?>



<fieldset class="search-advanced collapsible collapsed form-wrapper" id="edit-advanced"><legend><span class="fieldset-legend">Advanced search</span></legend><div class="fieldset-wrapper"><div class="criterion"><div class="form-item form-type-textfield form-item-or">
  <label for="edit-or">Containing any of the words </label>
 <input type="text" id="edit-or" name="or" value="" size="30" maxlength="255" class="form-text" />
</div>
<div class="form-item form-type-textfield form-item-phrase">
  <label for="edit-phrase">Containing the phrase </label>
 <input type="text" id="edit-phrase" name="phrase" value="" size="30" maxlength="255" class="form-text" />
</div>
<div class="form-item form-type-textfield form-item-negative">

  <label for="edit-negative">Containing none of the words </label>
 <input type="text" id="edit-negative" name="negative" value="" size="30" maxlength="255" class="form-text" />
</div>
</div><div class="criterion"><div class="form-item form-type-checkboxes form-item-type">
  <label for="edit-type">Only of the type(s) </label>
 <div id="edit-type" class="form-checkboxes"><div class="form-item form-type-checkbox form-item-type-article">
 <input type="checkbox" id="edit-type-article" name="type[article]" value="article" class="form-checkbox" />  <label class="option" for="edit-type-article">Article </label>

</div>
<div class="form-item form-type-checkbox form-item-type-page">
 <input type="checkbox" id="edit-type-page" name="type[page]" value="page" class="form-checkbox" />  <label class="option" for="edit-type-page">Basic page </label>

</div>
<div class="form-item form-type-checkbox form-item-type-faq">
 <input type="checkbox" id="edit-type-faq" name="type[faq]" value="faq" class="form-checkbox" />  <label class="option" for="edit-type-faq">FAQ </label>

</div>
<div class="form-item form-type-checkbox form-item-type-feed-items">
 <input type="checkbox" id="edit-type-feed-items" name="type[feed_items]" value="feed_items" class="form-checkbox" />  <label class="option" for="edit-type-feed-items">Feed Items </label>

</div>
<div class="form-item form-type-checkbox form-item-type-feeds">
 <input type="checkbox" id="edit-type-feeds" name="type[feeds]" value="feeds" class="form-checkbox" />  <label class="option" for="edit-type-feeds">Feeds </label>

</div>
<div class="form-item form-type-checkbox form-item-type-forum">
 <input type="checkbox" id="edit-type-forum" name="type[forum]" value="forum" class="form-checkbox" />  <label class="option" for="edit-type-forum">Forum topic </label>

</div>
<div class="form-item form-type-checkbox form-item-type-group">

 <input type="checkbox" id="edit-type-group" name="type[group]" value="group" class="form-checkbox" />  <label class="option" for="edit-type-group">Group </label>

</div>
<div class="form-item form-type-checkbox form-item-type-cop-document">
 <input type="checkbox" id="edit-type-cop-document" name="type[cop_document]" value="cop_document" class="form-checkbox" />  <label class="option" for="edit-type-cop-document">Group Admin Document </label>

</div>
<div class="form-item form-type-checkbox form-item-type-event">
 <input type="checkbox" id="edit-type-event" name="type[event]" value="event" class="form-checkbox" />  <label class="option" for="edit-type-event">Group Event </label>

</div>
<div class="form-item form-type-checkbox form-item-type-blog">
 <input type="checkbox" id="edit-type-blog" name="type[blog]" value="blog" class="form-checkbox" />  <label class="option" for="edit-type-blog">Group News </label>

</div>
<div class="form-item form-type-checkbox form-item-type-help-resource">
 <input type="checkbox" id="edit-type-help-resource" name="type[help_resource]" value="help_resource" class="form-checkbox" />  <label class="option" for="edit-type-help-resource">Help resource </label>

</div>
<div class="form-item form-type-checkbox form-item-type-news">

 <input type="checkbox" id="edit-type-news" name="type[news]" value="news" class="form-checkbox" />  <label class="option" for="edit-type-news">News </label>

</div>
<div class="form-item form-type-checkbox form-item-type-optimization">
 <input type="checkbox" id="edit-type-optimization" name="type[optimization]" value="optimization" class="form-checkbox" />  <label class="option" for="edit-type-optimization">Optimization Assessment Scorecard </label>

</div>
<div class="form-item form-type-checkbox form-item-type-public-article">
 <input type="checkbox" id="edit-type-public-article" name="type[public_article]" value="public_article" class="form-checkbox" />  <label class="option" for="edit-type-public-article">eXtension Admin Document </label>

</div>
</div>
</div>
</div><div class="action"><input type="submit" id="edit-submit--2" name="op" value="Advanced search" class="form-submit" /></div></div></fieldset>




<article id="article-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix" role="article"<?php print $attributes; ?>>

  <?php print $unpublished; ?>
  <?php if ($title && !$page): ?>
    <header>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1<?php print $title_attributes; ?>>
          <a href="<?php print $node_url; ?>" rel="bookmark"><?php print $title; ?></a>
        </h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
    </header>
  <?php endif; ?>
  <?php if ($display_submitted): ?>
    <footer class="submitted">
      <?php //print $user_picture; ?>
      <?php print $updated; ?>
    </footer>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      //print render($content['group_audience']);
	  print '<div class="scorecard_label">Optimization Assessment Scorecard for Group: <span>'.$gr->label.'</span></div>';
	  $overal_sum = 0;
	  $overal_num = 0;
	  foreach($structure as $sections){
		$sum = 0;
		$num = 0;
	  	print '<section>
		<h4>'.$sections['section'].'</h4>';
		foreach($sections['elements'] as $element){
			$fld = field_info_instance('node', $element['ranking'], 'optimization');
			print '<div class="scorecard_field">';
			print '
			
			<fieldset class="collapsible collapsed">
			
			
				<legend>';
			print '<span class="scorecard_score fieldset-legend">'.$content[$element['ranking']]['#title'].$content[$element['ranking']]['#items'][0]['value'].'			</span></legend>';
			print ''.$fld['description'].'
			</fieldset>';
			print '<div class="scorecard_note">'.$content[$element['note']]['#items'][0]['safe_value'].'</div>';
			print '</div>';
			$sum += $content[$element['ranking']]['#items'][0]['value'];
			$num++;
			$overal_sum += $content[$element['ranking']]['#items'][0]['value'];
			$overal_num ++;
		}
		print '<div class="scorecard_section_score">Overal '.$sections['section'].' Score: <span>'.round(($sum/$num), 2).'</span></div>';
		print '</section>';
	  }
	  print '<div class="scorecard_overal_score">Average Overall Optimization Score: <span>'.round(($overal_sum/$overal_num), 2).'</span></div>';
	  
    ?>
  </div>

  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?>

</article>
