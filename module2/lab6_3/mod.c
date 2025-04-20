#include "operation.h"
#include <math.h>

static double mod(double a, double b, int* error) {
    if (b == 0) {
        *error = 1;
        return 0;
    }
    return fmod(a, b);
}

static Operation op = {
    .name = "Остаток",
    .symbol = '%',
    .func = mod
};

Operation* getOperation() {
    return &op;
}
