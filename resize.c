#include "resize.h"

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

static double
sinc(double x)
{
    return sin(PI * x) / (PI * x);
}

static double
lanczos(double d, double n)
{
    double x = fabs(d);
    if (x < 0.00000001) {
        return 1.0;
    } else if (x >= n) {
        return 0.0;
    } else {
        return sinc(d) * sinc(d / n);
    }
}

static int
filter(int x, int total)
{
    if (total != 0) {
        x /= total;
    }
    if (x < 0) {
        return 0;
    } else if (x > 255) {
        return 255;
    } else {
        return x;
    }
}

GIC_IMAGE *
gic_resize_lanczos(GIC_IMAGE *img, double scale, int n)
{
    GIC_IMAGE *to_img;
    int from_width, from_height, to_width, to_height;

    from_width = img->width;
    from_height = img->height;
    to_width = from_width * scale;
    to_height = from_height * scale;
    to_img = gic_create_image(to_width, to_height);

    #pragma omp parallel num_threads(4)
    {
        int r, g, b;
        int w, h, x, y;

        double x0, y0;
        int x_bottom, x_top, y_bottom, y_top;
        double weight_total;

        #pragma omp for
        for (h = 0; h < to_height; h++) {
            for (w = 0; w < to_width; w++) {
                r = 0;
                g = 0;
                b = 0;

                x0 = w + 0.5;
                y0 = h + 0.5;

                x_bottom = (x0 - n) / scale;
                x_top = (x0 + n) / scale;
                y_bottom = (y0 - n) / scale;
                y_top = (y0 + n) / scale;
                weight_total = 0.0;
                
                for (y = y_bottom; y <= y_top; y++) {
                    if (y < 0 || y >= from_height) {
                        continue;
                    }
                    double yl = fabs(((y + 0.5) * scale) - y0);
                    double lanczos_y = lanczos(yl, n);
                    for (x = x_bottom; x <= x_top; x++) {
                        if (x < 0 || x >= from_width) {
                            continue;
                        }
                        double xl = fabs(((x + 0.5) * scale) - x0);
                        double lanczos_x = lanczos(xl, n);
                        double weight = lanczos_x * lanczos_y;
                        r += img->data[y][x * 3 + 0] * weight;
                        g += img->data[y][x * 3 + 1] * weight;
                        b += img->data[y][x * 3 + 2] * weight;
                        weight_total += weight;
                    }
                }

                r = filter(r, weight_total);
                g = filter(g, weight_total);
                b = filter(b, weight_total);

                to_img->data[h][w * 3 + 0] = r;
                to_img->data[h][w * 3 + 1] = g;
                to_img->data[h][w * 3 + 2] = b;
            }
        }
    }

    return to_img;
}

