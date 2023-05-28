#include "triangle.h"

void int_swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void float_swap(float *a, float *b)
{
    float tmp = *a;
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

///////////////////////////////////////////////////////////////////////////////
// Draw a textured triangle based on a texture array of colors.
// We split the original triangle in two, half flat-bottom and half flat-top.
///////////////////////////////////////////////////////////////////////////////
//
//        v0
//        /\
//       /  \
//      /    \
//     /      \
//   v1--------\
//     \_       \
//        \_     \
//           \_   \
//              \_ \
//                 \\
//                   \
//                    v2
//
///////////////////////////////////////////////////////////////////////////////
void draw_textured_triangle(
    int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2,
    uint32_t *texture)
{
    // Loop all the pixels of the triangle to render them based on the color
    // that comes from the texture
    // we need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }
    if (y1 > y2)
    {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
        float_swap(&u1, &u2);
        float_swap(&v1, &v2);
    }
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }

    // Create vector points after we sort the vertices
    vec2_t point_a = {x0, y0};
    vec2_t point_b = {x1, y1};
    vec2_t point_c = {x2, y2};

    //////////////////////////////////////////////////////
    // Render the upper part of the triangle (flat-bottom)
    //////////////////////////////////////////////////////
    float inverse_slope_left = 0;
    float inverse_slope_right = 0;

    if (y1 - y0 != 0)
        inverse_slope_left = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0)
        inverse_slope_right = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0)
    {
        for (int y = y0; y <= y1; y++)
        {
            int x_start = x1 + (y - y1) * inverse_slope_left;
            int x_end = x0 + (y - y0) * inverse_slope_right;

            if (x_end < x_start)
            {
                int_swap(&x_start, &x_end);
            }

            for (int x = x_start; x < x_end; x++)
            {
                draw_texel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
            }
        }
    }

    //////////////////////////////////////////////////////
    // Render the bottom part of the triangle (flat-top)
    //////////////////////////////////////////////////////
    // inverse_slope_right doesn't change
    inverse_slope_left = 0;

    if (y2 - y1 != 0)
        inverse_slope_left = (float)(x2 - x1) / abs(y2 - y1);

    if (y2 - y1 != 0)
    {
        for (int y = y1; y <= y2; y++)
        {
            int x_start = x1 + (y - y1) * inverse_slope_left;
            int x_end = x0 + (y - y0) * inverse_slope_right;

            if (x_end < x_start)
            {
                int_swap(&x_start, &x_end);
            }

            for (int x = x_start; x < x_end; x++)
            {
                draw_texel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
            }
        }
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
