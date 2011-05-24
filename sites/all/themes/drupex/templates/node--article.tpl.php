<?php
// $Id: node.tpl.php,v 1.2 2010/12/01 00:18:15 webchick Exp $

/**
 * @file
 * Display a node of type "article".
 */
?>

<div class="grid_16 alpha omega override_960px content_page" >
            <div class="grid_11 alpha omega override_650px" id="content_tag">
                <div class="content_gutter">

					
					<div class = "headline left">
					  <h1 class="title"><?php print $title; ?></h1>
					  <p class="caption"><?php print $updated; ?></abbr></p>
					</div>
					<br class="clearing" />
					
					
					
					<div id="content_css_reset">
						<div id="article">
						
						<?php
						  // We hide the comments and links now so that we can render them later.
						  hide($content['comments']);
						  hide($content['links']);
						  hide($content['additional_field']);
						  print render($content);
						?>
					
					  
						</div>
					</div>
					<br class="clearing" /> 
				</div>
		</div>
</div>
<?php
print render($content['additional_field']);
?>


  <?php
    // Remove the "Add new comment" link on the teaser page or if the comment
    // form is being displayed on the same page.
    if ($teaser || !empty($content['comments']['comment_form'])) {
      unset($content['links']['comment']['#links']['comment-add']);
    }
    // Only display the wrapper div if there are links.
    $links = render($content['links']);
    if ($links):
  ?>
    <div class="link-wrapper">
      <?php print $links; ?>
    </div>
  <?php endif; ?>

  <?php print render($content['comments']); ?>
