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
    vec3 dir = {r->dir.x, r->dir.y, r->dir.z};
    vec3 orig = {r->orig.x, r->orig.y, r->orig.z};
    vec3_scale(&dir, t);
    vec3_translate(&orig, &dir);
    return orig;
}

