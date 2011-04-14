<section id="comments" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <h2<?php print $title_attributes; ?>><?php print t('Comments'); ?></h2>
  <?php print render($title_suffix); ?>

  <?php print render($content['comments']); ?>

  <?php if ($content['comment_form']): ?>
    <h2><?php print t('Add new comment'); ?></h2>
    <?php print render($content['comment_form']); ?>
  <?php endif; ?>

</section>
