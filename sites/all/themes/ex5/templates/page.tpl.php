<script src="https://www.extension.org/javascripts/global_nav_internal.js" type="text/javascript"></script>

<div id="page-wrapper">

	<header class="menu-bar clearfix" role="banner">
   		<div>
			<div class="logo">
				<a href="/"><img src="<?php print base_path() . path_to_theme(); ?>/images/create_logo.png" border="0" width="260" height="100" /></a>
			</div>
		<?php print render($page['header']); ?>
		<?php print render($page['menu_bar']); ?>
		<?php print render($page['global_nav_bar']); ?>
		<?php print $breadcrumb; ?>
		</div>
  </header>
  <?php if ($messages): ?>
    <div id="messages"><div class="section clearfix">
      <?php print $messages; ?>
    </div></div> <!-- /.section, /#messages -->
  <?php endif; ?>
  <?php print render($page['help']); ?>
  <?php if ($highlighted = render($page['highlighted'])): print $highlighted; endif; ?>
  <div id="main-wrapper" class="clearfix">
	<?php if ($sidebar_first = render($page['sidebar_first'])): print $sidebar_first; endif; ?>
	<section id="main-content" role="main">
	  <?php print render($title_prefix); ?>
	  <?php if ($title): ?>
		<h1 id="page-title"><?php print htmlspecialchars_decode($title); ?></h1>
	  <?php endif; ?>
	  <?php print render($title_suffix); ?>

	  <?php if ($tabs = render($tabs)): ?>
		<div class="tabs"><?php print $tabs; ?></div>
	  <?php endif; ?>

	  <?php if ($action_links = render($action_links)): ?>
		<ul class="action-links"><?php print $action_links; ?></ul>
	  <?php endif; ?>

	  <?php print render($page['content']); ?>

	  <?php print $feed_icons; ?>

	</section>
	<section id="right-column">
		<h1></h1>
		<div id="right-column-content" class="clearfix">
		<?php if ($sidebar_second = render($page['sidebar_second_user'])): print $sidebar_second; endif; ?>
		<?php if ($sidebar_second = render($page['sidebar_second'])): print $sidebar_second; endif; ?>
		</div>
		<h2></h2>
	</section>
	<section id="footers">
 	<?php //include 'footer_first.inc'; ?>
  <?php if ($footer = render($page['footer'])): ?>
	<footer><?php print $footer; ?></footer>
  <?php endif; ?>
  	</section>
  </div>
</div>