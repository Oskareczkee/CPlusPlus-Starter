#include "algorithms.h"

void write_pixel(int x, int y,png_color_struct* color, png_bytep* row_pointers)
{
    png_byte* row = row_pointers[y];
    png_byte* ptr = &(row[x * 3]);
    ptr[0] = color->red;
    ptr[1] = color->green;
    ptr[2] = color->blue;
}

void __draw_line_high(int x0, int y0, int x1, int y1, png_color_struct* color, png_bytep* row_pointers)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0){
        xi = -1;
        dx = -dx;
    }

    int D = (2 * dx) - dy;
    int x = x0;

    for (int y = y0; y < y1; y++) {
        write_pixel(x, y, color, row_pointers);
        if (D > 0) {
            x += xi;
            D += 2 * (dx - dy);
        }
        else
            D += 2 * dx;
    }
}

void __draw_line_low(int x0, int y0, int x1, int y1, png_color_struct* color, png_bytep* row_pointers) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int D = (2 * dy) - dx;
    int y = y0;

    for (int x = x0; x < x1; x++) {
        write_pixel(x, y, color, row_pointers);
        if (D > 0) {
            y += yi;
            D += 2 * (dy - dx);
        }
        else
            D += 2 * dy;
    }
}

void draw_line(int xb, int yb, int xe, int ye, png_color_struct* color, png_bytep* row_pointers)
{
    if (abs(ye - yb) < abs(xe - xb)) {
        if (xb > xe)
            __draw_line_low(xe, ye, xb, yb, color, row_pointers);
        else
            __draw_line_low(xb, yb, xe, ye, color, row_pointers);
    }
    else {
        if (yb > ye)
            __draw_line_high(xe, ye, xb, yb, color, row_pointers);
        else
            __draw_line_high(xb, yb, xe, ye, color, row_pointers);
    }
}

void draw_elipsis(int cx, int cy, int rx, int ry, png_color_struct* color, png_bytep* row_pointers)
{
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;

    // Initial decision parameter of region 1
    d1 = (ry * ry) - (rx * rx * ry) +
        (0.25 * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    // For region 1
    while (dx < dy)
    {

        write_pixel(x + cx, y + cy, color, row_pointers);
        write_pixel(-x + cx, y + cy, color, row_pointers);
        write_pixel(x + cx, -y + cy, color, row_pointers);
        write_pixel(-x + cx, -y + cy, color, row_pointers);

        // Checking and updating value of
        // decision parameter based on algorithm
        if (d1 < 0)
        {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        }
        else
        {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }

    // Decision parameter of region 2
    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
        ((rx * rx) * ((y - 1) * (y - 1))) -
        (rx * rx * ry * ry);

    // Plotting points of region 2
    while (y >= 0)
    {

        write_pixel(x + cx, y + cy, color, row_pointers);
        write_pixel(-x + cx, y + cy, color, row_pointers);
        write_pixel(x + cx, -y + cy, color, row_pointers);
        write_pixel(-x + cx, -y + cy, color, row_pointers);

        // Checking and updating parameter
        // value based on algorithm
        if (d2 > 0)
        {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        }
        else
        {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
}

void draw_filled_elipsis(int cx, int cy, int rx, int ry, png_color_struct* color, png_bytep* row_pointers)
{
    for (int y = -ry; y <= ry; y++) {
        for (int x = -rx; x <= rx; x++) {
            if (x * x * ry * ry + y * y * rx * rx <= ry * ry * rx * rx)
                write_pixel(cx + x, cy + y,color, row_pointers);
        }
    }
}

void fill_color(line_coords* edges,int edge_count, png_color_struct* color, png_bytep* row_pointers)
{
    int y_min = edges[0].begin_y;
    int y_max = edges[0].begin_y;

    //find min/max y coordinates
    for (int i = 0; i < edge_count; i++) {
        if (edges[i].begin_y < y_min) y_min = edges[i].begin_y;
        if (edges[i].end_y < y_min) y_min = edges[i].end_y;
        if (edges[i].begin_y > y_max) y_max = edges[i].begin_y;
        if (edges[i].end_y > y_max) y_max = edges[i].end_y;
    }


    //scanline from lowest y coordinate to the maximum one
    for (int y = y_min; y <= y_max; y++) {
        std::vector<int> intersections;   

        for (int i = 0; i < edge_count; i++) {
            int x0 = edges[i].begin_x;
            int y0 = edges[i].begin_y;
            int x1 = edges[i].end_x;
            int y1 = edges[i].end_y;

            //do not count horizontal edges
            if (y0 == y1)
                continue;

            //Check for scanline intersections
            if ((y >= y0 && y < y1) || (y >= y1 && y < y0)) //check if scanline passes through actual edge 
            {
                int x = x0 + (y - y0) * (x1 - x0) / (float)(y1 - y0); //calculate x intersection
                intersections.push_back(x);
            }
        }

        //sort intersections from lowest to highest coordinates
        std::sort(intersections.begin(), intersections.end(), 
                    [](int& first, int& second) {return first < second; });

        for (int i = 0; i < intersections.size(); i += 2) {
            if (i + 1 >= intersections.size())
                break;
            int x_start = intersections[i];
            int x_end = intersections[i + 1];

            for (int x = x_start; x <= x_end; x++)
                write_pixel(x, y, color, row_pointers);
        }
    }
}
