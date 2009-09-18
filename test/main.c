#include "gic.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void
do_resize(GIC_IMAGE *img, double scale)
{
    GIC_IMAGE *thumb;

    thumb = gic_resize_area_average2(img, scale);
    gic_write_image(thumb, "thumb.jpg", 95);

    gic_jpeg_free(thumb);
}

int
main(int argc, char *argv[])
{
    int i;
    double scale1, scale2, scale3;
    clock_t before, after;
    GIC_IMAGE *img;

    before = clock();
    for (i = 0; i < 200000; i++) {
        if ((i % 3) == 0) {
            img = gic_jpeg_open("a.jpg");
        } else if ((i % 3) == 1) {
            img = gic_jpeg_open("b.jpg");
        } else {
            img = gic_jpeg_open("c.jpg");
        }
        scale1 = 120.0 / (img->width > img->height ? img->width : img->height);
        scale2 = 240.0 / (img->width > img->height ? img->width : img->height);
        scale3 = 800.0 / (img->width > img->height ? img->width : img->height);
        do_resize(img, scale1);
        do_resize(img, scale2);
        do_resize(img, scale3);
        gic_jpeg_free(img);
    }
    after = clock();
    printf("do_resize: %lf\n", (double)(after - before) / CLOCKS_PER_SEC);

    sleep(100);
    return 0;
}
