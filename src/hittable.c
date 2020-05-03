#include "hittable.h"


void set_face_normal(hit_record *rec, const ray *r, const vec3 *outward_normal) {
    rec->front_face = vec3_dot(&r->dir, outward_normal) < 0;
    if (rec->front_face) {
        rec->normal.x = outward_normal->x;
        rec->normal.y = outward_normal->y;
        rec->normal.z = outward_normal->z;
    } else {
        rec->normal.x = -outward_normal->x;
        rec->normal.y = -outward_normal->y;
        rec->normal.z = -outward_normal->z;
    }
}

