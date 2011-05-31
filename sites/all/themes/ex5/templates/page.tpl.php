<div id="pageBanners">
  <ul id="extensionNav">
    <li class="group"><span>Public sites</span>
      <ul>
        <li><a href="http://about.extension.org/" title="A starting place for all things eXtension">About.eXtension</a></li>
        <li><a href="http://www.extension.org" title="The eXtension Public Site">Public Site</a></li>
        <li><a href="http://search.extension.org" title="One search for hundereds of Cooperative Extension Sites">Search</a></li>
      </ul>
    </li>
    <li class="group"><span>Collaboration</span>
      <ul>
        <li><a href="http://create.extension.org/" title="A collaborative development of resources about the eXtension initiative: news, governance, and projects.">Create</a></li>
        <li><a href="http://cop.extension.org/wiki" title="CoP wiki for content creation">CoP</a></li>
        <li><a href="http://campus.extension.org/ " title="Moodle courses developed by eXtension Communities of Practice, as well as Extension professionals throughout the Cooperative Extension System, for use by the general public">Campus</a></li>
        <li><a href="http://collaborate.extension.org/wiki/" title="A wiki for professionals from the land-grant universities to collaborate on topics of interest.">Collaborate</a></li>
      </ul>
    </li>
    <li class="group last"><span>Content Tools and People</span>
      <ul>
        <li><a href="http://aae.extension.org" title="Ask an Expert">AaE</a></li>
        <li><a href="http://cop.extension.org/events" title="Events">Events</a></li>
        <li><a href="http://cop.extension.org/faq" title="FAQ">FAQ</a></li>
        <li><a href="http://people.extension.org/" title="Manage your eXtension profile, find colleagues, create and join communities.">People</a></li>
        <li><a href="http://learn.extension.org/" title="Upcoming Professional Development Sessions and Archived PD Recordings">Learn</a></li>
      </ul>
    </li>
  </ul>
</div>

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
	<section id="right-column">
		<h1></h1>
		<div id="right-column-content" class="clearfix">
		<?php if ($sidebar_second = render($page['sidebar_second_user'])): print $sidebar_second; endif; ?>
		<?php if ($sidebar_second = render($page['sidebar_second'])): print $sidebar_second; endif; ?>
		</div>
		<h2></h2>
	</section>
	<section id="footers">
 	<?php include 'footer_first.inc'; ?>
  <?php if ($footer = render($page['footer'])): ?>
	<footer><?php print $footer; ?></footer>
  <?php endif; ?>
  	</section>
  </div>
</div>