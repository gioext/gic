#include "gic_resize.h"

int
gic_resize_nearest_neighbour(GIC_IMAGE *img, double scale)
{
    int x, y, i;
    int to_width = (int)(img->width * scale);
    int to_height = (int)(img->height * scale);

    // create GIC_IMAGE
    GIC_IMAGE to_img;
    to_img.width = to_width;
    to_img.height = to_height;
    to_img.data = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * to_height);
    for (i = 0; i < to_width; i++) {
        to_img.data[i] = (JSAMPROW)malloc(sizeof(JSAMPLE) * 3 * to_width);
    }

    for (y = 0; y < to_height; y++) {
        for (x = 0; x < to_width; x++) {
            int xp = (int)(x / scale);
            int yp = (int)(y / scale);

            int r = img->data[yp][xp * 3 + 0];
            int g = img->data[yp][xp * 3 + 1];
            int b = img->data[yp][xp * 3 + 2];

            to_img.data[y][x * 3 + 0] = r;
            to_img.data[y][x * 3 + 1] = g;
            to_img.data[y][x * 3 + 2] = b;
        }
    }

    gic_write_image(&to_img, "test.jpg");
    gic_jpeg_free(&to_img);
    return 0;
}

int
gic_resize_area_average(GIC_IMAGE *img, double scale)
{
    return 0;
}
