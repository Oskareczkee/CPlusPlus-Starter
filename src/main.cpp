/*
 * Copyright 2002-2008 Guillaume Cottenceau, 2015 Aleksander Denisiuk
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>
#include "algorithms.h"


#define OUT_FILE "initials.png"
#define WIDTH 600
#define HEIGHT 600
#define COLOR_TYPE PNG_COLOR_TYPE_RGB
#define BIT_DEPTH 8


void abort_(const char* s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}

int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep* row_pointers;

void create_png_file()
{
    width = WIDTH;
    height = HEIGHT;
    bit_depth = BIT_DEPTH;
    color_type = COLOR_TYPE;

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++)
        row_pointers[y] = (png_byte*)malloc(width * (bit_depth / 8) * 3);


}


void write_png_file(char* file_name)
{
    /* create file */
    FILE* fp = fopen(file_name, "wb");
    if (fp==NULL)
        abort_("[write_png_file] File %s could not be opened for writing", file_name);

    
    /* initialize stuff */
    
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        abort_("[write_png_file] png_create_write_struct failed");

    info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr)
        abort_("[write_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during init_io");

    png_init_io(png_ptr, fp);



    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing header");

    png_set_IHDR(png_ptr, info_ptr, width, height,
        bit_depth, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing bytes");

    png_write_image(png_ptr, row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during end of write");

    png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
    for (y = 0; y < height; y++)
        free(row_pointers[y]);
    free(row_pointers);

    png_destroy_write_struct(&png_ptr, NULL);
    fclose(fp);
}

void process_file(void)
{
    for (y = 0; y < height; y++) {
        png_byte* row = row_pointers[y];
        for (x = 0; x < width; x++) {
            png_byte* ptr = &(row[x * 3]);
            ptr[0] = 0;
            ptr[1] = ptr[2] = 255;
        }
    }

    struct line_coords polygon_N[] = {
        {320, 440, 320, 160},
        {320, 160, 330, 160},
        {330, 160, 400, 400},
        {400, 400, 400, 160},
        {400, 160, 410, 160},
        {410, 160, 410, 440},
        {410, 440, 400, 440},
        {400, 440, 330, 200},
        {330, 200, 330, 440},
        {330, 440, 320, 440}
    };

    //circular background
    draw_filled_elipsis(300, 300, 200, 200, new png_color_struct{ 0, 0, 100 }, row_pointers);
    draw_filled_elipsis(300, 300, 180, 180, new png_color_struct{ 220, 220, 255 }, row_pointers);


    draw_filled_elipsis(230, 300, 70, 140, new png_color_struct{ 255,0,0 }, row_pointers);
    draw_filled_elipsis(230, 300, 55, 115, new png_color_struct{ 220, 220, 255 }, row_pointers);

    fill_color(polygon_N, sizeof(polygon_N) / sizeof(polygon_N[0]), new png_color_struct{ 255,0,0 }, row_pointers);
}


int main(int argc, char** argv)
{
    char out_file[] = "initials.png";

    create_png_file();
    process_file();
    write_png_file(out_file);

    return 0;
}