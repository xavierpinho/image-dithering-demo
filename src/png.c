#include "png.h"
#include "image.h"
#include <assert.h>
#include <png.h>
#include <stdio.h>

Image *PNG_import_RGB24(FILE *f) {
  assert(NULL != f);

  png_struct *read =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (NULL == read) {
    return NULL;
  }

  png_info *info = png_create_info_struct(read);
  if (NULL == info) {
    return NULL;
  }

  unsigned char hdr[8];
  fread(hdr, sizeof(unsigned char), 8, f);
  if (png_sig_cmp(hdr, 0, 8)) {
    png_destroy_read_struct(&read, &info, NULL);
    return NULL;
  }

  png_init_io(read, f);
  png_set_sig_bytes(read, 8);
  png_read_info(read, info);
  png_set_strip_alpha(read);

  Image_Dx dx = png_get_image_width(read, info);
  Image_Dy dy = png_get_image_height(read, info);

  Image *m = Image_new(dx, dy, 3);
  if (NULL == m) {
    png_destroy_read_struct(&read, &info, NULL);
    return NULL;
  }

  png_set_palette_to_rgb(read);
  png_read_image(read, Image_get_rows(m));

  png_destroy_read_struct(&read, &info, NULL);
  return m;
}
