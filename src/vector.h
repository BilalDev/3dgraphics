#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

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

float vec2_length(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_mul(vec2_t v, float factor);
vec2_t vec2_div(vec2_t v, float factor);
float vec2_dot(vec2_t a, vec2_t b);

float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_mul(vec3_t v, float factor);
vec3_t vec3_div(vec3_t v, float factor);
// cross product is not commutative (vec3_cross(a, b) != vec3_cross(b, a))
vec3_t vec3_cross(vec3_t a, vec3_t b);
// dot product is commutative
float vec3_dot(vec3_t a, vec3_t b);

/*
 * Algo is as follow:
 * - find B-A and C-A
 * - find the normal N by getting the cross product
 * - find the camera ray (vector of the camera position and A)
 * - find the dot product between N and the camera ray
 * - return true if greater than 0 (render face)
 */
bool should_render_face(vec3_t vertices[], vec3_t camera_position);

vec3_t vec3_rotate_x(vec3_t initial_position, float angle);
vec3_t vec3_rotate_y(vec3_t initial_position, float angle);
vec3_t vec3_rotate_z(vec3_t initial_position, float angle);

#endif
