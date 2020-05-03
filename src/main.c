#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vec3.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "camera.h"
#include "color.h"
#include "rand.h"
#include "material.h"


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
    .center = {-1.0, -0.25, -1.0},
    .radius = 0.35
};
static const sphere sphere5 = {
    .center = {0.5, -0.4, -0.8},
    .radius = 0.2
};

static const material mat_diffuse_red = {
    .type = DIFFUSE_LAMBERTIAN,
    .albedo = {0.7, 0.3, 0.3}
};
static const material mat_diffuse_green = {
    .type = DIFFUSE_LAMBERTIAN,
    .albedo = {0.2, 0.8, 0.3}
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

int main(int argc, char **argv) {
    const int w = 800;
    const int h = 400;
    const int samples_per_pixel = 200;
    const int max_depth = 50;

    printf("P3\n%d %d\n255\n", w, h);

    camera cam = {
        .lower_left_corner = {-2.0, -1.0, -1.0},
        .horizontal = {4.0, 0.0, 0.0},
        .vertical = {0.0, 2.0, 0.0},
        .origin = {0.0, 0.0, 0.0}
    };

    scene world;
    scene_init(&world);
    scene_add(&world, SPHERE, &sphere1, &mat_diffuse_red);
    scene_add(&world, SPHERE, &sphere2, &mat_diffuse_green);
    scene_add(&world, SPHERE, &sphere3, &mat_metal_1);
    scene_add(&world, SPHERE, &sphere4, &mat_glass_1);
    scene_add(&world, SPHERE, &sphere5, &mat_metal_2);

    for (int j = h-1; j >= 0; --j) {
        fprintf(stderr, "Scanlines remaining: %d\n", j);
        for (int i = 0; i < w; ++i) {
            
            vec3 pixel = {0,0,0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = ((double)i + rand_double()) / w;
                double v = ((double)j + rand_double()) / h;

                ray r = camera_get_ray(&cam, u, v);
                vec3 sample = ray_color(&r, &world, max_depth);

                vec3_add_m(&pixel, &sample);
            }

            color c = color_sample(pixel.x, pixel.y, pixel.z, samples_per_pixel);
            printf("%d %d %d\n", c.r, c.g, c.b);
        }
    }

    scene_clear(&world);

    fprintf(stderr, "\nDone.\n");
}
