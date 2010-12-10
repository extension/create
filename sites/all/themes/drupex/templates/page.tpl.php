<?php
// $Id: page.tpl.php,v 1.5 2010/09/09 15:27:08 webchick Exp $

/*
This template was copied from bartik.
Drupex addition is the #extensionNav div

*/
?>
<div id="page-wrapper"><div id="page">
  
      <ul id="extensionNav" class="clearfix">
             <li class="group"><span>Public sites</span>
                 <ul>
                     <li><a href="http://about.extension.org/" title="A starting place for all things eXtension">About.eXtension</a></li>
                     <li><a href="http://www.extension.org/" title="The eXtension Public Site">Public Site</a></li>
                     <li><a href="http://search.extension.org/" title="One search for hundereds of Cooperative Extension Sites">Search</a></li>
                 </ul>
             </li>
             <li class="group"><span>Wikis</span>
                 <ul>
                     <li><a href="http://about.extension.org/wiki/" title="The About wiki: Organizational information, resources and updates about the eXtension initiative">About wiki</a></li>
                     <li><a href="http://cop.extension.org/wiki/" title="CoP wiki for content creation">CoP</a></li>
                     <li><a href="http://collaborate.extension.org/wiki/" title="A wiki for professionals from the land-grant universities to collaborate on topics of interest.">Collaborate</a></li>
                 </ul>
             </li>
             <li class="group"><span>Content Tools and People</span>
                 <ul>
                    <li><a href="http://create.extension.org/" title="Create">Create</a></li>
                     <li><a href="http://aae.extension.org/" title="Ask an Expert">AaE</a></li>
                     <li><a href="http://events.extension.org/" title="Events">Events</a></li>
                     <li><a href="http://faq.extension.org/" title="FAQ">FAQ</a></li>
                     <li><a href="http://people.extension.org/" title="Manage your eXtension profile, find colleagues, create and join communities.">People</a></li>
                     <li><a href="http://learn.extension.org/" title="Upcoming Professional Development Sessions and Archived PD Recordings">Learn</a></li>
                 </ul>
             </li>
             <li class="group last"><span>Moodle</span>
                 <ul>
                     <li><a href="http://campus.extension.org/" title="Moodle courses developed by eXtension Communities of Practice, as well as Extension professionals throughout the Cooperative Extension System, for use by the general public">Campus</a></li>
                     <li><a href="http://pdc.extension.org/" title="Moodle courses developed by Extension professionals for internal staff development and training.">PDC</a></li>
                 </ul>
             </li>
      </ul>

  
  <div id="header"><div class="section clearfix">

    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <div id="name-and-slogan"<?php if ($hide_site_name && $hide_site_slogan) { print ' class="element-invisible"'; } ?>>

        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
              <strong>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </strong>
            </div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id="site-slogan"<?php if ($hide_site_slogan) { print ' class="element-invisible"'; } ?>>
            <?php print $site_slogan; ?>
          </div>
        <?php endif; ?>

      </div> <!-- /#name-and-slogan -->
    <?php endif; ?>

    <?php print render($page['header']); ?>

    <?php if ($main_menu): ?>
      <div id="main-menu" class="navigation">
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu-links',
            'class' => array('links', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </div> <!-- /#main-menu -->
    <?php endif; ?>

    <?php if ($secondary_menu): ?>
      <div id="secondary-menu" class="navigation">
        <?php print theme('links__system_secondary_menu', array(
          'links' => $secondary_menu,
          'attributes' => array(
            'id' => 'secondary-menu-links',
            'class' => array('links', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Secondary menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </div> <!-- /#secondary-menu -->
    <?php endif; ?>

  </div></div> <!-- /.section, /#header -->

  <?php if ($messages): ?>
    <div id="messages"><div class="section clearfix">
      <?php print $messages; ?>
    </div></div> <!-- /.section, /#messages -->
  <?php endif; ?>

  <?php if ($page['featured']): ?>
    <div id="featured" class="section clearfix">
      <?php print render($page['featured']); ?>
    </div> <!-- /#featured -->
  <?php endif; ?>

  <div id="main-wrapper" class="clearfix"><div id="main" class="clearfix">

    <?php if ($breadcrumb): ?>
      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
    <?php endif; ?>

    <?php if ($page['sidebar_first']): ?>
      <div id="sidebar-first" class="column sidebar"><div class="section">
        <?php print render($page['sidebar_first']); ?>
      </div></div> <!-- /.section, /#sidebar-first -->
    <?php endif; ?>

    <div id="content" class="column"><div class="section">
      <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title">
          <?php print $title; ?>
        </h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): ?>
        <div class="tabs">
          <?php print render($tabs); ?>
        </div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>

    </div></div> <!-- /.section, /#content -->

    <?php if ($page['sidebar_second']): ?>
      <div id="sidebar-second" class="column sidebar"><div class="section">
        <?php print render($page['sidebar_second']); ?>
      </div></div> <!-- /.section, /#sidebar-second -->
    <?php endif; ?>

  </div></div> <!-- /#main, /#main-wrapper -->

  <?php if ($page['triptych_first'] || $page['triptych_middle'] || $page['triptych_last']): ?>
    <div id="triptych-wrapper"><div id="triptych" class="clearfix">
      <?php print render($page['triptych_first']); ?>
      <?php print render($page['triptych_middle']); ?>
      <?php print render($page['triptych_last']); ?>
    </div></div> <!-- /#triptych, /#triptych-wrapper -->
  <?php endif; ?>

  <div id="footer-wrapper"><div class="section">

    <?php if ($page['footer_firstcolumn'] || $page['footer_secondcolumn'] || $page['footer_thirdcolumn'] || $page['footer_fourthcolumn']): ?>
      <div id="footer-columns" class="clearfix">
        <?php print render($page['footer_firstcolumn']); ?>
        <?php print render($page['footer_secondcolumn']); ?>
        <?php print render($page['footer_thirdcolumn']); ?>
        <?php print render($page['footer_fourthcolumn']); ?>
      </div> <!-- /#footer-columns -->
    <?php endif; ?>

    <?php if ($page['footer']): ?>
      <div id="footer" class="clearfix">
        <?php print render($page['footer']); ?>
      </div> <!-- /#footer -->
    <?php endif; ?>

  </div></div> <!-- /.section, /#footer-wrapper -->

</div></div> <!-- /#page, /#page-wrapper -->
