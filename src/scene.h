#ifndef SCENE_H
#define SCENE_H

#include "hittable.h"
#include "material.h"

typedef enum geometry_type {
    SPHERE = 0
} geometry_type;

typedef struct scene_node {
    geometry_type type;
    const void *geometry;
    const material *mat;
    void *next;
} scene_node;

typedef struct scene {
    scene_node *first;
    scene_node *last;
} scene;

void scene_init(scene *s);
void scene_add(scene *s, geometry_type t, const void *geometry, const material *mat);
void scene_clear(scene *s);

int scene_hit(const scene *s, const ray *r, double t_min, double t_max, hit_record *rec);


#endif
