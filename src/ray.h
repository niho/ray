#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct ray {
    vec3 orig;
    vec3 dir;
} ray;

ray ray_init(const vec3 *orig, const vec3 *dir);

vec3 ray_at(const ray *r, double t);

#endif
