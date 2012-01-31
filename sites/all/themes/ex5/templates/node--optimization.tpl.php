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
	dsm($content);
?>

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
	
	  $results = _node_optimization_calculate_rating($structure, $content);
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      //print render($content['group_audience']);
	  print '<div class="scorecard_label">Optimization Assessment Scorecard for Group: <span>'.$gr->label.'</span></div>';
	  ?>
	  <div class="scorecard_results">
	  	<div class="scorecard_overall">
	  		<div class="scorecard_overall_average_label">Average</div>
			<div class="scorecard_overall_label">Overall</div>
			<div class="scorecard_overall_score_label">Optimization Score</div>
			<div class="scorecard_overall_scale_container" style="width: 182px;">
				<div class="scorecard_overall_scale_value" style="width: <?php print(floor($results['overall']*182/5)); ?>px;"></div>
			</div>
			<div class="scorecard_overall_score">
			<?php print $results['overall']; ?>
			</div>
	  	</div>
		<div class="scorecard_details">
			<?php
			foreach($results['elements'] AS $element => $value){
				?>
				<div class="scorecard_element">
					<div class="scorecard_element_average_label">Average</div>
					<div class="scorecard_element_label"><?php print $element; ?></div>
					<div class="scorecard_element_score_label">Optimization Score</div>
					<div class="scorecard_element_scale_container" style="width: 104px;">
						<div class="scorecard_element_scale_value" style="width: <?php print(floor($value*104/5)); ?>px;"></div>
					</div>
					<div class="scorecard_element_score">
					<?php print $value; ?>
					</div>
				</div>
				<?php
			}
			
			?>
		</div>
	  
	  </div>
	  <?php
	  
	  foreach($structure as $sections){
	  	print '<section>
		<h4>'.$sections['section'].'</h4>';
		foreach($sections['elements'] as $element){
			$fld = field_info_instance('node', $element['ranking'], 'optimization');
			print '<div class="scorecard_field">';
			print '
			<div style="font-size: 18px; width: auto; float: left; font-weight: bold;">'.$content[$element['ranking']]['#items'][0]['value'].'</div>
			<fieldset class="collapsible collapsed">
				<legend>';
			print '<span class="scorecard_score fieldset-legend">'.$content[$element['ranking']]['#title'].'			</span></legend>';
			print '<div class="fieldset-wrapper">'.$fld['description'].'</div>
			</fieldset>';
			print '<div class="scorecard_note">'.$content[$element['note']]['#items'][0]['safe_value'].'</div>';
			print '</div>';
		}
		print '</section>';
	  }
	  if(isset($content['field_scorecard_summary'])){
		  print '<section>
		<h4>Summary</h4>
			<div class="scorecard_field">';
		  print $content['field_scorecard_summary']['#items'][0]['safe_value'];
		  print '</div>
		  </section>';
	  }
    ?>
  </div>

  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?>

</article>
<?php

function _node_optimization_calculate_rating($structure, $content){
	$result = array();
	$overall_sum = 0;
	$overall_num = 0;
	foreach($structure AS $sections){
		$sum = 0;
		$num = 0;
		foreach($sections['elements'] AS $element){
			$sum += $content[$element['ranking']]['#items'][0]['value'];
			$num++;
			$overall_sum += $content[$element['ranking']]['#items'][0]['value'];
			$overall_num ++;
		}
		$result['elements'][$sections['section']] = round(($sum/$num), 2);
	}
	$result['overall'] = round(($overall_sum/$overall_num), 2);
	return $result;
}