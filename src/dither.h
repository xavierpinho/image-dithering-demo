/* dither.h -- Various dithering algorithms. */
#ifndef IMAGE_DITHER_H
#define IMAGE_DITHER_H

#include "image.h"

/* All procedures return a newly-allocated image. */

/* Threshold method. */
Image *Image_dither_threshold_y8_y1(Image *);
Image *Image_dither_threshold_random_y8_y1(Image *);

/* Using 2x2, 4x4 and 8x8 matrices. */
Image *Image_dither_ordered2_y8_y1(Image *);
Image *Image_dither_ordered4_y8_y1(Image *);
Image *Image_dither_ordered8_y8_y1(Image *);

/* Error-diffusion dithering. */
Image *Image_dither_floydsteinberg_y8_y1(Image *);
Image *Image_dither_jarvisjudiceninke_y8_y1(Image *);
Image *Image_dither_stucki_y8_y1(Image *);
Image *Image_dither_burkes_y8_y1(Image *);
Image *Image_dither_sierra_y8_y1(Image *);
Image *Image_dither_sierratworow_y8_y1(Image *);
Image *Image_dither_sierralite_y8_y1(Image *);
Image *Image_dither_atkinson_y8_y1(Image *);

#endif
