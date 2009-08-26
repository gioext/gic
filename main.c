#include "gic.h"
#include "gic_resize.h"

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: gic $jpeg $size");
        exit(1);
    }

    GIC_IMAGE *img = gic_jpeg_open(argv[1]);

    double scale = atof(argv[2]) / (img->width > img->height ? img->width : img->height);

    GIC_IMAGE *result = gic_resize_nearest_neighbour(img, scale);

    gic_write_image(result, "test.jpg");
    gic_jpeg_free(img);
    gic_jpeg_free(result);

    return 0;
}
