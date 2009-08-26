#include "gic.h"
#include "gic_resize.h"

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: gic $jpeg $size\n");
        exit(1);
    }

    GIC_IMAGE *img = gic_jpeg_open(argv[1]);

    double scale = atof(argv[2]) / (img->width > img->height ? img->width : img->height);
    printf("[source]\n");
    printf("  width:  %d\n", img->width);
    printf("  height: %d\n", img->height);
    printf("  scale:  %lf\n", scale);

    GIC_IMAGE *result = gic_resize_nearest_neighbour(img, scale);
    printf("[convert]\n");
    printf("  width:  %d\n", result->width);
    printf("  height: %d\n", result->height);

    gic_write_image(result, "test.jpg");
    gic_jpeg_free(img);
    gic_jpeg_free(result);

    return 0;
}
