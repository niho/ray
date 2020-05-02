#ifndef COLOR_H
#define COLOR_H

typedef struct color {
    int r;
    int g;
    int b;
} color;

color color_sample(double r, double g, double b, int samples_per_pixel);

#endif
