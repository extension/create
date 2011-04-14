<div id="container page">
    <header class="clearfix" role="banner">
	<?php if ($logo): ?>
	  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
		<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
	  </a>
	<?php endif; ?>
	<?php if ($site_name || $site_slogan): ?>
	  <hgroup>
		<?php if ($site_name): ?>
		  <h1>
			<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
			  <?php print $site_name; ?>
			</a>
		  </h1>
		<?php endif; ?>
		<?php if ($site_slogan): ?>
		  <h2><?php print $site_slogan; ?></h2>
		<?php endif; ?>
	  </hgroup>
	<?php endif; ?>
	<?php print render($page['header']); ?>
	<?php print render($page['menu_bar']); ?>
  </header>
  <?php print $messages; ?>
  <?php print render($page['help']); ?>
  <?php if ($highlighted = render($page['highlighted'])): print $highlighted; endif; ?>
  <?php print $breadcrumb; ?>
  <div id="main-wrapper" class="clearfix">
	<?php if ($sidebar_first = render($page['sidebar_first'])): print $sidebar_first; endif; ?>
	<section id="main-content" role="main">
	  <?php print render($title_prefix); ?>
	  <?php if ($title): ?>
		<h1 id="page-title"><?php print $title; ?></h1>
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
	<?php if ($sidebar_second = render($page['sidebar_second'])): print $sidebar_second; endif; ?>
  </div>
  <?php if ($footer = render($page['footer'])): ?>
	<footer><?php print $footer; ?></footer>
  <?php endif; ?>
</div>