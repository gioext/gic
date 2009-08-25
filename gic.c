#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "gic.h"

int
gic_jpeg_open(char *filename, GIC_IMAGE *img)
{
    int i;
    FILE *fin = fopen(filename, "rb");
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

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
    return 0;
}

int
gic_jpeg_free(GIC_IMAGE *img)
{
    int i;
    for (i = 0; i < img->height; i++) {
        free((img->data)[i]);
    }
    free(img->data);
    return 0;
}

int
gic_write_ppm(char *infile, char *outfile)
{
    GIC_IMAGE img;
    int i, j;

    if(gic_jpeg_open(infile, &img) != 0) {
        return 1;
    }

    // write ppm
    FILE *fout = fopen(outfile, "wb");
    fprintf(fout, "P6\n");
    fprintf(fout, "%d %d\n", img.width, img.height);
    fprintf(fout, "255\n");
    for (i = 0; i < img.height; i++) {
        for (j = 0; j < img.width; j++) {
            putc(img.data[i][j * 3 + 0], fout);
            putc(img.data[i][j * 3 + 1], fout);
            putc(img.data[i][j * 3 + 2], fout);
        }
    }
    fclose(fout);

    // free img
    if (gic_jpeg_free(&img) != 0) {
        return 1;
    }
    return 0;
}
