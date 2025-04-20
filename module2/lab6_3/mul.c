#include "operation.h"

static double mul(double a, double b, int* error) {
    return a * b;
}

static Operation op = {
    .name = "Умножение",
    .symbol = '*',
    .func = mul
};

Operation* getOperation() {
    return &op;
}
