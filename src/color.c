#include <math.h>

#include "color.h"

static inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

color color_sample(double r, double g, double b, int samples_per_pixel) {
    double scale = 1.0 / samples_per_pixel;
    color c = {
        .r = 256 * clamp(sqrt(scale * r), 0.0, 0.999),
        .g = 256 * clamp(sqrt(scale * g), 0.0, 0.999),
        .b = 256 * clamp(sqrt(scale * b), 0.0, 0.999)
    };
    return c;
}
