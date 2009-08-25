#include <jpeglib.h>

#ifndef GIC_H
#define GIC_H

typedef struct gic_image {
    JSAMPARRAY data;
    int width;
    int height;
} GIC_IMAGE;



int
gic_jpeg_open(char *filename, GIC_IMAGE *img);


int
gic_jpeg_free(GIC_IMAGE *img);


int
gic_jpeg_free(GIC_IMAGE *img);


int
gic_write_ppm(char *infile, char *outfile);

#endif
