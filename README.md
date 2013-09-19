Tortuga
=======

Simple C file to introduce new students to programming. It emulates the Logo language turte.

I did it for my lectures at Universitat Politècnica de Catalunya, so the name of the constants
and functions are in spanish language.

It works with any C compiler, and requires [Simple Directmedia Layer (SDL) v1.2](http://www.libsdl.org) to work.

Building instructions for Ubuntu
--------------------------------
1. Install GCC and SDL, if not already installed: `sudo apt-get install build-essential libsdl1.2-dev`
2. Compile the example file in the same folder as tortuga.h with -lSDL and -lm options:
`gcc example.c -o example -lSDL -lm`
3. Run the binary: `./example`

And, voilà! Your turtle is drawing your screen:

![](sample.gif)


