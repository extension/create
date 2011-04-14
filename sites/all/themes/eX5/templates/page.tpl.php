<div id="page-wrapper">
	<div id="top-bar">
		<div>
			<ul id="extensionNav" class="clearfix">
				<li><a href="http://www.extension.org/" title="The eXtension Public Site">Public Site</a></li>
				 <li><a href="http://search.extension.org/" title="One search for hundereds of Cooperative Extension Sites">Search</a></li>
				  <li><a href="http://collaborate.extension.org/wiki/" title="A wiki for professionals from the land-grant universities to collaborate on topics of interest.">Collaborate</a></li>
				  <li><a href="http://aae.extension.org/" title="Ask an Expert">AaE</a></li>
				  <li><a href="http://events.extension.org/" title="Events">Events</a></li>
				  <li><a href="http://faq.extension.org/" title="FAQ">FAQ</a></li>
				  <li><a href="http://people.extension.org/" title="Manage your eXtension profile, find colleagues, create and join communities.">People</a></li>
				  <li><a href="http://learn.extension.org/" title="Upcoming Professional Development Sessions and Archived PD Recordings">Learn</a></li>
				  <li class="last"><a href="http://campus.extension.org/" title="Moodle courses developed by eXtension Communities of Practice, as well as Extension professionals throughout the Cooperative Extension System, for use by the general public">Campus</a></li>
			</ul>
	  <?php print render($page['global_nav_bar']); ?>
	  	</div>
	</div>
	<header class="menu-bar clearfix" role="banner">
   		<div>
			<div class="logo">
				<a href="/"><img src="<?php print base_path() . path_to_theme(); ?>/images/ex5-logo.png" border="0" width="164" height="81" /></a>
			</div>
		<?php print render($page['header']); ?>
		<?php print render($page['menu_bar']); ?>
		<?php print $breadcrumb; ?>
		</div>
  </header>
  <?php print $messages; ?>
  <?php print render($page['help']); ?>
  <?php if ($highlighted = render($page['highlighted'])): print $highlighted; endif; ?>
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