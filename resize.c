#include "resize.h"


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

static int
gcd(int x, int y)
{
    int max = x > y ? x : y;
    int min = x < y ? x : y;

    if (min == 0) {
        return max;
    }

    while(1) {
        int n = max % min;
        if (n == 0) {
            return min;
        }
        max = min;
        min = n;
    }
}

static int
lcm(int x, int y)
{
    return (x * y) / gcd(x, y);
}

#define MAX_SPLIT_SIZE 5

GIC_IMAGE *
gic_resize_area_average(GIC_IMAGE *img, double scale)
{
    int w, h;
    int to_width, to_height, from_width, from_height;
    from_width = img->width;
    from_height = img->height;
    to_width = img->width * scale;
    to_height = img->height * scale;
    GIC_IMAGE *to_img = gic_create_image(to_width, to_height);

    int lcmx = lcm(to_width, from_width);
    int lcmy = lcm(to_height, from_height);
    if (lcmx > MAX_SPLIT_SIZE * to_width) {
        lcmx = MAX_SPLIT_SIZE * to_width;
    }
    if (lcmy > MAX_SPLIT_SIZE * to_height) {
        lcmy = MAX_SPLIT_SIZE * to_height;
    }
    double sx = (double)from_width / lcmx;
    double sy = (double)from_height / lcmy;
    double dx = (double)to_width / lcmx;
    double dy = (double)to_height / lcmy;
    double dxy = dx * dy;

    double *R = (double *)calloc(sizeof(double), to_width * to_height);
    double *G = (double *)calloc(sizeof(double), to_width * to_height);
    double *B = (double *)calloc(sizeof(double), to_width * to_height);


    for (h = 0; h < lcmy; h++) {
        int fy = (int)(h * sy);
        int ty = (int)(h * dy);
        int tyw = ty * to_width;
        for (w = 0; w < lcmx; w++) {
            int fx = (int)(w * sx);
            int tx = (int)(w * dx);

            double r = img->data[fy][fx * 3 + 0] * dxy;
            double g = img->data[fy][fx * 3 + 1] * dxy;
            double b = img->data[fy][fx * 3 + 2] * dxy;

            R[tyw + tx] += r;
            G[tyw + tx] += g;
            B[tyw + tx] += b;
        }
    }

    for (h = 0; h < to_height; h++) {
        for (w = 0; w < to_width; w++) {
            to_img->data[h][w * 3 + 0] = R[h * to_width + w];
            to_img->data[h][w * 3 + 1] = G[h * to_width + w];
            to_img->data[h][w * 3 + 2] = B[h * to_width + w];
        }
    }
    free(R);
    free(G);
    free(B);
    return to_img;
}

int
gic_resize_bilinear(GIC_IMAGE *img, int to_width, int to_height)
{
    return 0;
}



/*
 * lanczos func
 * sinc(x) * sinc(x / n)
 *
 * sinc func
 * sin(PI * x) / (PI * x)
 *
 */
static double
sinc(double x)
{
    return sin(PI * x) / (PI * x);
}

#define DISCRETE_SIZE 3000
static double
lanczos(double x, int n)
{
    if (n < 2 || n > 3) {
        fprintf(stderr, "Only suport lanczos-2 or lanczos-3\n");
        exit(1);
    }
    static double lookup[DISCRETE_SIZE];
    int i = (int)(x * 1000);

    if (i <= 0) {
        return 1.0;
    } else if (i >= DISCRETE_SIZE) {
        return 0.0;
    }


    double r = lookup[i];
    if (r == 0.0) {
        r = sinc(x) * sinc(x / n);
        lookup[i] = r;
    }
    return r;
}

static int
filter(double x, double total)
{
    int c = x / total;
    if (c < 0) {
        return 0;
    } else if (c > 255) {
        return 255;
    } else {
        return c;
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

    #pragma omp parallel num_threads(2)
    {
        double r, g, b;
        int w, h, x, y;

        double x0, y0;
        int x_bottom, x_top, y_bottom, y_top;
        double weight_total;
        double xl, yl;
        double lanczos_y, lanczos_x;
        double weight;

        #pragma omp for
        for (h = 0; h < to_height; h++) {
            y0 = h + 0.5;
            y_bottom = (y0 - n) / scale;
            y_top = (y0 + n) / scale;
            for (w = 0; w < to_width; w++) {
                r = 0;
                g = 0;
                b = 0;
                weight_total = 0.0;

                x0 = w + 0.5;
                x_bottom = (x0 - n) / scale;
                x_top = (x0 + n) / scale;
                
                for (y = y_bottom; y <= y_top; y++) {
                    if (y < 0 || y >= from_height) {
                        continue;
                    }
                    yl = fabs(((y + 0.5) * scale) - y0);
                    lanczos_y = lanczos(yl, n);
                    for (x = x_bottom; x <= x_top; x++) {
                        if (x < 0 || x >= from_width) {
                            continue;
                        }
                        xl = fabs(((x + 0.5) * scale) - x0);
                        lanczos_x = lanczos(xl, n);
                        weight = lanczos_x * lanczos_y;
                        r += img->data[y][x * 3 + 0] * weight;
                        g += img->data[y][x * 3 + 1] * weight;
                        b += img->data[y][x * 3 + 2] * weight;
                        weight_total += weight;
                    }
                }

                to_img->data[h][w * 3 + 0] = filter(r, weight_total);
                to_img->data[h][w * 3 + 1] = filter(g, weight_total);
                to_img->data[h][w * 3 + 2] = filter(b, weight_total);
            }
        }
    }

    return to_img;
}

