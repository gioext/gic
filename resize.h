#ifndef GIC_RESIZE_H
#define GIC_RESIZE_H

#include <omp.h>
#include "gic.h"

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
