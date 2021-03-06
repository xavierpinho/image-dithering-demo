image-dithering-demo
====================

This repository contains straightforward implementations of various
well-known image dithering algorithms for my own educational purposes.

Algorithms
----------

* Greyscale (ITU-R BT.601-7, ITU-R BT.709-6 and average)
* Threshold (fixed and random)
* Ordered (2x2, 4x4 and 8x8)
* Floyd-Steinberg
* Jarvis-Judice-Ninke
* Stucki
* Burkes
* Sierra, Sierra lite and 2-row Sierra
* Atkinson

Quick start
-----------

    cd src && make
    cd ../eg && make

The commands above build and populate the `eg` directory with the
resulting images obtained from dithering the file `kodim23.png`.

The resulting images are in NetPBM's P1/P2/P3 file formats.

Building
--------

Run `make` inside the `src` directory.
The source code is in ANSI C but requires `libpng`.

Using
-----

Once built run the `cli` executable found inside the `src` directory.
You may run `cli` without parameters to get help.

License
-------

Public domain (see the `UNLICENSE` file.)

Credits
-------

The test image `kodim23.png` is copied from http://r0k.us/graphics/kodak/.
