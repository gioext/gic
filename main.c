#include "gic.h"
#include "resize.h"

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: gic $jpeg\n");
        exit(1);
    }

    GIC_IMAGE *img = gic_jpeg_open(argv[1]);
    double scale1 = 240.0 / (img->width > img->height ? img->width : img->height);

    GIC_IMAGE *thumb = gic_resize_lanczos(img, scale1, 2);
    //GIC_IMAGE *result = gic_resize_nearest_neighbour(img, scale);
    gic_write_image(thumb, "thumb.jpg");
    gic_jpeg_free(thumb);

    gic_jpeg_free(img);

    return 0;
}
