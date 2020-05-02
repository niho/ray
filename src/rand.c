#include <stdlib.h>

#include "rand.h"

double rand_double() {
    return rand() / (RAND_MAX + 1.0);
}

double rand_double_clamp(double min, double max) {
    return min + (max-min) * rand_double();
}
