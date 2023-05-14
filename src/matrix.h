#ifndef MATRIX_H
#define MATRIX_H

typedef struct
{
    float matrix[4][4];
} mat4_t;

mat4_t mat4_make_scale(float sx, float sy, float sz);

mat4_t mat4_identity();

#endif
