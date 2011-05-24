<article class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $unpublished; ?>

  <header>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h3<?php print $title_attributes; ?>><?php print $title ?></h3>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if ($new): ?>
      <em><?php print $new ?></em>
    <?php endif; ?>
  </header>

  <?php if ($picture || $submitted): ?>
    <footer>
      <?php print $picture; ?>
      <?php
        print t('Submitted by !username on !datetime', array(
          '!username' => $author,
          '!datetime' => '<time pubdate="pubdate" datetime="' . $datetime . '">' . $created . '</time>',
          )
        );
      ?>
    </footer>
  <?php endif; ?>

  <div<?php print $content_attributes; ?>>
    <?php
      hide($content['links']);
      print render($content);
      print $signature;
    ?>
  </div>

  <?php if ($links = render($content['links'])): print $links; endif; ?>

</article>
