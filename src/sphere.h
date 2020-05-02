#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

typedef struct sphere {
    vec3 center;
    double radius;
} sphere;

int sphere_hit(const sphere *s, const ray *r, double t_min, double t_max, hit_record *rec);

#endif
