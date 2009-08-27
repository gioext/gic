#include "gic.h"

GIC_IMAGE *
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
gic_jpeg_open(char *filename)
{
    int i;
    FILE *fin = fopen(filename, "rb");
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    GIC_IMAGE *img = (GIC_IMAGE *)malloc(sizeof(GIC_IMAGE));

    if (fin == NULL) {
        printf("file open error\n");
        exit(1);
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fin);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    img->width = cinfo.output_width;
    img->height = cinfo.output_height;


    img->data = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * img->height);
    for (i = 0; i < img->height; i++) {
        (img->data)[i] = (JSAMPROW)calloc(sizeof(JSAMPLE), 3 * img->width);
    }

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, img->data + cinfo.output_scanline, cinfo.output_height - cinfo.output_scanline);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fin);
    return img;
}

int
gic_jpeg_free(GIC_IMAGE *img)
{
    int i;
    for (i = 0; i < img->height; i++) {
        free((img->data)[i]);
    }
    free(img->data);
    free(img);
    return 0;
}

int
gic_write_image(GIC_IMAGE *img, char *filename)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    FILE *outfile = fopen(filename, "wb");
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = img->width;
    cinfo.image_height = img->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, 90, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    jpeg_write_scanlines(&cinfo, img->data, img->height);


    jpeg_finish_compress(&cinfo);

    jpeg_destroy_compress(&cinfo);

    fclose(outfile);
    return 0;
}

int
gic_write_ppm(char *infile, char *outfile)
{
    GIC_IMAGE *img;
    int i, j;

    img = gic_jpeg_open(infile);

    // write ppm
    FILE *fout = fopen(outfile, "wb");
    fprintf(fout, "P6\n");
    fprintf(fout, "%d %d\n", img->width, img->height);
    fprintf(fout, "255\n");
    for (i = 0; i < img->height; i++) {
        for (j = 0; j < img->width; j++) {
            putc(img->data[i][j * 3 + 0], fout);
            putc(img->data[i][j * 3 + 1], fout);
            putc(img->data[i][j * 3 + 2], fout);
        }
    }
    fclose(fout);

    // free img
    if (gic_jpeg_free(img) != 0) {
        return 1;
    }
    return 0;
}
