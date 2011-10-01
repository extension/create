<?php print '<div class="views-row views-row-14 views-row-even" style="background-image: url(/sites/all/modules/exlistview/images/'.$record->type.'.png)">
	
	  
	  <div class="exlistview-group-label"><a href="/node/'.$record->group_id.'">'.$record->group_name.'</a></div> 
	  <div class="views-field views-field-title">
		<span class="field-content"><a href="/node/'.$record->nodeid.'">'.$node->title.'</a></span>  
	  </div>  
	  <div class="views-field views-field-body">
		<span class="field-content">'.truncate_utf8(preg_replace( '/\s+/', ' ',strip_tags($node->body['und'][0]['safe_value'])), 220,true,true).'</span>
	  </div>  
	  <div class="views-field views-field-last-updated-1">        <span class="field-content"><em class="placeholder">'.format_interval(time() - (int) $node->changed).'</em> ago</span>  </div>  
	  <div class="views-field views-field-field-first-name">        <div class="field-content">by '.$record->first_name.'</div>  </div>  
	  <div class="views-field views-field-field-last-name">        <div class="field-content">'.$record->last_name.'</div>  </div>  
	 </div>';
