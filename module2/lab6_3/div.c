#include "operation.h"

static double divide(double a, double b, int* error) {
    if (b == 0) {
        *error = 1;
        return 0;
    }
    return a / b;
}

static Operation op = {
    .name = "Деление",
    .symbol = '/',
    .func = divide
};

Operation* getOperation() {
    return &op;
}
