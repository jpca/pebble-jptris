pebble-jptris
=============

This is Tetris clone for pebble smart watch.
The code is a fork of an old Borland Turbo-C source code for MS-DOS that I wrote in 1994 :-)

Compilation
-----------
Install Pebble 2.0 SDK available at https://developer.getpebble.com/2/
 
create a pebble project with pebble  used
>pebble new-project jptris
>cd jptris

Copy .h an .c source in this github project under src/ directory and build
>pebble build

Install
>pebble install --phone <your phone ip address in your lan>

Tips
----
You will find the grid (playground) and pieces definitions on top of jptris.c,
you can easily change them to customize your own tetris.

Binary download
---------------
This code is for pebble 2.0 firmware.
During the BETA period, upgrade your pebble at this page : https://developer.getpebble.com/2/getting-started/

1.0 binary version available at : https://dl.dropboxusercontent.com/u/2910062/jptris.pbw

Player manual
-------------
* up button : move left
* down button : move right
* middle button : rotate
* back button : end game
* extra pieces if you exceed 500 points

Planned features
----------------

* middle button long push to move down
* accelerometer to move left, right & down
* high score screenshot & sharing
