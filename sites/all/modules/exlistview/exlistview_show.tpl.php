<?php print drupal_render($tag_form) ?>

<?php $more_than_zero_nodes = FALSE; ?>

<?php foreach ($listview_nodes as $listview_node): ?>
  <?php $more_than_zero_nodes = TRUE; ?>
  <? break; ?>
<?php endforeach; ?>  

<?php if (!$more_than_zero_nodes): ?>
<p>No content available with specified tags and workflow states.</p>
<?php else: ?>

<?php print $pager; ?>

<table>

  <tr>
      <th>
          <?php print l('Id', $_GET['q'], 
                      array('query' => array('field' => 'nid') + $sort_direction + $page_number)); 
          ?>
      </th>
      
      <th>Title</th>
      
      <th>
          <?php print l('Created', $_GET['q'], 
                      array('query' => array('field' => 'created') + $sort_direction + $page_number)); 
          ?>
      </th>
  </tr>
  
<?php foreach ($listview_nodes as $listview_node): ?>
  <tr>
    <td><?php print $listview_node->nid; ?></td>
    <td><?php print l($listview_node->title, "/node/{$listview_node->nid}"); ?></td>
    <td><?php print format_date($listview_node->created); ?></td>
  </tr>
<?php endforeach; ?>
  
</table>


<?php print $pager; ?>
<?php endif; ?>