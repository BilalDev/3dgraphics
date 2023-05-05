#include "vector.h"
#include "math.h"

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
