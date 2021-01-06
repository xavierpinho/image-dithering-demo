/* image.h -- Image representation and manipulation procedures. */
#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>

/* An image is a rectangular grid of pixels. */
typedef struct Image Image;
typedef size_t Image_Dx;
typedef size_t Image_Dy;

/* A pixel is a fixed-size (cf. Image_PixelDepth) string of bytes. */
typedef unsigned char Image_Pixel;

/* The size (in bytes) of each pixel. */
typedef size_t Image_PixelDepth;

/* Allocates an image with the given dimensions and pixel depth. */
Image *Image_new(Image_Dx, Image_Dy, Image_PixelDepth);

/* Returns a new copy of a given image. */
Image *Image_clone(Image *);

/* Deallocates a previously allocated image. */
void Image_free(Image *);

/* Return the image's pixel depth. */
Image_PixelDepth Image_get_pixel_depth(Image *);

/* Returns the image's dimensions. */
Image_Dx Image_get_dx(Image *);
Image_Dy Image_get_dy(Image *);

/* Returns the image's pixel at the given coordinates. */
Image_Pixel *Image_get_pixel_at(Image *, Image_Dx, Image_Dy);

/* Updates the image's pixel at the given coordinates. */
void Image_set_pixel_at(Image *, Image_Dx, Image_Dy, Image_Pixel *);

/* Returns the internal array of pointers for each row. */
Image_Pixel **Image_get_rows(Image *);

/* Traverses an image invoking a given callback on every pixel. */
typedef void *(*Image_TraversalCallback)(Image *, Image_Dx, Image_Dy, void *);

/* Top-bottom left-right traversal. */
void *Image_traverse_forward(Image *, Image_TraversalCallback, void *);

#endif
