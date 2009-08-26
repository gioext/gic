#include "gic_resize.h"

static GIC_IMAGE *
gic_create_image(int width, int height)
{
    int i;
    GIC_IMAGE *img = (GIC_IMAGE *)malloc(sizeof(GIC_IMAGE));
    img->width = width;
    img->height = height;
    img->data = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * height);
    for (i = 0; i < width; i++) {
        img->data[i] = (JSAMPROW)malloc(sizeof(JSAMPLE) * 3 * width);
    }

    return img;
}


GIC_IMAGE *
gic_resize_nearest_neighbour(GIC_IMAGE *img, double scale)
{
    int x, y;
    int xp, yp;
    int r, g, b;
    int to_width, to_height;
    to_width = img->width * scale;
    to_height = img->height * scale;

    GIC_IMAGE *to_img = gic_create_image(to_width, to_height);

    for (y = 0; y < to_height; y++) {
        for (x = 0; x < to_width; x++) {
            xp = (int)(x / scale);
            yp = (int)(y / scale);

            r = img->data[yp][xp * 3 + 0];
            g = img->data[yp][xp * 3 + 1];
            b = img->data[yp][xp * 3 + 2];

            to_img->data[y][x * 3 + 0] = r;
            to_img->data[y][x * 3 + 1] = g;
            to_img->data[y][x * 3 + 2] = b;
        }
    }

    return to_img;
}

int
gic_resize_area_average(GIC_IMAGE *img, int to_width, int to_height)
{
    return 0;
}

int
gic_resize_bilinear(GIC_IMAGE *img, int to_width, int to_height)
{
    return 0;
}
