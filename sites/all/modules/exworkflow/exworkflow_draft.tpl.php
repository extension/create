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
  
<?php foreach ($draft_nodes as $draft_node): ?>
  <tr>
    <td><?php print $draft_node->nid; ?></td>
    <td><?php print l($draft_node->title, "/node/{$draft_node->nid}"); ?></td>
    <td><?php print format_date($draft_node->created); ?></td>
  </tr>
<?php endforeach; ?>
  
</table>


<?php print $pager; ?>
