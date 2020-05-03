#include "camera.h"

void camera_init(camera *c, const vec3 *lookfrom, const vec3 *lookat, const vec3 *vup, double vfov, double aspect) {
    vec3_copy(&c->origin, lookfrom);

    double theta = degrees_to_radian(vfov);
    double half_height = tan(theta/2);
    double half_width = aspect * half_height;

    vec3 u, v, w;
    vec3_copy(&w, lookfrom);
    vec3_sub_m(&w, lookat);
    vec3_unit_m(&w);
    u = vec3_cross(vup, &w);
    vec3_unit_m(&u);
    v = vec3_cross(&w, &u);

    vec3 u_hw = vec3_mul_t(&u, half_width);
    vec3 v_hh = vec3_mul_t(&v, half_height);
    vec3_copy(&c->lower_left_corner, &c->origin);
    vec3_sub_m(&c->lower_left_corner, &u_hw);
    vec3_sub_m(&c->lower_left_corner, &v_hh);
    vec3_sub_m(&c->lower_left_corner, &w);

    c->horizontal = vec3_mul_t(&u, half_width * 2);
    c->vertical = vec3_mul_t(&v, half_height * 2);
}

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
