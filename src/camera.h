#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"

typedef struct camera {
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
} camera;

void camera_init(camera *c, const vec3 *lookfrom, const vec3 *lookat, const vec3 *vup, double vfov, double aspect, double aperture, double focus_dist);

ray camera_get_ray(const camera *c, double u, double v);

#endif
