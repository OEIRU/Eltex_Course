#ifndef OPERATION_H
#define OPERATION_H

typedef double (*OperationFunc)(double, double, int*);

typedef struct {
    const char* name;
    char symbol;
    OperationFunc func;
} Operation;

// Функция, экспортируемая каждой библиотекой
Operation* getOperation();

#endif
