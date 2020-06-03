#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#include "vec3.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "camera.h"
#include "color.h"
#include "rand.h"
#include "material.h"

static scene world;
static camera cam;

const int w = 640;
const int h = 480;
const int samples_per_pixel = 200;
const int max_depth = 50;

static const sphere sphere1 = {
    .center = {0.0, 0.0, -1.0},
    .radius = 0.5
};
static const sphere sphere2 = {
    .center = {0.0, -100.5, -1.0},
    .radius = 100
};
static const sphere sphere3 = {
    .center = {1.0, 0.0, -1.0},
    .radius = 0.4
};
static const sphere sphere4 = {
    .center = {-1.0, 0.5, -2.0},
    .radius = 1
};
static const sphere sphere5 = {
    .center = {0.5, -0.4, -0.8},
    .radius = 0.2
};
static const sphere sphere6 = {
    .center = {0.5, 2, 0.8},
    .radius = 1
};


static const material mat_diffuse_red = {
    .type = DIFFUSE_LAMBERTIAN,
    .albedo = {0.7, 0.3, 0.3}
};
static const material mat_diffuse_green = {
    .type = DIFFUSE_LAMBERTIAN,
    .albedo = {0.28, 0.3, 0.29}
};

static const material mat_metal_1 = {
    .type = METAL,
    .albedo = {0.8, 0.6, 0.2},
    .fuzz = 0.8
};
static const material mat_metal_2 = {
    .type = METAL,
    .albedo = {0.8, 0.8, 0.8},
    .fuzz = 0.1
};
static const material mat_glass_1 = {
    .type = DIELECTRIC,
    .albedo = {1.0, 1.0, 1.0},
    .ref_idx = 1.5
};


vec3 ray_color(const ray *r, const scene *world, int depth) {
    hit_record rec;

    if (depth <= 0)
        return Z;

    if (scene_hit(world, r, 0.001, infinity, &rec)) {
        ray scattered;
        vec3 attenuation;

        if (material_scatter(rec.mat, r, &rec, &attenuation, &scattered)) {
            vec3 color = ray_color(&scattered, world, depth-1);
            vec3_mul_m(&color, &attenuation);
            return color;
        } else {
            return Z;
        }
    }

    vec3 unit_direction = vec3_unit(&r->dir);
    double t = 0.5 * (unit_direction.y + 1.0);
    vec3 c1 = {1.0, 1.0, 1.0};
    vec3 c2 = {0.5, 0.7, 1.0};
    c1 = vec3_mul_t(&c1, 1.0 - t);
    c2 = vec3_mul_t(&c2, t);
    return vec3_add(&c1, &c2);
}

static void random_scene(scene *world) {
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            vec3 center = {
                .x = a + 0.9 * rand_double(),
                .y = 2.0 * rand_double(),
                .z = b + 0.9 * rand_double()
            };

            sphere *s = (sphere *)malloc(sizeof(sphere));
            s->center = center;
            s->radius = 0.2;

            material *mat = (material *)malloc(sizeof(material));
            mat->type = METAL;
            mat->fuzz = rand_double_clamp(0.0, 0.8);
            mat->albedo.x = 0.8;
            mat->albedo.y = 0.8;
            mat->albedo.z = 0.8;

            scene_add(world, SPHERE, s, mat);
        }
    }
}

static void *render_scanline(void *arg) {
    int j = (int)arg;

    color *scanline;
    scanline = malloc(sizeof(color) * w);

    for (int i = 0; i < w; ++i) {
        vec3 pixel = {0,0,0};
        for (int s = 0; s < samples_per_pixel; ++s) {
            double u = ((double)i + rand_double()) / w;
            double v = ((double)j + rand_double()) / h;

            ray r = camera_get_ray(&cam, u, v);
            vec3 sample = ray_color(&r, &world, max_depth);

            vec3_add_m(&pixel, &sample);
        }

        scanline[i] = color_sample(pixel.x, pixel.y, pixel.z, samples_per_pixel);

        fprintf(stderr, ".");
    }

    return scanline;
}

int main(int argc, char **argv) {
    printf("P3\n%d %d\n255\n", w, h);

    const vec3 lookfrom = {13,2,3};
    const vec3 lookat = {0,0,-1};
    const vec3 vup = {0,1,0};
    double vfov = 25;
    double aspect = (double)w/(double)h;
    double aperture = 0.1;
    vec3 dist_v = vec3_sub(&lookfrom, &lookat);
    double focus = vec3_length(&dist_v);
    camera_init(&cam, &lookfrom, &lookat, &vup, vfov, aspect, aperture, focus);

    scene_init(&world);
    scene_add(&world, SPHERE, &sphere1, &mat_diffuse_red);
    scene_add(&world, SPHERE, &sphere2, &mat_diffuse_green);
    scene_add(&world, SPHERE, &sphere3, &mat_metal_1);
    scene_add(&world, SPHERE, &sphere4, &mat_glass_1);
    scene_add(&world, SPHERE, &sphere5, &mat_metal_2);
    scene_add(&world, SPHERE, &sphere6, &mat_glass_1);

    random_scene(&world);

    pthread_t threads[h-1];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int j = h-1; j >= 0; --j) {
        if (pthread_create(&threads[j], &attr, render_scanline, (void *)j) != 0) {
            perror("pthread_create failed");
            exit(1);
        }
    }

    pthread_attr_destroy(&attr);

    for(int t = h-1; t >= 0; --t) {
        color *scanline = NULL;
        if (pthread_join(threads[t], (void **)&scanline) != 0) {
            perror("pthread_join failed");
            exit(1);
        }

        for(int x = 0; x < w; x++) {
            printf("%d %d %d\n", scanline[x].r, scanline[x].g, scanline[x].b);
        }
    }

    scene_clear(&world);

    fprintf(stderr, "\nDone.\n");
}
