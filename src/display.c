#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
// uint32_t instead of int to make sure it's a 32 bit integer
uint32_t *color_buffer = NULL;
float *z_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;
// here using int because we don't care about the size of int as it's only 800/600
int window_width = 800;
int window_height = 600;

bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    window = SDL_CreateWindow(NULL,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              window_width,
                              window_height,
                              SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer.\n");
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void draw_grid(void)
{
    for (int y = 0; y < window_height; y += 10)
    {
        for (int x = 0; x < window_width; x += 10)
        {
            color_buffer[(window_width * y) + x] = 0xFF4285f4;
        }
    }
}

void draw_pixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < window_width && y < window_height && y >= 0)
    {
        color_buffer[(window_width * y) + x] = color;
    }
}

void draw_texel(
    int x, int y, uint32_t *texture,
    vec4_t point_a, vec4_t point_b, vec4_t point_c,
    tex2_t a_uv, tex2_t b_uv, tex2_t c_uv)
{
    vec2_t p = {x, y};
    vec2_t a = vec2_from_vec4(point_a);
    vec2_t b = vec2_from_vec4(point_b);
    vec2_t c = vec2_from_vec4(point_c);

    vec3_t weights = barycentric_weights(a, b, c, p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolated_u;
    float interpolated_v;
    float interpolated_reciprocal_w; // 1/w

    // Perform the interpolation of all U/w and V/w values using barycentric weights
    interpolated_u = (a_uv.u / point_a.w) * alpha + (b_uv.u / point_b.w) * beta + (c_uv.u / point_c.w) * gamma;
    interpolated_v = (a_uv.v / point_a.w) * alpha + (b_uv.v / point_b.w) * beta + (c_uv.v / point_c.w) * gamma;

    interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

    interpolated_u /= interpolated_reciprocal_w;
    interpolated_v /= interpolated_reciprocal_w;

    // Map the UV coordinate to the full texture width and height
    int tex_x = abs((int)(interpolated_u * texture_width)) % texture_width;
    int tex_y = abs((int)(interpolated_v * texture_height)) % texture_height;

    // Adjust 1/w so the pixels that are closer to the camera have smaller values
    interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

    // Only draw the pixel if the depth value is less than the one previously stored in the same z-buffer
    if (interpolated_reciprocal_w < z_buffer[(window_width * y) + x])
    {
        draw_pixel(x, y, texture[(texture_width * tex_y) + tex_x]);

        // Update the z-buffer value with the 1/w of this current pixel
        z_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int current_x = x + i;
            int current_y = y + j;

            draw_pixel(current_x, current_y, color);
        }
    }
}

// Draw line using DDA algorithm poor performance because algo is dealing with floating point and division
// float x_inc = dx / (float)side_length;
// float y_inc = dy / (float)side_length;
void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int side_length = (abs(dx) >= abs(dy)) ? abs(dx) : abs(dy);

    float x_inc = dx / (float)side_length;
    float y_inc = dy / (float)side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= side_length; i++)
    {
        draw_pixel(round(current_x), round(current_y), color);

        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void render_color_buffer(void)
{
    SDL_UpdateTexture(color_buffer_texture,
                      NULL,
                      color_buffer,
                      (int)(window_width * sizeof(uint32_t)));

    SDL_RenderCopy(renderer,
                   color_buffer_texture,
                   NULL,
                   NULL);
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

void clear_z_buffer(void)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            z_buffer[(window_width * y) + x] = 1.0;
        }
    }
}

void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
