<?php
$date_created = DateTime::createFromFormat('Y-m-d H:i:s', $row->field_data_field_event_time_field_event_time_value);
print $date_created->format('F Y');