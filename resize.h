#ifndef GIC_RESIZE_H
#define GIC_RESIZE_H

#include <omp.h>
#include "gic.h"

#define PI        3.141592

GIC_IMAGE *
gic_resize_nearest_neighbour(GIC_IMAGE *img, double scale);

int
gic_resize_area_average(GIC_IMAGE *img, int to_width, int to_height);

int
gic_resize_bilinear(GIC_IMAGE *img, int to_width, int to_height);

GIC_IMAGE *
gic_resize_lanczos(GIC_IMAGE *img, double scale, int n);

#endif
