#include "triangle.h"

void int_swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Draw a filled triangle with a flat bottom
//
//        (x0,y0)
//          / \
//         /   \
//        /     \
//       /       \
//      /         \
//  (x1,y1)------(mx,my)
//
//////////////////////////////////////////////////////////////////////////////////////////////
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int mx, int my, uint32_t color)
{
    // delta x / delta y
    float inverse_slope_left = (float)(x1 - x0) / (y1 - y0);
    float inverse_slope_right = (float)(mx - x0) / (my - y0);

    // start x_start and x_end from the top vertex
    float x_start = x0;
    float x_end = x0;

    for (int y = y0; y <= my; y++)
    {
        draw_line(x_start, y, x_end, y, color);

        x_start += inverse_slope_left;
        x_end += inverse_slope_right;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
// Draw a filled triangle with a flat top
//
//  (x1,y1)------(mx,my)
//      \         /
//       \       /
//        \     /
//         \   /
//          \ /
//        (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////////////////
void fill_flat_top_triangle(int x1, int y1, int mx, int my, int x2, int y2, uint32_t color)
{
    float inverse_slope_left = (x2 - x1) / (float)(y2 - y1);
    float inverse_slope_right = (x2 - mx) / (float)(y2 - my);

    float x_start = x2;
    float x_end = x2;

    for (int y = y2; y >= y1; y--)
    {
        draw_line(x_start, y, x_end, y, color);

        x_start -= inverse_slope_left;
        x_end -= inverse_slope_right;
    }
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // we need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }
    if (y1 > y2)
    {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    if (y1 == y2)
    {
        // we can simply draw the flat bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else if (y0 == y1)
    {
        // we can simply draw the flat top triangle
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        // calculates the new vertex (Mx, My) using triangle similarity (ratio is equals)
        int my = y1;
        int mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

        fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);
        fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);
    }
}

void swap(triangle_t *a, triangle_t *b)
{
    triangle_t temp = *a;

    *a = *b;
    *b = temp;
}

void bubblesort(triangle_t *triangles)
{
    int number_of_values = array_length(triangles);
    for (int i = 0; i < number_of_values; i++)
    {
        for (int j = i; j < number_of_values; j++)
        {
            if (triangles[i].avg_depth < triangles[j].avg_depth)
            {
                swap(&triangles[i], &triangles[j]);
            }
        }
    }
}
