 <div class="panels-flexible-region-4-calendar_list_view">
      <div class="view-content">
<?php 
foreach($data AS $month=>$events){
	print '
        <h3>'.$month.'</h3>';
	$row = 1;
	foreach($events AS $event){
		print '
  			<div class="views-row views-row-'.$row++.'">
  				<div class="views-field views-field-field-event-time">
					<div class="field-content">'.$event['day'].'</div>
				</div>
				<div class="views-field views-field-field-group">
					<div class="field-content"><a href="/node/'.$event['gid'].'">'.$event['group'].'</a></div>
				</div> 
  				<div class="views-field views-field-title">
					<span class="field-content"><a href="/node/'.$event['nid'].'">'.$event['title'].'</a></span>
				</div>
			</div>';
	}
}
?>
    	</div>
</div>