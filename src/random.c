#include <math.h>
#include <stdlib.h>

#include "random.h"

double rand_fp() {
    int rand_int = rand() - 1;
    if(rand_int < 0) {
        rand_int = 0;
    }
    return (double)rand_int / RAND_MAX;
}