# flruler
A desktop pixel ruler. Runs on Mac, Linux, Windows, FreeBSD.

A program for measuring the pixel size of on-screen elements. Intended
for GUI debugging to verify widget separations and sizes, though may
also be used for general computer graphics debugging purposes, including
measuring pixel distances of images, web applications, etc.

You can easily make copies of the ruler and reorient it with arrow keys and mouse: 
    
    Keyboard     Description
    --------     ---------------------------------------------
       c         Copy the current ruler (that has keyboard focus)
       Esc       Closes current ruler (that has keyboard focus)
       q         Quit all rulers
                 _
       Up         |
       Down       |_ Re-orients ruler
       Left       |  Arrow points to where zero will be
       Right     _|

       +         Increase length of ruler by 10%
       -         Decrease length of ruler by 10%

   Mouse Operation      Description
   ---------------      ---------------------------------------------
   Left-Drag            Drags current ruler around for positioning
   Left-Double-click    Reverses direction of current ruler's numbers
   Left-Triple-click    Rotates current ruler 90 degrees
   Middle-Click         Makes copy of current ruler

## LICENSE

This code is licensed as GPL 3; see `LICENSE` file for details. 

## BUGS

    To report bugs, create an 'issue' on the project's github page:
    https://github.com/erco77/flruler

## DOCUMENTATION

See `./docs/flruler.html`

## WEBSITE

    github:  https://github.com/erco77/flruler
    webpage: http://seriss.com/people/erco/fltk/flruler

