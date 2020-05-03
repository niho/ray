#include <stdlib.h>
#include <stdio.h>

#include "scene.h"
#include "sphere.h"

void scene_init(scene *s) {
    s->first = NULL;
    s->last = NULL;
}

void scene_add(scene *s, geometry_type t, const void *geometry, const material *mat) {
    scene_node *node = (scene_node *)malloc(sizeof(scene_node));
    node->type = t;
    node->geometry = geometry;
    node->mat = mat;
    node->next = NULL;
    if (s->last == NULL) {
        s->first = node;
        s->last = node;
    } else {
        s->last->next = node;
        s->last = node;
    }
}

void scene_clear(scene *s) {
    scene_node *node = s->first;
    for (;;) {
        if (node == NULL)
            break;
        scene_node *next = node->next;
        free(node);
        node = next;
    }
    s->first = s->last = NULL;
}

int scene_hit(const scene *s, const ray *r, double t_min, double t_max, hit_record *rec) {
    int hit_anything = 0;
    double closest_so_far = t_max;

    scene_node *node = s->first;
    for (int i = 0;;i++) {
        if (node == NULL)
            break;

        switch(node->type) {
            case SPHERE:
                if (sphere_hit((sphere *)node->geometry, r, t_min, closest_so_far, rec)) {
                    hit_anything = 1;
                    closest_so_far = rec->t;
                    rec->mat = node->mat;
                }
        }

        node = (scene_node *)node->next;
    }

    return hit_anything;
}
