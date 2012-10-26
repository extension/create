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
      <?php print $updated; ?>
    </footer>
  <?php endif; ?>
  <div<?php print $content_attributes; ?>>
    <?php
	dpm($content);
		
		
	/* Contacts
	-----------------------------------------------------------------------------*/
	
	hide($content['field_primary_contact']);
	hide($content['field_secondary_contact']);
	if(count($content['field_primary_contact']['#items']) != 0 || count($content['field_secondary_contact']['#items']) != 0){
		?>
		<div class="field-label custom-label">Contacts: </div>
		<?php
		if(count($content['field_primary_contact']['#items']) != 0){
			hide($content['field_primary_contact'][0]['links']);
			print render($content['field_primary_contact']);
		}
		if(count($content['field_secondary_contact']['#items']) != 0){
			hide($content['field_secondary_contact'][0]['links']);
			print render($content['field_secondary_contact']);
		}
	}
	
	/* Narative summary
	-----------------------------------------------------------------------------*/
	if(isset($content['body'])){
		hide($content['body']);
		?>
		<div class="field-label custom-label">Narative summary: </div>
		<?php
		print render($content['body']);
	}
	
	/* Youtube
	-----------------------------------------------------------------------------*/
	if ( isset($content["field_number_of_likes"]) || isset($content["field_number_of_views"]) ){
	?>
		<div class="field-label custom-label">Youtube: </div>
		<?php
		if(isset($content["field_number_of_views"])){
			hide($content["field_number_of_views"]);
			print render($content["field_number_of_views"]);
		}
		if(isset($content["field_number_of_likes"])){
			hide($content["field_number_of_likes"]);
			print render($content["field_number_of_likes"]);
		}
	
	}
	/* Twitter
	-----------------------------------------------------------------------------*/
	if ( isset($content["field_number_of_twitter_follower"]) || isset($content["field_average_amplification_rate"]) ){
	?>
		<div class="field-label custom-label">Twitter: </div>
		<?php
		if(isset($content["field_number_of_twitter_follower"])){
			hide($content["field_number_of_twitter_follower"]);
			print render($content["field_number_of_twitter_follower"]);
		}
		if(isset($content["field_average_amplification_rate"])){
			hide($content["field_average_amplification_rate"]);
			print render($content["field_average_amplification_rate"]);
		}
	
	}
	
	/* Facebook
	-----------------------------------------------------------------------------*/
	if ( isset($content["field_number_of_facebook_likes"]) 
								|| isset($content["field_talking_about_this"]) 
								|| isset($content["field_total_reach"]) 
								|| isset($content["field_engaged_users"])
								|| isset($content["field_number_of_stories_created"]) ){
	?>
		<div class="field-label custom-label">Facebook: </div>
		<?php
		if(isset($content["field_number_of_facebook_likes"])){
			hide($content["field_number_of_facebook_likes"]);
			print render($content["field_number_of_facebook_likes"]);
		}
		if(isset($content["field_talking_about_this"])){
			hide($content["field_talking_about_this"]);
			print render($content["field_talking_about_this"]);
		}
		if(isset($content["field_total_reach"])){
			hide($content["field_total_reach"]);
			print render($content["field_total_reach"]);
		}
		if(isset($content["field_engaged_users"])){
			hide($content["field_engaged_users"]);
			print render($content["field_engaged_users"]);
		}
		if(isset($content["field_number_of_stories_created"])){
			hide($content["field_number_of_stories_created"]);
			print render($content["field_number_of_stories_created"]);
		}
	}
	
	/* Webinars
	-----------------------------------------------------------------------------*/
	if ( isset($content["field_number_conducted"]) 
								|| isset($content["field_number_of_participants"]) 
								|| isset($content["field_number_of_views_of_webinar"])){
	?>
		<div class="field-label custom-label">Webinars: </div>
		<?php
		if(isset($content["field_number_conducted"])){
			hide($content["field_number_conducted"]);
			print render($content["field_number_conducted"]);
		}
		if(isset($content["field_number_of_participants"])){
			hide($content["field_number_of_participants"]);
			print render($content["field_number_of_participants"]);
		}
		if(isset($content["field_number_of_views_of_webinar"])){
			hide($content["field_number_of_views_of_webinar"]);
			print render($content["field_number_of_views_of_webinar"]);
		}
	}
	
	/* Grants and contracts
	-----------------------------------------------------------------------------*/
	if (isset($content["field_grants_contracts_supportin"])){
		unset($content["field_grants_contracts_supportin"]["#suffix"]);
	}
	if(count($content["field_grants_contracts_supportin"]["#items"]) == 0 ){
		hide($content["field_grants_contracts_supportin"]);
	}
	
	/* Assessment and evaluation reports
	-----------------------------------------------------------------------------*/
	if ( isset($content["field_enter_the_reports_and_or_p"]) ){
	?>
		<div class="field-label custom-label">Assessment and evaluation reports: </div>
		<?php
		hide($content["field_enter_the_reports_and_or_p"]);
		print render($content["field_enter_the_reports_and_or_p"]);
	}
	
	/* Marketing plans and results
	-----------------------------------------------------------------------------*/
	if ( isset($content["field_enter_reports_and_or_provi"]) ){
	?>
		<div class="field-label custom-label">Marketing plans and results: </div>
		<?php
		hide($content["field_enter_reports_and_or_provi"]);
		print render($content["field_enter_reports_and_or_provi"]);
	}
	
	/* Transrormation
	-----------------------------------------------------------------------------*/
	if ( isset($content["field_to_what_extent_has_extensi"]) || isset($content["field_to_what_extent_does_extens"]) ){
	?>
		<div class="field-label custom-label">Transrormation: </div>
		<?php
		if(isset($content["field_to_what_extent_has_extensi"])){
			hide($content["field_to_what_extent_has_extensi"]);
			print render($content["field_to_what_extent_has_extensi"]);
		}
		if(isset($content["field_to_what_extent_does_extens"])){
			hide($content["field_to_what_extent_does_extens"]);
			print render($content["field_to_what_extent_does_extens"]);
		}
	
	}
	
	/* Educational products
	-----------------------------------------------------------------------------*/
	if ( isset($content["field_to_what_extent_have_collab"]) || isset($content["field_please_comment_on_the_exte"]) ){
	?>
		<div class="field-label custom-label">Educational products: </div>
		<?php
		if(isset($content["field_to_what_extent_have_collab"])){
			hide($content["field_to_what_extent_have_collab"]);
			print render($content["field_to_what_extent_have_collab"]);
		}
		if(isset($content["field_please_comment_on_the_exte"])){
			hide($content["field_please_comment_on_the_exte"]);
			print render($content["field_please_comment_on_the_exte"]);
		}
	
	}
	/* Partnerships
	-----------------------------------------------------------------------------*/
	if(count($content["field_partnershiips"]["#items"]) == 0 ){
		hide($content["field_partnershiips"]);
	}else{
		unset($content["field_partnershiips"]["#suffix"]);
		foreach ($content["field_partnershiips"]["#items"] AS $k=>$v){
			hide($content["field_partnershiips"][$k]["links"]);
		}
	}
		
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
  <?php if ($links = render($content['links'])): print $links; endif; ?>
  <?php print render($content['comments']); ?>
</article>