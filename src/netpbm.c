#include "netpbm.h"
#include "image.h"
#include <assert.h>
#include <stdio.h>

Image *Netpbm_P3_import_RGB24(FILE *f) {
  assert(NULL != f);

  unsigned char h0, h1;
  if (2 != fscanf(f, "%c%c", &h0, &h1) || 'P' != h0 || '3' != h1) {
    return NULL;
  }

  Image_Dx dx;
  Image_Dy dy;
  if (2 != fscanf(f, "%ld %ld", &dx, &dy)) {
    return NULL;
  }

  unsigned int mpixval;
  if (1 != fscanf(f, "%u", &mpixval) || 255 != mpixval) {
    return NULL;
  }

  Image *m = Image_new(dx, dy, 3);
  if (NULL == m) {
    return NULL;
  }

  for (Image_Dy y = 0; y < dy; y++) {
    for (Image_Dx x = 0; x < dx; x++) {
      unsigned int r, g, b;
      if (3 != fscanf(f, "%u %u %u", &r, &g, &b) || mpixval < r ||
          mpixval < g || mpixval < b) {
        Image_free(m);
        return NULL;
      }

      Image_Pixel rgb[3] = {r, g, b};
      Image_set_pixel_at(m, x, y, rgb);
    }
  }
  return m;
}

Image *Netpbm_P2_import_Y8(FILE *f) {
  assert(NULL != f);

  unsigned char h0, h1;
  if (2 != fscanf(f, "%c%c", &h0, &h1) || 'P' != h0 || '2' != h1) {
    return NULL;
  }

  Image_Dx dx;
  Image_Dy dy;
  if (2 != fscanf(f, "%ld %ld", &dx, &dy)) {
    return NULL;
  }

  unsigned int mpixval;
  if (1 != fscanf(f, "%u", &mpixval) || 255 != mpixval) {
    return NULL;
  }

  Image *m = Image_new(dx, dy, 1);
  if (NULL == m) {
    return NULL;
  }

  for (Image_Dy y = 0; y < dy; y++) {
    for (Image_Dx x = 0; x < dx; x++) {
      unsigned int g;
      if (1 != fscanf(f, "%u", &g) || mpixval < g) {
        Image_free(m);
        return NULL;
      }
      Image_set_pixel_at(m, x, y, (Image_Pixel *)&g);
    }
  }
  return m;
}

void Netpbm_P3_export_RGB24(FILE *f, Image *m) {
  assert(NULL != f);
  assert(NULL != m);
  assert(3 == Image_get_pixel_depth(m));

  Image_Dx dx = Image_get_dx(m);
  Image_Dy dy = Image_get_dy(m);
  fprintf(f, "P3\n%ld %ld\n255\n", dx, dy);
  for (Image_Dy y = 0; y < dy; y++) {
    for (Image_Dx x = 0; x < dx; x++) {
      Image_Pixel *rgb = Image_get_pixel_at(m, x, y);
      fprintf(f, "%d %d %d\n", rgb[0], rgb[1], rgb[2]);
    }
  }
}

void Netpbm_P2_export_Y8(FILE *f, Image *m) {
  assert(NULL != f);
  assert(NULL != m);
  assert(1 == Image_get_pixel_depth(m));

  Image_Dx dx = Image_get_dx(m);
  Image_Dy dy = Image_get_dy(m);
  fprintf(f, "P2\n%ld %ld\n255\n", dx, dy);
  for (Image_Dy y = 0; y < dy; y++) {
    for (Image_Dx x = 0; x < dx; x++) {
      Image_Pixel *g = Image_get_pixel_at(m, x, y);
      assert(NULL != g);
      fprintf(f, "%d\n", *g);
    }
  }
}

void Netpbm_P1_export_Y1(FILE *f, Image *m) {
  assert(NULL != f);
  assert(NULL != m);
  assert(1 == Image_get_pixel_depth(m));

  Image_Dx dx = Image_get_dx(m);
  Image_Dy dy = Image_get_dy(m);
  fprintf(f, "P1\n%ld %ld\n1", dx, dy);
  for (Image_Dy y = 0; y < dy; y++) {
    for (Image_Dx x = 0; x < dx; x++) {
      Image_Pixel *g = Image_get_pixel_at(m, x, y);
      fprintf(f, "%d\n", 1 - *g);
    }
  }
}
