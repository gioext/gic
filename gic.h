#ifndef GIC_H
#define GIC_H

#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <math.h>
#include <omp.h>

typedef struct gic_image {
    JSAMPARRAY data;
    int width;
    int height;
} GIC_IMAGE;

GIC_IMAGE *
gic_create_image(int width, int height);


GIC_IMAGE *
gic_jpeg_open(char *filename);


int
gic_jpeg_free(GIC_IMAGE *img);


int
gic_write_ppm(char *infile, char *outfile);


int
gic_write_image(GIC_IMAGE *img, char *filename, int quality);

#define PI             3.141592
#define MAX_SPLIT_SIZE 10
#define DISCRETE_SIZE  3000

GIC_IMAGE *
gic_resize_nearest_neighbour(GIC_IMAGE *img, double scale);

GIC_IMAGE *
gic_resize_area_average1(GIC_IMAGE *img, double scale);

GIC_IMAGE *
gic_resize_area_average2(GIC_IMAGE *img, double scale);

GIC_IMAGE *
gic_resize_lanczos(GIC_IMAGE *img, double scale, int n);

#endif
