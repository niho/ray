#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "ray.h"

struct hit_record;

typedef enum material_type {
    DIFFUSE_LAMBERTIAN,
    METAL,
    DIELECTRIC
} material_type;

typedef struct material {
    material_type type;
    vec3 albedo;
    double fuzz;
    double ref_idx;
} material;

int material_scatter(
        const material *m,
        const ray *r_in,
        const struct hit_record *rec,
        vec3 *attenuation,
        ray *scattered);

#endif
