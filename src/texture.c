#include "texture.h"

int texture_width = 64;
int texture_height = 64;

uint32_t *mesh_texture = NULL;
upng_t *png_texture = NULL;

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
    vec2_t ac = vec2_sub(c, a);
    vec2_t ab = vec2_sub(b, a);
    vec2_t pc = vec2_sub(c, p);
    vec2_t pb = vec2_sub(b, p);
    vec2_t ap = vec2_sub(p, a);

    // Area of the full parallelogram (triangle ABC) using cross product
    float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // || AC x AB ||

    // Alpha = area of parallelogram-PBC over the area of the full parallelogram-ABC
    float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;

    // Beta = area of parallelogram-APC over the area of the full parallelogram-ABC
    float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;

    float gamma = 1.0 - alpha - beta;

    vec3_t weights = {alpha, beta, gamma};

    return weights;
}

void load_png_texture_data(char *filepath)
{
    png_texture = upng_new_from_file(filepath);
    if (png_texture != NULL)
    {
        upng_decode(png_texture);
        if (upng_get_error(png_texture) == UPNG_EOK)
        {
            mesh_texture = (uint32_t *)upng_get_buffer(png_texture);
            texture_width = upng_get_width(png_texture);
            texture_height = upng_get_height(png_texture);
        }
    }
}
