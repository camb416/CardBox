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

<img src="https://raw.github.com/camb416/CardBox/master/doc/assets_folder_structure.png" />
