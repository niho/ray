#include "camera.h"

void camera_init(camera *c, const vec3 *lookfrom, const vec3 *lookat, const vec3 *vup, double vfov, double aspect, double aperture, double focus_dist) {
    vec3_copy(&c->origin, lookfrom);

    c->lens_radius = aperture / 2;

    double theta = degrees_to_radian(vfov);
    double half_height = tan(theta/2);
    double half_width = aspect * half_height;

    vec3_copy(&c->w, lookfrom);
    vec3_sub_m(&c->w, lookat);
    vec3_unit_m(&c->w);
    c->u = vec3_cross(vup, &c->w);
    vec3_unit_m(&c->u);
    c->v = vec3_cross(&c->w, &c->u);

    vec3 u_hw = vec3_mul_t(&c->u, half_width * focus_dist);
    vec3 v_hh = vec3_mul_t(&c->v, half_height * focus_dist);
    vec3 w_f  = vec3_mul_t(&c->w, focus_dist); 
    vec3_copy(&c->lower_left_corner, &c->origin);
    vec3_sub_m(&c->lower_left_corner, &u_hw);
    vec3_sub_m(&c->lower_left_corner, &v_hh);
    vec3_sub_m(&c->lower_left_corner, &w_f);

    c->horizontal = vec3_mul_t(&c->u, half_width * 2 * focus_dist);
    c->vertical = vec3_mul_t(&c->v, half_height * 2 * focus_dist);
}

ray camera_get_ray(const camera *c, double s, double t) {
    vec3 rd = vec3_rand_unit_in_disk();
    vec3_mul_mt(&rd, c->lens_radius);

    vec3 u_rd_x = vec3_mul_t(&c->u, rd.x);
    vec3 v_rd_y = vec3_mul_t(&c->v, rd.y);
    vec3 offset = vec3_add(&u_rd_x, &v_rd_y);

    vec3 sh = vec3_mul_t(&c->horizontal, s);
    vec3 tv = vec3_mul_t(&c->vertical, t);

    vec3 orig;
    vec3_copy(&orig, &c->origin);
    vec3_add_m(&orig, &offset);

    vec3 dir;
    vec3_copy(&dir, &c->lower_left_corner);
    vec3_add_m(&dir, &sh);
    vec3_add_m(&dir, &tv);
    vec3_sub_m(&dir, &c->origin);
    vec3_sub_m(&dir, &offset);

    ray r = { .orig = orig, .dir = dir };
    return r;
}
