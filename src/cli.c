/* cli.c -- Demo program. */
#include "dither.h"
#include "greyscale.h"
#include "image.h"
#include "netpbm.h"
#include "png.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* Available options */
enum {
  PNG_TO_PPM_IDENTITY,
  PPM_TO_PGM_GREYSCALE_BT601,
  PPM_TO_PGM_GREYSCALE_BT709,
  PPM_TO_PGM_GREYSCALE_AVERAGE,
  PGM_TO_PBM_THRESHOLD,
  PGM_TO_PBM_THRESHOLD_RANDOM,
  PGM_TO_PBM_DITHER_2,
  PGM_TO_PBM_DITHER_4,
  PGM_TO_PBM_DITHER_8,
  PGM_TO_PBM_DITHER_FLOYDSTEINBERG,
  PGM_TO_PBM_DITHER_JARVISJUDICENINKE,
  PGM_TO_PBM_DITHER_STUCKI,
  PGM_TO_PBM_DITHER_BURKES,
  PGM_TO_PBM_DITHER_SIERRA,
  PGM_TO_PBM_DITHER_SIERRATWOROW,
  PGM_TO_PBM_DITHER_SIERRALITE,
  PGM_TO_PBM_DITHER_ATKINSON,
} CLI_OPTIONS;

static void print_usage(char **argv) {
  fprintf(stdout,
          "Usage: %s <Option> <Input file> <Output file>\n"
          "<Option>:\n"
          "  %d\t PNG to PPM conversion\n"
          "  %d\t PPM to PGM using ITU-R BT.601-7 formula\n"
          "  %d\t PPM to PGM using ITU-R BT.709-6 formula\n"
          "  %d\t PPM to PGM using average formula\n"
          "  %d\t PGM to PBM using fixed threshold dithering\n"
          "  %d\t PGM to PBM using random threshold dithering\n"
          "  %d\t PGM to PBM using 2x2 ordered dithering\n"
          "  %d\t PGM to PBM using 4x4 ordered dithering\n"
          "  %d\t PGM to PBM using 8x8 ordered dithering\n"
          "  %d\t PGM to PBM using Floyd-Steinberg dithering\n"
          "  %d\t PGM to PBM using Jarvis-Judice-Ninke dithering\n"
          "  %d\t PGM to PBM using Stucki dithering\n"
          "  %d\t PGM to PBM using Burkes dithering\n"
          "  %d\t PGM to PBM using Sierra dithering\n"
          "  %d\t PGM to PBM using 2-row Sierra dithering\n"
          "  %d\t PGM to PBM using Sierra lite dithering\n"
          "  %d\t PGM to PBM using Atkinson dithering\n",
          argv[0], PNG_TO_PPM_IDENTITY, PPM_TO_PGM_GREYSCALE_BT601,
          PPM_TO_PGM_GREYSCALE_BT709, PPM_TO_PGM_GREYSCALE_AVERAGE,
          PGM_TO_PBM_THRESHOLD, PGM_TO_PBM_THRESHOLD_RANDOM,
          PGM_TO_PBM_DITHER_2, PGM_TO_PBM_DITHER_4, PGM_TO_PBM_DITHER_8,
          PGM_TO_PBM_DITHER_FLOYDSTEINBERG, PGM_TO_PBM_DITHER_JARVISJUDICENINKE,
          PGM_TO_PBM_DITHER_STUCKI, PGM_TO_PBM_DITHER_BURKES,
          PGM_TO_PBM_DITHER_SIERRA, PGM_TO_PBM_DITHER_SIERRATWOROW,
          PGM_TO_PBM_DITHER_SIERRALITE, PGM_TO_PBM_DITHER_ATKINSON);
}

static Image *id(Image *m) { return m; }
static int run(FILE *in, FILE *out, Image *(read)(FILE *),
               Image *(transform)(Image *), void(write)(FILE *, Image *)) {

  Image *m = read(in);
  if (NULL == m) {
    fprintf(stderr, "Can't decode image\n");
    return EXIT_FAILURE;
  }

  Image *mm = transform(m);
  if (NULL == mm) {
    fprintf(stderr, "Can't apply transformation\n");
    return EXIT_FAILURE;
  }

  write(out, mm);
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {

  if (4 != argc) {
    print_usage(argv);
    return EXIT_FAILURE;
  }

  FILE *in = fopen(argv[2], "r");
  if (NULL == in) {
    fprintf(stderr, "Can't open file '%s'\n", argv[2]);
    return EXIT_FAILURE;
  }

  FILE *out = fopen(argv[3], "w");
  if (NULL == out) {
    fprintf(stderr, "Can't create file '%s'\n", argv[3]);
    return EXIT_FAILURE;
  }

  switch (atoi(argv[1])) {
  case PNG_TO_PPM_IDENTITY:
    return run(in, out, PNG_import_RGB24, id, Netpbm_P3_export_RGB24);
  case PPM_TO_PGM_GREYSCALE_AVERAGE:
    return run(in, out, Netpbm_P3_import_RGB24, Image_greyscale_avg_rgb24_y8,
               Netpbm_P2_export_Y8);
  case PPM_TO_PGM_GREYSCALE_BT601:
    return run(in, out, Netpbm_P3_import_RGB24, Image_greyscale_bt601_rgb24_y8,
               Netpbm_P2_export_Y8);
  case PPM_TO_PGM_GREYSCALE_BT709:
    return run(in, out, Netpbm_P3_import_RGB24, Image_greyscale_bt709_rgb24_y8,
               Netpbm_P2_export_Y8);
  case PGM_TO_PBM_THRESHOLD:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_threshold_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_THRESHOLD_RANDOM:
    return run(in, out, Netpbm_P2_import_Y8,
               Image_dither_threshold_random_y8_y1, Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_2:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_ordered2_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_4:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_ordered4_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_8:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_ordered8_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_FLOYDSTEINBERG:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_floydsteinberg_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_JARVISJUDICENINKE:
    return run(in, out, Netpbm_P2_import_Y8,
               Image_dither_jarvisjudiceninke_y8_y1, Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_ATKINSON:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_atkinson_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_BURKES:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_burkes_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_SIERRA:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_sierra_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_SIERRATWOROW:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_sierratworow_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_SIERRALITE:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_sierralite_y8_y1,
               Netpbm_P1_export_Y1);
  case PGM_TO_PBM_DITHER_STUCKI:
    return run(in, out, Netpbm_P2_import_Y8, Image_dither_stucki_y8_y1,
               Netpbm_P1_export_Y1);
  default:
    fprintf(stderr, "Invalid option choice\n");
    return EXIT_FAILURE;
  }
}
