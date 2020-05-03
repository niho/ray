#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"
#include "ray.h"
#include "material.h"

typedef struct hit_record {
    vec3 p;
    vec3 normal;
    double t;
    int front_face;
    const material *mat;
} hit_record;

void set_face_normal(hit_record *rec, const ray *r, const vec3 *outward_normal);

#endif
