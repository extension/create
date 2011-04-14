<form action="/search" method="post" id="search-block-form" accept-charset="UTF-8">
<input title="Enter the terms you wish to search for." type="search" id="edit-search-block-form--2" name="search_block_form" placeholder="search" size="15" maxlength="128" class="form-text" autocomplete = "on" results=5 />
<?php
	print $search['hidden'];
	//print $search_form;
  	if (isset($search['extra_field'])): ?>
		<div class="extra-field">
		  <?php print $search['extra_field']; ?>
		</div>
<?php endif; ?>