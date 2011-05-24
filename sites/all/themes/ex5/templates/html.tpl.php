<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" <?php print $rdf_namespaces ?>>
    <head profile="<?php print $grddl_profile ?>">
        <?php print $head; ?>
        <title><?php print $head_title; ?></title>
        <?php print $meta; ?>
        <?php print $styles; ?>
    </head>
    <body class="<?php print $classes; ?>"<?php print $attributes; ?>>
		<?php print $page_top; ?>
        <?php print $page; ?>
        <?php print $scripts; ?>
		<?php print $page_bottom; ?>
<?php print $belatedpng; ?>
    </body>
</html>
