#include "vector.h"

// V2

vec2_t new_vec2(float x, float y)
{
    vec2_t result = {x, y};

    return result;
}

float vec2_length(vec2_t v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

vec2_t vec2_add(vec2_t a, vec2_t b)
{
    vec2_t v = {
        .x = a.x + b.x,
        .y = a.y + b.y};

    return v;
}

vec2_t vec2_sub(vec2_t a, vec2_t b)
{
    vec2_t v = {
        .x = a.x - b.x,
        .y = a.y - b.y};

    return v;
}

vec2_t vec2_mul(vec2_t v, float factor)
{
    vec2_t result = {
        .x = v.x * factor,
        .y = v.y * factor};

    return result;
}

vec2_t vec2_div(vec2_t v, float factor)
{
    vec2_t result = {
        .x = v.x / factor,
        .y = v.y / factor};

    return result;
}

float vec2_dot(vec2_t a, vec2_t b)
{
    return (a.x * b.x) + (a.y * b.y);
}

void vec2_normalize(vec2_t *v)
{
    float length = vec2_length(*v);

    v->x /= length;
    v->y /= length;
}

// V3

vec3_t new_vec3(float x, float y, float z)
{
    vec3_t result = {x, y, z};

    return result;
}

float vec3_length(vec3_t v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vec3_t vec3_add(vec3_t a, vec3_t b)
{
    vec3_t v = {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z};

    return v;
}

vec3_t vec3_sub(vec3_t a, vec3_t b)
{
    vec3_t v = {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z};

    return v;
}

vec3_t vec3_mul(vec3_t v, float factor)
{
    vec3_t result = {
        .x = v.x * factor,
        .y = v.y * factor,
        .z = v.z * factor};

    return result;
}

vec3_t vec3_div(vec3_t v, float factor)
{
    vec3_t result = {
        .x = v.x / factor,
        .y = v.y / factor,
        .z = v.z / factor};

    return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b)
{
    vec3_t result = {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x};

    return result;
}

float vec3_dot(vec3_t a, vec3_t b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3_normalize(vec3_t *v)
{
    float length = vec3_length(*v);

    v->x /= length;
    v->y /= length;
    v->z /= length;
}

vec3_t vec3_rotate_x(vec3_t initial_vector, float angle)
{
    vec3_t rotated_vector = {
        .x = initial_vector.x,
        .y = (initial_vector.y * cos(angle)) - (initial_vector.z * sin(angle)),
        .z = (initial_vector.z * cos(angle)) + (initial_vector.y * sin(angle))};

    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t initial_vector, float angle)
{
    vec3_t rotated_vector = {
        .x = (initial_vector.x * cos(angle)) - (initial_vector.z * sin(angle)),
        .y = initial_vector.y,
        .z = (initial_vector.z * cos(angle)) + (initial_vector.x * sin(angle))};

    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t initial_vector, float angle)
{
    vec3_t rotated_vector = {
        .x = (initial_vector.x * cos(angle)) - (initial_vector.y * sin(angle)),
        .y = (initial_vector.y * cos(angle)) + (initial_vector.x * sin(angle)),
        .z = initial_vector.z};

    return rotated_vector;
}

vec4_t vec4_from_vec3(vec3_t v)
{
    vec4_t result = {v.x, v.y, v.z, 1};

    return result;
}

vec3_t vec3_from_vec4(vec4_t v)
{
    vec3_t result = {v.x, v.y, v.z};

    return result;
}

vec2_t vec2_from_vec4(vec4_t v)
{
    vec2_t result = {v.x, v.y};

    return result;
}

vec3_t vec3_clone(vec3_t *v)
{
    vec3_t clone = {
        .x = v->x,
        .y = v->y,
        .z = v->z,
    };

    return clone;
}