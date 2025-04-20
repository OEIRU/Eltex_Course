#include "operation.h"
#include <math.h>

static double power(double a, double b, int* error) {
    return pow(a, b);
}

static Operation op = {
    .name = "Степень",
    .symbol = '^',
    .func = power
};

Operation* getOperation() {
    return &op;
}
