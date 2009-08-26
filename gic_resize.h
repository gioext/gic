#ifndef GIC_RESIZE_H
#define GIC_RESIZE_H

#include "gic.h"


int
gic_resize_nearest_neighbour(GIC_IMAGE *img, double scale);

int
gic_resize_area_average(GIC_IMAGE *img, double scale);


#endif
