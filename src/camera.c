#include "camera.h"

ray camera_get_ray(const camera *c, double u, double v) {
    vec3 hu = vec3_mul_t(&c->horizontal, u);
    vec3 vv = vec3_mul_t(&c->vertical, v);

    vec3 dir;
    vec3_copy(&dir, &c->lower_left_corner);
    vec3_add_m(&dir, &hu);
    vec3_add_m(&dir, &vv);
    vec3_sub_m(&dir, &c->origin);

    ray r = { .orig = c->origin, .dir = dir };
    return r;
}
