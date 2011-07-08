<?php print drupal_render($tag_form) ?>


<?php if (count($listview_nodes) > 0):?>

<?php print $pager; ?>

<table>
  
  <tr>
      <th>
          <?php print l('Id', $_GET['q'], 
                      array('query' => array('field' => 'nid') + $sort_direction + $page_number)); ?>
      </th>
      
      <th>
        <?php print l('Title', $_GET['q'],
                     array('query' => array('field' => 'title') + $sort_direction + $page_number)); ?>
      </th>
      <th>
          <?php print l('Reviews', $_GET['q'], 
                       array('query' => array('field' => 'review_count') + $sort_direction + $page_number)); ?>
      </th>
      
      <th>
          <?php print l('Created', $_GET['q'], 
                      array('query' => array('field' => 'created') + $sort_direction + $page_number)); ?>
      </th>
  </tr>
  
<?php foreach ($listview_nodes as $listview_node): ?>
  <tr>
    <td><?php print $listview_node->nid; ?></td>
    <td><?php print l($listview_node->title, "node/{$listview_node->nid}"); ?></td>
    <td><?php print ($listview_node->review_count == null) ? 'n/a' : $listview_node->review_count; ?></td>
    <td><?php print format_date($listview_node->created); ?></td>
  </tr>
<?php endforeach; ?>  
</table>

<?php print $pager; ?>

<?php else: ?>
<p>No content available with specified tags and workflow states.</p>
<?php endif; ?>
