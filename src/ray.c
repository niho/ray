#include "ray.h"

ray ray_init(const vec3 *orig, const vec3 *dir) {
    ray r;
    r.orig.x = orig->x;
    r.orig.y = orig->y;
    r.orig.z = orig->z;
    r.dir.x = dir->x;
    r.dir.y = dir->y;
    r.dir.z = dir->z;
    return r;
}

vec3 ray_at(const ray *r, double t) {
    vec3 bt = vec3_mul_t(&r->dir, t);
    vec3 p = vec3_add(&r->orig, &bt);
    return p;
}

