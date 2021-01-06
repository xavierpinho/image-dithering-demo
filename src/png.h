/* png.h -- PNG file encoding/decoding procedures. */
#ifndef IMAGE_PNG_H
#define IMAGE_PNG_H

#include "image.h"
#include <stdio.h>

Image *
PNG_import_RGB24(FILE *);

#endif
