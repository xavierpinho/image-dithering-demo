/* netpbm.h -- Netpbm file encoding/decoding procedures. */
#ifndef IMAGE_NETPBM_H
#define IMAGE_NETPBM_H

#include "image.h"
#include <stdio.h>

Image *Netpbm_P3_import_RGB24(FILE *);
Image *Netpbm_P2_import_Y8(FILE *);
Image *Netpbm_P1_import_Y1(FILE *);

void Netpbm_P3_export_RGB24(FILE *, Image *);
void Netpbm_P2_export_Y8(FILE *, Image *);
void Netpbm_P1_export_Y1(FILE *, Image *);

#endif
