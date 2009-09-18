#include "gic.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: gic $from $to $size\n");
        exit(1);
    }
    clock_t before, after;
    double scale1;
    GIC_IMAGE *img;
    GIC_IMAGE *thumb;


    before = clock();
    img = gic_jpeg_open(argv[1]);
    if (!img) {
        fprintf(stderr, "failed open jpeg file\n");
        exit(1);
    }
    after = clock();
    printf("open: %lf\n", (double)(after - before) / CLOCKS_PER_SEC);

    scale1 = atof(argv[2]) / (img->width > img->height ? img->width : img->height);

    before = clock();
    thumb = gic_resize_area_average2(img, scale1);
    gic_write_image(thumb, "thumb.jpg", 90);
    after = clock();
    printf("area average2: %lf\n", (double)(after - before) / CLOCKS_PER_SEC);
    gic_jpeg_free(thumb);
    gic_jpeg_free(img);

    sleep(100);

    return 0;
}
