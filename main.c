#include "gic.h"
#include "gic_resize.h"

int
main(int argc, char *argv[])
{
    GIC_IMAGE img;
    gic_jpeg_open(argv[1], &img);
    gic_resize_nearest_neighbour(&img, 0.5);

    return 0;
}
