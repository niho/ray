#include <math.h>

#include "vec3.h"
#include "rand.h"

void vec3_zero(vec3 *v) {
    v->x = 0;
    v->y = 0;
    v->z = 0;
}

void vec3_copy(vec3 *v, const vec3 *u) {
    v->x = u->x;
    v->y = u->y;
    v->z = u->z;
}

void vec3_rand(vec3 *v) {
    v->x = rand_double();
    v->y = rand_double();
    v->z = rand_double();
}

void vec3_rand_clamp(vec3 *v, double min, double max) {
    v->x = rand_double_clamp(min, max);
    v->y = rand_double_clamp(min, max);
    v->z = rand_double_clamp(min, max);
}

vec3 vec3_add(const vec3 *v1, const vec3 *v2) {
    vec3 v = {v1->x+v2->x, v1->y+v2->y, v1->z+v2->z};
    return v;
}

vec3 vec3_sub(const vec3 *v1, const vec3 *v2) {
    vec3 v = {v1->x-v2->x, v1->y-v2->y, v1->z-v2->z};
    return v;
}

vec3 vec3_mul(const vec3 *v1, const vec3 *v2) {
    vec3 v = {v1->x*v2->x, v1->y*v2->y, v1->z*v2->z};
    return v;
}

void vec3_add_m(vec3 *v1, const vec3 *v2) {
    v1->x = v1->x + v2->x;
    v1->y = v1->y + v2->y;
    v1->z = v1->z + v2->z;
}

void vec3_sub_m(vec3 *v1, const vec3 *v2) {
    v1->x = v1->x - v2->x;
    v1->y = v1->y - v2->y;
    v1->z = v1->z - v2->z;
}

void vec3_mul_m(vec3 *v1, const vec3 *v2) {
    v1->x = v1->x * v2->x;
    v1->y = v1->y * v2->y;
    v1->z = v1->z * v2->z;
}

vec3 vec3_mul_t(const vec3 *v, double t) {
    vec3 u = {v->x*t, v->y*t, v->z*t};
    return u;
}

void vec3_mul_mt(vec3 *v, double t) {
    v->x = v->x * t;
    v->y = v->y * t;
    v->z = v->z * t;
}

vec3 vec3_div_t(const vec3 *v, double t) {
    return vec3_mul_t(v, 1.0 / t);
}

void vec3_div_mt(vec3 *v, double t) {
    vec3_mul_mt(v, 1.0 / t);
}

double vec3_length(const vec3 *v) {
    return sqrt(vec3_length_squared(v));
}

double vec3_length_squared(const vec3 *v) {
    return v->x*v->x + v->y*v->y + v->z*v->z;
}

vec3 vec3_neg(const vec3 *v) {
    vec3 u = {-v->x, -v->y, -v->z};
    return u;
}

void vec3_neg_m(vec3 *v) {
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
}

void vec3_translate(vec3 *v1, const vec3 *v2) {
    v1->x = v1->x + v2->x;
    v1->y = v1->y + v2->y;
    v1->z = v1->z + v2->z;
}

void vec3_scale(vec3 *v, double s) {
    v->x = v->x * s;
    v->y = v->y * s;
    v->z = v->z * s;
}

double vec3_dot(const vec3 *v1, const vec3 *v2) {
    return v1->x * v2->x 
         + v1->y * v2->y
         + v1->z * v2->z;
}

vec3 vec3_cross(const vec3 *v1, const vec3 *v2) {
    vec3 v;
    v.x = v1->y * v2->z - v1->z * v2->y;
    v.y = v1->z * v2->x - v1->x * v2->z;
    v.z = v1->x * v2->y - v1->y * v2->x;
    return v;
}

vec3 vec3_unit(const vec3 *v) {
    return vec3_div_t(v, vec3_length(v));
}

void vec3_unit_m(vec3 *v) {
    vec3_div_mt(v, vec3_length(v));
}

vec3 vec3_rand_in_unit_sphere() {
    while (1) {
        vec3 p;
        vec3_rand_clamp(&p, -1.0, 1.0);
        if (vec3_length_squared(&p) >= 1.0) continue;
        return p;
    }
}

vec3 vec3_rand_unit() {
    double a = rand_double_clamp(0, 2*pi);
    double z = rand_double_clamp(-1, 1);
    double r = sqrt(1 - z*z);
    vec3 v = { r*cos(a), r*sin(a), z };
    return v;
}

vec3 vec3_rand_unit_in_disk() {
    while (1) {
        vec3 p = {rand_double_clamp(-1,1), rand_double_clamp(-1,1), 0};
        if (vec3_length_squared(&p) >= 1) continue;
        return p;
    }
}

vec3 vec3_reflect(const vec3 *v, const vec3 *n) {
    vec3 u = vec3_mul_t(n, 2 * vec3_dot(v, n));
    return vec3_sub(v, &u);
}

vec3 vec3_refract(const vec3 *v, const vec3 *n, double etai_over_etat) {
    vec3 v_neg = {-v->x, -v->y, -v->z};
    double cos_theta = vec3_dot(&v_neg, n);
    vec3 r_out_parallel = vec3_mul_t(n, cos_theta);
    vec3_add_m(&r_out_parallel, v);
    vec3_mul_mt(&r_out_parallel, etai_over_etat);
    double r_perp = -sqrt(1.0 - vec3_length_squared(&r_out_parallel));
    vec3 r_out_perp = vec3_mul_t(n, r_perp);
    return vec3_add(&r_out_parallel, &r_out_perp);
}
