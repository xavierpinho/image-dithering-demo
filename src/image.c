#include "image.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct Image {
  Image_Dx dx;
  Image_Dy dy;
  Image_PixelDepth pixdepth;
  Image_Pixel **pix;
};

Image *Image_new(Image_Dx dx, Image_Dy dy, Image_PixelDepth pixdepth) {
  Image *m = malloc(sizeof(struct Image));
  if (NULL == m) {
    return NULL;
  }

  Image_Pixel **pix = malloc(sizeof(Image_Pixel *) * dy);
  if (NULL == pix) {
    free(m);
    return NULL;
  }

  for (Image_Dy y = 0; y < dy; y++) {
    pix[y] = malloc(pixdepth * dx);
    if (NULL == pix[y]) {
      for (; 0 < y; y--) {
        free(pix[y]);
      }
      free(pix);
      free(m);
      return NULL;
    }
  }

  m->dx = dx;
  m->dy = dy;
  m->pixdepth = pixdepth;
  m->pix = pix;
  return m;
}

void Image_free(Image *m) {
  assert(NULL != m);
  free(m->pix);
  free(m);
}

Image_PixelDepth Image_get_pixel_depth(Image *m) {
  assert(NULL != m);
  return m->pixdepth;
}

Image_Dx Image_get_dx(Image *m) {
  assert(NULL != m);
  return m->dx;
}

Image_Dy Image_get_dy(Image *m) {
  assert(NULL != m);
  return m->dy;
}

Image_Pixel *Image_get_pixel_at(Image *m, Image_Dx x, Image_Dy y) {
  assert(NULL != m);
  assert(NULL != m->pix);
  assert(x < Image_get_dx(m));
  assert(y < Image_get_dy(m));
  return m->pix[y] + (x * m->pixdepth);
}

void Image_set_pixel_at(Image *m, Image_Dx x, Image_Dy y, Image_Pixel *p) {
  assert(NULL != m);
  assert(NULL != m->pix);
  assert(x < Image_get_dx(m));
  assert(y < Image_get_dy(m));
  memcpy(m->pix[y] + (x * m->pixdepth), p, m->pixdepth);
}

Image_Pixel **Image_get_rows(Image *m) {
  assert(NULL != m);
  return m->pix;
}

Image *Image_clone(Image *m) {
  assert(NULL != m);
  Image_Dx dx = Image_get_dx(m);
  Image_Dy dy = Image_get_dy(m);
  Image_PixelDepth depth = Image_get_pixel_depth(m);
  Image *out = Image_new(dx, dy, depth);
  if (NULL == out) {
    return NULL;
  }
  for (Image_Dy y = 0; y < dy; y++) {
    for (Image_Dx x = 0; x < dx; x++) {
      Image_set_pixel_at(out, x, y, Image_get_pixel_at(m, x, y));
    }
  }
  return out;
}

void *Image_traverse_forward(Image *m, Image_TraversalCallback f, void *data) {
  assert(NULL != m);
  Image_Dx dx = Image_get_dx(m);
  Image_Dy dy = Image_get_dy(m);
  for (Image_Dy y = 0; y < dy; y++) {
    for (Image_Dx x = 0; x < dx; x++) {
      data = f(m, x, y, data);
    }
  }
  return data;
}
