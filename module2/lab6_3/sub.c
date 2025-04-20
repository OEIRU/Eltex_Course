#include "operation.h"

static double sub(double a, double b, int* error) {
    return a - b;
}

static Operation op = {
    .name = "Вычитание",
    .symbol = '-',
    .func = sub
};

Operation* getOperation() {
    return &op;
}
