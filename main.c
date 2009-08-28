#include "gic.h"
#include "resize.h"

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Usage: gic $from $to $size\n");
        exit(1);
    }

    GIC_IMAGE *img = gic_jpeg_open(argv[1]);
    double scale1 = atof(argv[3]) / (img->width > img->height ? img->width : img->height);

    GIC_IMAGE *thumb = gic_resize_lanczos(img, scale1, 3);
    //GIC_IMAGE *result = gic_resize_nearest_neighbour(img, scale);
    gic_write_image(thumb, argv[2], 90);
    gic_jpeg_free(thumb);

    gic_jpeg_free(img);

    return 0;
}
