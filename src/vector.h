#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include "math.h"

typedef struct
{
    float x;
    float y;
} vec2_t;

typedef struct
{
    float x;
    float y;
    float z;
} vec3_t;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec4_t;

// V2 FUNCTIONS
float vec2_length(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_mul(vec2_t v, float factor);
vec2_t vec2_div(vec2_t v, float factor);
float vec2_dot(vec2_t a, vec2_t b);
void vec2_normalize(vec2_t *v);

// V3 FUNCTIONS
float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_mul(vec3_t v, float factor);
vec3_t vec3_div(vec3_t v, float factor);
// cross product is not commutative (vec3_cross(a, b) != vec3_cross(b, a))
vec3_t vec3_cross(vec3_t a, vec3_t b);
// dot product is commutative
float vec3_dot(vec3_t a, vec3_t b);
// not everything is meant to be normalized
// ask yourself if you only need the direction of a vector
// if so, then you should normalize
void vec3_normalize(vec3_t *v);

vec3_t vec3_rotate_x(vec3_t initial_position, float angle);
vec3_t vec3_rotate_y(vec3_t initial_position, float angle);
vec3_t vec3_rotate_z(vec3_t initial_position, float angle);

// V4 conversions
vec4_t vec4_from_vec3(vec3_t v);
vec3_t vec3_from_vec4(vec4_t v);
vec2_t vec2_from_vec4(vec4_t v);

#endif
