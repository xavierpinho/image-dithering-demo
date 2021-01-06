/* greyscale.h -- Various greyscale transformations. */
#ifndef IMAGE_GREYSCALE_H
#define IMAGE_GREYSCALE_H

#include "image.h"

/* All procedures return a newly-allocated image. */

/* ITU-R BT.601-7 */
Image *Image_greyscale_bt601_rgb24_y8(Image *);

/* ITU-R BT.709-6 */
Image *Image_greyscale_bt709_rgb24_y8(Image *);

/* Average formula */
Image *Image_greyscale_avg_rgb24_y8(Image *);

#endif
