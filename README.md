# vtGBte (Very Tiny GameBoy Tile Editor)

![](./screen.png)

A simple *keyboard-controlled* Game Boy tile editor for Unix systems written in C using ncurses library. 
Hope you'll find it useful. 

## Building

First you'll need to clone the repo:
`git clone https://github.com/paul-arutyunov/vtGBte.git`
`cd vtGBte`

To build the project, run
`$ make`
in your terminal.

This will create binary file in the repo dir.

To install the program, simply run
`$ make install`

This will install the binary to the directory specified in the makefile (`/usr/local/bin` by default).

Expectably, to uninstall run
`$ make uninstall`

Before re-building always run
`$ make clean`
This will remove all object files in the repo dir.
Also it'd be nice to have a script named, say, "rebuild.sh" located in the repo dir and consisting of two lines.


*Note that you'll need `ncurses` library located somwhere on your computer.*
*And `make`.*
*And `install`.*
*And `rm`.*
*And, if thruth to be told, a C compiler is required too.*
*Why am I writing this.*
