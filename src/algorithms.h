#include <png.h>
#include <math.h>
#include <utility> //std::swap
#include <cstdlib>
#include <vector>
#include <algorithm>

struct line_coords {
	int begin_x, begin_y;
	int end_x, end_y;
};

/// <summary>
/// Draws pixel on given coordinates with given color
/// </summary>
/// <param name="x">x coordinate</param>
/// <param name="y">y coordinate</param>
/// <param name="row_pointers">matrix containing all picture pixels</param>
/// <param name="color">color of given pixel</param>
void write_pixel(int x, int y, struct png_color_struct* color, png_bytep* row_pointers);

/// <summary>
/// Helper - Draws line with slope between <-1;0>
/// </summary>
/// See <see href="https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm">Bresenham Line algorithm</see>
void __draw_line_high(int x0, int y0, int x1, int y1, png_color_struct* color, png_bytep* row_pointers);

/// <summary>
/// Helper - Draws line with slope between <0;1>
/// </summary>
/// See <see href="https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm">Bresenham Line algorithm</see>
void __draw_line_low(int x0, int y0, int x1, int y1, png_color_struct* color, png_bytep* row_pointers);

/// <summary>
/// Draws line from point (xb, yb) to (xe,ye) using bresenham algorithm
/// </summary>
/// <param name="xb">x coordinate of beginning</param>
/// <param name="yb">y coordinate of beginning</param>
/// <param name="xe"x coordinate of end</param>
/// <param name="ye">y coordinate of end</param>
/// <param name="color">color of the line</param>
/// See <see href="https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm">Bresenham Line algorithm</see>
void draw_line(int xb, int yb, int xe, int ye, struct png_color_struct* color, png_bytep* row_pointers);

/// <summary>
/// Draws elipsis at the origin cx, cy with given radii rx,ry
/// </summary>
/// See <see href="https://www.geeksforgeeks.org/dsa/midpoint-ellipse-drawing-algorithm/">Bresenham Line algorithm</see>
void draw_elipsis(int cx, int cy, int rx, int ry, struct png_color_struct* color, png_bytep* row_pointers);
/*Proszê mi wybaczyæ, ale algorytm ze strony powy¿ej wydaje siê byæ o wiele czytelniejszy, dostosowa³em go do naszego programu*/
/*EDIT: W algorytmie nie jest wyt³umaczone, ¿e trzeba zrobiæ projekcjê na 4 osi symetrii (nie wiem jak inaczej to nazwaæ)*/

void draw_filled_elipsis(int cx, int cy, int rx, int ry, struct png_color_struct* color, png_bytep* row_pointers);

void fill_color(line_coords* edges, int edge_count, png_color_struct* color, png_bytep* row_pointers);