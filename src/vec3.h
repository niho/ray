#ifndef VEC3_H
#define VEC3_H

#include <math.h>

#define infinity    INFINITY
#define pi          3.1415926535897932385

typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;

static const vec3 Z = {0.0,0.0,0.0};
static const vec3 I = {1.0,1.0,1.0};

void vec3_zero(vec3 *v);
void vec3_copy(vec3 *v, const vec3 *u);
void vec3_rand(vec3 *v);
void vec3_rand_clamp(vec3 *v, double min, double max);

vec3 vec3_add(const vec3 *v1, const vec3 *v2);
vec3 vec3_sub(const vec3 *v1, const vec3 *v2);
vec3 vec3_mul(const vec3 *v1, const vec3 *v2);

void vec3_add_m(vec3 *v1, const vec3 *v2);
void vec3_sub_m(vec3 *v1, const vec3 *v2);
void vec3_mul_m(vec3 *v1, const vec3 *v2);

vec3 vec3_mul_t(const vec3 *v, double t);
vec3 vec3_div_t(const vec3 *v, double t);

void vec3_mul_mt(vec3 *v, double t);
void vec3_div_mt(vec3 *v, double t);

double vec3_length(const vec3 *v);
double vec3_length_squared(const vec3 *v);

vec3 vec3_neg(const vec3 *v);
void vec3_neg_m(vec3 *v);

void vec3_translate(vec3 *v1, const vec3 *v2);
void vec3_scale(vec3 *v, double s);

double vec3_dot(const vec3 *v1, const vec3 *v2);
vec3 vec3_cross(const vec3 *v1, const vec3 *v2);

vec3 vec3_unit(const vec3 *v);

vec3 vec3_rand_in_unit_sphere();
vec3 vec3_rand_unit();

vec3 vec3_reflect(const vec3 *v, const vec3 *n);
vec3 vec3_refract(const vec3 *v, const vec3 *n, double etai_over_etat);

#endif
