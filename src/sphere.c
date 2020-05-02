#include <math.h>

#include "sphere.h"


int sphere_hit(const sphere *s, const ray *r, double t_min, double t_max, hit_record *rec) {
    vec3 oc = vec3_sub(&r->orig, &s->center);
    double a = vec3_length_squared(&r->dir);
    double half_b = vec3_dot(&oc, &r->dir);
    double c = vec3_length_squared(&oc) - s->radius*s->radius;
    double discriminant = half_b*half_b - a*c;

    if (discriminant > 0) {
        double root = sqrt(discriminant);
        double temp = (-half_b - root)/a;
        if (temp < t_max && temp > t_min) {
            rec->t = temp;
            rec->p = ray_at(r, rec->t);
            vec3 n = vec3_sub(&rec->p, &s->center);
            vec3 normal = vec3_div_t(&n, s->radius);
            set_face_normal(rec, r, &normal);
            return 1;
        }
        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec->t = temp;
            rec->p = ray_at(r, rec->t);
            vec3 n = vec3_sub(&rec->p, &s->center);
            vec3 normal = vec3_div_t(&n, s->radius);
            set_face_normal(rec, r, &normal);
            return 1;
        }
    }
    return 0;
}
