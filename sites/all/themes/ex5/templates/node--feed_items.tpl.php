<article id="feed_items-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix" role="feed_item"<?php print $attributes; ?>>
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
      hide($content['comments']);
      hide($content['links']);
	  hide($content['additional_field']);
      print truncate_utf8(preg_replace( '/\s+/', ' ',strip_tags( render(  $content['body'] ))), 270,true,true);
	  print '<br />
<br />
<a href="'.$field_url[0]['safe_value'].'" target="_blank">read more on about.extension.org blog</a>';
    ?>
  </div>
</article>
