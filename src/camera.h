#if !defined(CAMERA_H)
#define CAMERA_H

#include "vector.h"

typedef struct
{
    vec3_t position;
    vec3_t direction;
    vec3_t forward_velocity;
    // rotation around y axis
    float yaw_angle;
} camera_t;

extern camera_t camera;

#endif // CAMERA_H
