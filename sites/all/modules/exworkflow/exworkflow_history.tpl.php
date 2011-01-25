<?php if (count($workflow_events) > 0):?>

  <table>

    <tr>
        <th>Set By</th>
        <th>Revision Id</th>
        <th>Workflow Description</th>
        <th>Created</th>
    </tr>
  
  <?php foreach ($workflow_events as $workflow_event): ?>
    <tr>
        <td><?php print l($workflow_event->login, "user/{$workflow_event->user_id}"); ?></td>
        <td><?php print l($workflow_event->revision_id, "node/{$workflow_event->node_id}/revisions/{$workflow_event->revision_id}/view"); ?></td>
        <td><?php print $workflow_event->description; ?></td>
        <td><?php print format_date($workflow_event->created); ?></td>
    </tr>
  <?php endforeach; ?>  
  </table>
    
<?php else: ?>
<!-- Should not happen as the tab should not show if events don't exist, but doing it anyways -->
<p>There are no workflow events at this time.</p>
<?php endif; ?>