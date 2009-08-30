#include "gic.h"
#include "resize.h"
#include <time.h>

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Usage: gic $from $to $size\n");
        exit(1);
    }
    clock_t before, after;
    double scale1;
    GIC_IMAGE *img;
    GIC_IMAGE *thumb;


    before = clock();
    img = gic_jpeg_open(argv[1]);
    after = clock();
    printf("open: %lf\n", (double)(after - before) / CLOCKS_PER_SEC);

    scale1 = atof(argv[3]) / (img->width > img->height ? img->width : img->height);


    before = clock();
    thumb = gic_resize_lanczos(img, scale1, 2);
    after = clock();
    printf("lanczos-2: %lf\n", (double)(after - before) / CLOCKS_PER_SEC);


    before = clock();
    thumb = gic_resize_nearest_neighbour(img, scale1);
    after = clock();
    printf("nearest neighbour: %lf\n", (double)(after - before) / CLOCKS_PER_SEC);


    before = clock();
    thumb = gic_resize_area_average(img, scale1);
    after = clock();
    printf("area average: %lf\n", (double)(after - before) / CLOCKS_PER_SEC);


    before = clock();
    gic_write_image(thumb, argv[2], 90);
    after = clock();
    printf("write: %lf\n", (double)(after - before) / CLOCKS_PER_SEC);



    gic_jpeg_free(thumb);
    gic_jpeg_free(img);

    return 0;
}
