
/**
 *  @file
 *  README for the Media Update Module.
 */

This module allows for media entries to be updated once they have been created. Updating 
a media entry will update all the places were the entry is being used.


How to:
Enabling this module will surface an update form on the media edit page. 
1) Go to the media page. i.e. http://your-media-site.com/admin/content/media
2) Click on 'edit' under operations for the media you want to update.
3) Select the new media you want to replace this entry with.
4) Hit save... all content using this piece of media will be updated.

You may need to clear your cache if changes do not show up immediately.

Tested with:
media 7.x-1.0-beta5 
http://drupal.org/node/1209128

media 7.x-2.0-unstable1
http://drupal.org/node/1245124

For additional help, notes and patches
See http://drupal.org/project/media_update 
