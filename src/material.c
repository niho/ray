#include <assert.h>

#include "material.h"
#include "hittable.h"
#include "rand.h"

static int diffuse_scatter(const material *m, const struct hit_record *rec, vec3 *attenuation, ray *scattered) {
    assert(m->type == DIFFUSE_LAMBERTIAN);
    vec3 R = vec3_rand_unit();
    vec3 dir = vec3_add(&rec->normal, &R);
    scattered->orig = rec->p;
    scattered->dir = dir;
    vec3_copy(attenuation, &m->albedo);
    return 1;
}

static int metal_scatter(const material *m, const ray *r_in, const struct hit_record *rec, vec3 *attenuation, ray *scattered) {
    assert(m->type == METAL);
    vec3 dir_unit = vec3_unit(&r_in->dir);
    vec3 reflected = vec3_reflect(&dir_unit, &rec->normal);
    vec3 R = vec3_rand_in_unit_sphere();
    vec3_mul_mt(&R, m->fuzz < 1 ? m->fuzz : 1);
    vec3_add_m(&R, &reflected);
    scattered->orig = rec->p;
    scattered->dir = R;
    vec3_copy(attenuation, &m->albedo); 
    return vec3_dot(&scattered->dir, &rec->normal) > 0;
}

static double schlick(double cosine, double ref_idx) {
    double r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine), 5);
}

static int dielectric_scatter(const material *m, const ray *r_in, const struct hit_record *rec, vec3 *attenuation, ray *scattered) { 
    vec3_copy(attenuation, &I);
    double etai_over_etat;
    if (rec->front_face) {
        etai_over_etat = 1.0 / m->ref_idx;
    } else {
        etai_over_etat = m->ref_idx;
    }
    
    vec3 unit_dir = vec3_unit(&r_in->dir);
    vec3 unit_dir_neg = vec3_neg(&unit_dir);
    double cos_theta = ffmin(vec3_dot(&unit_dir_neg, &rec->normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
    
    if (etai_over_etat * sin_theta > 1.0) {
        vec3 reflected = vec3_reflect(&unit_dir, &rec->normal);
        scattered->orig = rec->p;
        scattered->dir = reflected;
        return 1;
    }

    double reflect_prob = schlick(cos_theta, etai_over_etat);
    if (rand_double() < reflect_prob) {
        vec3 reflected = vec3_reflect(&unit_dir, &rec->normal);
        scattered->orig = rec->p;
        scattered->dir = reflected;
        return 1;
    }

    vec3 refracted = vec3_refract(&unit_dir, &rec->normal, etai_over_etat);
    scattered->orig = rec->p;
    scattered->dir = refracted;
    return 1;
}

int material_scatter(const material *m, const ray *r_in, const struct hit_record *rec, vec3 *attenuation, ray *scattered) {
    switch (m->type) {
        case DIFFUSE_LAMBERTIAN:
            return diffuse_scatter(m, rec, attenuation, scattered);
        case METAL:
            return metal_scatter(m, r_in, rec, attenuation, scattered);
        case DIELECTRIC:
            return dielectric_scatter(m, r_in, rec, attenuation, scattered);
        default:
            return 0;
    }
}
