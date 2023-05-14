#include "matrix.h"

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
    mat4_t m = mat4_identity();

    m[0][0] = sx;
    m[1][1] = sy;
    m[2][2] = sz;

    return m;
}

mat4_t mat4_identity()
{
    mat4_t m;

    m[0][0] = 1;
    m[1][1] = 1;
    m[2][2] = 1;
    m[3][3] = 1;
}