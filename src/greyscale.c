#include "greyscale.h"
#include "image.h"
#include <assert.h>

static Image *map_rgb24_y8(Image *in, Image_TraversalCallback f) {
  assert(NULL != in);
  Image *out = Image_new(Image_get_dx(in), Image_get_dy(in), 1);
  if (NULL == out) {
    return NULL;
  }
  return (Image *)Image_traverse_forward(in, f, out);
}

static void *bt601_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  Image_Pixel *rgb = Image_get_pixel_at(in, x, y);
  Image_Pixel y8 = 0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2];
  Image_set_pixel_at((Image *)out, x, y, &y8);
  return out;
}

static void *bt709_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  Image_Pixel *rgb = Image_get_pixel_at(in, x, y);
  Image_Pixel y8 = 0.2126 * rgb[0] + 0.7152 * rgb[1] + 0.0722 * rgb[2];
  Image_set_pixel_at((Image *)out, x, y, &y8);
  return out;
}

static void *average_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  Image_Pixel *rgb = Image_get_pixel_at(in, x, y);
  Image_Pixel y8 = 0.3333 * rgb[0] + 0.3333 * rgb[1] + 0.3333 * rgb[2];
  Image_set_pixel_at((Image *)out, x, y, &y8);
  return out;
}

Image *Image_greyscale_bt601_rgb24_y8(Image *in) {
  return map_rgb24_y8(in, bt601_cb);
}

Image *Image_greyscale_bt709_rgb24_y8(Image *in) {
  return map_rgb24_y8(in, bt709_cb);
}

Image *Image_greyscale_avg_rgb24_y8(Image *in) {
  return map_rgb24_y8(in, average_cb);
}
