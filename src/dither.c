#include "dither.h"
#include "image.h"
#include <assert.h>
#include <stdlib.h>

static Image *map_y8_y1(Image *in, Image_TraversalCallback f) {
  assert(NULL != in);
  Image *out = Image_new(Image_get_dx(in), Image_get_dy(in), 1);
  if (NULL == out) {
    return NULL;
  }
  return (Image *)Image_traverse_forward(in, f, out);
}

static void *clone_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  Image_set_pixel_at(out, x, y, Image_get_pixel_at(in, x, y));
  return out;
}

static Image *errdif_y8_y1(Image *in, Image_TraversalCallback f) {
  Image *out = map_y8_y1(in, clone_y8_cb);
  if (NULL == out) {
    return NULL;
  }
  return (Image *)Image_traverse_forward(out, f, out);
}

static void *threshold_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  Image_Pixel *y8 = Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = 127 < *y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);
  return out;
}

static void *threshold_rand_y8_cb(Image *in, Image_Dx x, Image_Dy y,
                                  void *out) {
  Image_Pixel *y8 = Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = (rand() % 255) < *y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);
  return out;
}

static void *ordered2_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  int M[2][2] = {{0, 3}, {2, 1}};
  Image_Pixel *y8 = Image_get_pixel_at(in, x, y);
  Image_Pixel h = M[x % 2][y % 2] * 256 / 4;
  Image_Pixel y1 = h < *y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);
  return out;
}

static void *ordered4_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  int M[4][4] = {{0, 12, 3, 15}, {8, 4, 11, 7}, {2, 14, 1, 13}, {10, 6, 9, 5}};
  Image_Pixel *y8 = Image_get_pixel_at(in, x, y);
  Image_Pixel h = M[x % 4][y % 4] * 256 / 16;
  Image_Pixel y1 = h < *y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);
  return out;
}

static void *ordered8_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  int M[8][8] = {
      {0, 32, 8, 40, 2, 34, 10, 42},  {48, 16, 56, 24, 50, 18, 58, 26},
      {12, 44, 4, 36, 14, 46, 6, 38}, {60, 28, 52, 20, 62, 30, 54, 22},
      {3, 35, 11, 43, 1, 33, 9, 41},  {51, 19, 59, 27, 49, 17, 57, 25},
      {15, 47, 7, 39, 13, 45, 5, 37}, {63, 31, 55, 23, 61, 29, 53, 21}};
  Image_Pixel *y8 = Image_get_pixel_at(in, x, y);
  Image_Pixel h = M[x % 8][y % 8] * 256 / 64;
  Image_Pixel y1 = h < *y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);
  return out;
}

Image *Image_dither_threshold_y8_y1(Image *in) {
  return map_y8_y1(in, threshold_y8_cb);
}

Image *Image_dither_threshold_random_y8_y1(Image *in) {
  return map_y8_y1(in, threshold_rand_y8_cb);
}

Image *Image_dither_ordered2_y8_y1(Image *in) {
  return map_y8_y1(in, ordered2_y8_cb);
}

Image *Image_dither_ordered4_y8_y1(Image *in) {
  return map_y8_y1(in, ordered4_y8_cb);
}

Image *Image_dither_ordered8_y8_y1(Image *in) {
  return map_y8_y1(in, ordered8_y8_cb);
}

static Image_Pixel wrap_y8(int v) { return v < 1 ? 0 : 254 < v ? 255 : v; }

static void *floydsteinberg_y8_cb(Image *in, Image_Dx x, Image_Dy y,
                                  void *out) {
  assert(in == out);
  Image_Pixel y8 = *Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = 127 < y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);

  int err = y8 - (y1 ? 255 : 0);

  Image_Dx dx = Image_get_dx(in);
  Image_Dy dy = Image_get_dy(in);

  if (x + 1 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y);
    Image_Pixel q = wrap_y8(p + err * 7 / 16);
    Image_set_pixel_at(in, x + 1, y, &q);
  }
  if (x > 0 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 3 / 16);
    Image_set_pixel_at(in, x - 1, y + 1, &q);
  }
  if (x < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 1);
    Image_Pixel q = wrap_y8(p + err * 5 / 16);
    Image_set_pixel_at(in, x, y + 1, &q);
  }
  if (x + 1 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 1 / 16);
    Image_set_pixel_at(in, x + 1, y + 1, &q);
  }
  return out;
}

Image *Image_dither_floydsteinberg_y8_y1(Image *in) {
  return errdif_y8_y1(in, floydsteinberg_y8_cb);
}

static void *jarvisjudiceninke_y8_cb(Image *in, Image_Dx x, Image_Dy y,
                                     void *out) {
  assert(in == out);
  Image_Pixel y8 = *Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = 127 < y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);

  int err = y8 - (y1 ? 255 : 0);

  Image_Dx dx = Image_get_dx(in);
  Image_Dy dy = Image_get_dy(in);

  if (x + 1 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y);
    Image_Pixel q = wrap_y8(p + err * 7 / 48);
    Image_set_pixel_at(in, x + 1, y, &q);
  }
  if (x + 2 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y);
    Image_Pixel q = wrap_y8(p + err * 5 / 48);
    Image_set_pixel_at(in, x + 2, y, &q);
  }
  if (x > 1 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 3 / 48);
    Image_set_pixel_at(in, x - 2, y + 1, &q);
  }
  if (x > 0 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 5 / 48);
    Image_set_pixel_at(in, x - 1, y + 1, &q);
  }
  if (y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 1);
    Image_Pixel q = wrap_y8(p + err * 7 / 48);
    Image_set_pixel_at(in, x, y + 1, &q);
  }
  if (x + 1 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 5 / 48);
    Image_set_pixel_at(in, x + 1, y + 1, &q);
  }
  if (x + 2 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 3 / 48);
    Image_set_pixel_at(in, x + 2, y + 1, &q);
  }
  if (x > 1 && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 2, y + 2);
    Image_Pixel q = wrap_y8(p + err * 1 / 48);
    Image_set_pixel_at(in, x - 2, y + 2, &q);
  }
  if (x > 0 && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 2);
    Image_Pixel q = wrap_y8(p + err * 3 / 48);
    Image_set_pixel_at(in, x - 1, y + 2, &q);
  }
  if (y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 2);
    Image_Pixel q = wrap_y8(p + err * 5 / 48);
    Image_set_pixel_at(in, x, y + 2, &q);
  }
  if (x + 1 < dx && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 2);
    Image_Pixel q = wrap_y8(p + err * 3 / 48);
    Image_set_pixel_at(in, x + 1, y + 2, &q);
  }
  if (x + 2 < dx && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y + 2);
    Image_Pixel q = wrap_y8(p + err * 1 / 48);
    Image_set_pixel_at(in, x + 2, y + 2, &q);
  }
  return out;
}

static void *stucki_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  assert(in == out);
  Image_Pixel y8 = *Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = 127 < y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);

  int err = y8 - (y1 ? 255 : 0);

  Image_Dx dx = Image_get_dx(in);
  Image_Dy dy = Image_get_dy(in);

  if (x + 1 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y);
    Image_Pixel q = wrap_y8(p + err * 8 / 42);
    Image_set_pixel_at(in, x + 1, y, &q);
  }
  if (x + 2 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y);
    Image_Pixel q = wrap_y8(p + err * 4 / 42);
    Image_set_pixel_at(in, x + 2, y, &q);
  }
  if (x > 1 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 2 / 42);
    Image_set_pixel_at(in, x - 2, y + 1, &q);
  }
  if (x > 0 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 4 / 42);
    Image_set_pixel_at(in, x - 1, y + 1, &q);
  }
  if (y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 1);
    Image_Pixel q = wrap_y8(p + err * 8 / 42);
    Image_set_pixel_at(in, x, y + 1, &q);
  }
  if (x + 1 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 4 / 42);
    Image_set_pixel_at(in, x + 1, y + 1, &q);
  }
  if (x + 2 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 2 / 42);
    Image_set_pixel_at(in, x + 2, y + 1, &q);
  }
  if (x > 1 && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 2, y + 2);
    Image_Pixel q = wrap_y8(p + err * 1 / 42);
    Image_set_pixel_at(in, x - 2, y + 2, &q);
  }
  if (x > 0 && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 2);
    Image_Pixel q = wrap_y8(p + err * 2 / 42);
    Image_set_pixel_at(in, x - 1, y + 2, &q);
  }
  if (y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 2);
    Image_Pixel q = wrap_y8(p + err * 4 / 42);
    Image_set_pixel_at(in, x, y + 2, &q);
  }
  if (x + 1 < dx && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 2);
    Image_Pixel q = wrap_y8(p + err * 2 / 42);
    Image_set_pixel_at(in, x + 1, y + 2, &q);
  }
  if (x + 2 < dx && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y + 2);
    Image_Pixel q = wrap_y8(p + err * 1 / 42);
    Image_set_pixel_at(in, x + 2, y + 2, &q);
  }
  return out;
}

static void *burkes_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  assert(in == out);
  Image_Pixel y8 = *Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = 127 < y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);

  int err = y8 - (y1 ? 255 : 0);

  Image_Dx dx = Image_get_dx(in);
  Image_Dy dy = Image_get_dy(in);

  if (x + 1 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y);
    Image_Pixel q = wrap_y8(p + err * 8 / 32);
    Image_set_pixel_at(in, x + 1, y, &q);
  }
  if (x + 2 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y);
    Image_Pixel q = wrap_y8(p + err * 4 / 32);
    Image_set_pixel_at(in, x + 2, y, &q);
  }
  if (x > 1 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 2 / 32);
    Image_set_pixel_at(in, x - 2, y + 1, &q);
  }
  if (x > 0 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 4 / 32);
    Image_set_pixel_at(in, x - 1, y + 1, &q);
  }
  if (y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 1);
    Image_Pixel q = wrap_y8(p + err * 8 / 32);
    Image_set_pixel_at(in, x, y + 1, &q);
  }
  if (x + 1 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 4 / 32);
    Image_set_pixel_at(in, x + 1, y + 1, &q);
  }
  if (x + 2 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 2 / 32);
    Image_set_pixel_at(in, x + 2, y + 1, &q);
  }
  return out;
}

static void *sierra_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  assert(in == out);
  Image_Pixel y8 = *Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = 127 < y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);

  int err = y8 - (y1 ? 255 : 0);

  Image_Dx dx = Image_get_dx(in);
  Image_Dy dy = Image_get_dy(in);

  if (x + 1 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y);
    Image_Pixel q = wrap_y8(p + err * 5 / 32);
    Image_set_pixel_at(in, x + 1, y, &q);
  }
  if (x + 2 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y);
    Image_Pixel q = wrap_y8(p + err * 3 / 32);
    Image_set_pixel_at(in, x + 2, y, &q);
  }
  if (x > 1 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 2 / 32);
    Image_set_pixel_at(in, x - 2, y + 1, &q);
  }
  if (x > 0 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 4 / 32);
    Image_set_pixel_at(in, x - 1, y + 1, &q);
  }
  if (y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 1);
    Image_Pixel q = wrap_y8(p + err * 5 / 32);
    Image_set_pixel_at(in, x, y + 1, &q);
  }
  if (x + 1 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 4 / 32);
    Image_set_pixel_at(in, x + 1, y + 1, &q);
  }
  if (x + 2 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 2 / 32);
    Image_set_pixel_at(in, x + 2, y + 1, &q);
  }
  if (x > 0 && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 2);
    Image_Pixel q = wrap_y8(p + err * 2 / 32);
    Image_set_pixel_at(in, x - 1, y + 2, &q);
  }
  if (y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 2);
    Image_Pixel q = wrap_y8(p + err * 3 / 32);
    Image_set_pixel_at(in, x, y + 2, &q);
  }
  if (x + 1 < dx && y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 2);
    Image_Pixel q = wrap_y8(p + err * 2 / 32);
    Image_set_pixel_at(in, x + 1, y + 2, &q);
  }
  return out;
}

static void *sierratworow_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  assert(in == out);
  Image_Pixel y8 = *Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = 127 < y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);

  int err = y8 - (y1 ? 255 : 0);

  Image_Dx dx = Image_get_dx(in);
  Image_Dy dy = Image_get_dy(in);

  if (x + 1 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y);
    Image_Pixel q = wrap_y8(p + err * 4 / 16);
    Image_set_pixel_at(in, x + 1, y, &q);
  }
  if (x + 2 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y);
    Image_Pixel q = wrap_y8(p + err * 3 / 16);
    Image_set_pixel_at(in, x + 2, y, &q);
  }
  if (x > 1 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 1 / 16);
    Image_set_pixel_at(in, x - 2, y + 1, &q);
  }
  if (x > 0 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 2 / 16);
    Image_set_pixel_at(in, x - 1, y + 1, &q);
  }
  if (y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 1);
    Image_Pixel q = wrap_y8(p + err * 3 / 16);
    Image_set_pixel_at(in, x, y + 1, &q);
  }
  if (x + 1 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 2 / 16);
    Image_set_pixel_at(in, x + 1, y + 1, &q);
  }
  if (x + 2 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y + 1);
    Image_Pixel q = wrap_y8(p + err * 1 / 16);
    Image_set_pixel_at(in, x + 2, y + 1, &q);
  }
  return out;
}

static void *sierralite_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  assert(in == out);
  Image_Pixel y8 = *Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = 127 < y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);

  int err = y8 - (y1 ? 255 : 0);

  Image_Dx dx = Image_get_dx(in);
  Image_Dy dy = Image_get_dy(in);

  if (x + 1 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y);
    Image_Pixel q = wrap_y8(p + err * 2 / 4);
    Image_set_pixel_at(in, x + 1, y, &q);
  }
  if (x > 0 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 1 / 4);
    Image_set_pixel_at(in, x - 1, y + 1, &q);
  }
  if (y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 1);
    Image_Pixel q = wrap_y8(p + err * 1 / 4);
    Image_set_pixel_at(in, x, y + 1, &q);
  }
  return out;
}

static void *atkinson_y8_cb(Image *in, Image_Dx x, Image_Dy y, void *out) {
  assert(in == out);
  Image_Pixel y8 = *Image_get_pixel_at(in, x, y);
  Image_Pixel y1 = 127 < y8;
  Image_set_pixel_at((Image *)out, x, y, &y1);

  int err = y8 - (y1 ? 255 : 0);

  Image_Dx dx = Image_get_dx(in);
  Image_Dy dy = Image_get_dy(in);

  if (x + 1 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y);
    Image_Pixel q = wrap_y8(p + err * 1 / 8);
    Image_set_pixel_at(in, x + 1, y, &q);
  }
  if (x + 2 < dx) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 2, y);
    Image_Pixel q = wrap_y8(p + err * 1 / 8);
    Image_set_pixel_at(in, x + 2, y, &q);
  }
  if (x > 0 && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x - 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 1 / 8);
    Image_set_pixel_at(in, x - 1, y + 1, &q);
  }
  if (y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 1);
    Image_Pixel q = wrap_y8(p + err * 1 / 8);
    Image_set_pixel_at(in, x, y + 1, &q);
  }
  if (x + 1 < dx && y + 1 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x + 1, y + 1);
    Image_Pixel q = wrap_y8(p + err * 1 / 8);
    Image_set_pixel_at(in, x + 1, y + 1, &q);
  }
  if (y + 2 < dy) {
    Image_Pixel p = *Image_get_pixel_at(in, x, y + 2);
    Image_Pixel q = wrap_y8(p + err * 1 / 8);
    Image_set_pixel_at(in, x, y + 2, &q);
  }
  return out;
}

Image *Image_dither_jarvisjudiceninke_y8_y1(Image *in) {
  return errdif_y8_y1(in, jarvisjudiceninke_y8_cb);
}

Image *Image_dither_stucki_y8_y1(Image *in) {
  return errdif_y8_y1(in, stucki_y8_cb);
}

Image *Image_dither_burkes_y8_y1(Image *in) {
  return errdif_y8_y1(in, burkes_y8_cb);
}

Image *Image_dither_sierra_y8_y1(Image *in) {
  return errdif_y8_y1(in, sierra_y8_cb);
}

Image *Image_dither_sierratworow_y8_y1(Image *in) {
  return errdif_y8_y1(in, sierratworow_y8_cb);
}

Image *Image_dither_sierralite_y8_y1(Image *in) {
  return errdif_y8_y1(in, sierralite_y8_cb);
}

Image *Image_dither_atkinson_y8_y1(Image *in) {
  return errdif_y8_y1(in, atkinson_y8_cb);
}
