<h1>CardBox</h1>

An OpenGL image sorter and browser with a static JSON datafile backend.

<img src="https://raw.github.com/camb416/CardBox/master/screenshot.jpg" />

Compiles against Cinder 0.8.4 http://libcinder.org

<pre>
_KEYMAP_

D...............Toggle Debug Mode (Display testing UI, show cursor widget)
~...............Toggle Fullscreen Mode

</pre>

The application is designed to work with an 22" ELO Touchscreen running at 1050x1680px in portrait mode.

It will load a maximum of 40 images.

A sample data.js data file is included in the package.

This file contains the relative location of the images, and all caption information about them.

This file should be placed in ~/Documents/AMNH/postcards/.

With each revision of this file, new images should be copied to a new folder (beginning with opening) such that if a parse error is detected, the old data file and images will be maintained.

<h2>UPDATING THE ASSETS:</h2>

To update the images in the viewer:

First, duplicate the data.js file, and create a new folder in the assets folder.

before:
<br /><img src="https://raw.github.com/camb416/CardBox/master/doc/assets_folder_structure.png" />

after:
<br /><img src="https://raw.github.com/camb416/CardBox/master/doc/asset_update_structure.png" />

Then, update the "basepath" variable in the data.js file. Then for each card, paste in the values for each element as indicated in the graphic below. The "path" variable should be the exact filename of the file in the new folder you created in assets (in this case, the files should be in "press".

<img src="https://raw.github.com/camb416/CardBox/master/doc/data_file.png" />

Icon Art adapted from art by <a href="http://www.cuberto.com/">Cuberto</a> for <a href="http://www.smashingmagazine.com/2012/02/02/freebie-valentines-day-icon-set/">Smashing Magazine</a>
