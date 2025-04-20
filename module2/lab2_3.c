#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define CLEAR_BUFFER while (getchar() != '\n')
#define MAX_OPERATIONS 10

typedef double (*OperationFunc)(double, double, int*);

typedef struct {
    char name[20];
    char symbol;
    OperationFunc operation;
} Operation;

double add(double a, double b, int* error) { return a + b; }
double sub(double a, double b, int* error) { return a - b; }
double mul(double a, double b, int* error) { return a * b; }
double divide(double a, double b, int* error) {
    if(b == 0) { *error = 1; return 0; }
    return a / b;
}
double power(double a, double b, int* error) { return pow(a, b); }
double modulus(double a, double b, int* error) {
    if(b == 0) { *error = 1; return 0; }
    return fmod(a, b);
}

Operation operations[] = {
    {"Сложение", '+', add},
    {"Вычитание", '-', sub},
    {"Умножение", '*', mul},
    {"Деление", '/', divide},
    {"Степень", '^', power},
    {"Остаток", '%', modulus}
};

int op_count = sizeof(operations) / sizeof(operations[0]);

void printMenu() {
    system("clear || cls");
    printf("\n\033[1;35m=== КАЛЬКУЛЯТОР ===\033[0m\n");
    for(int i = 0; i < op_count; i++) {
        printf("\033[1;36m%d. %s (%c)\033[0m\n", 
              i+1, operations[i].name, operations[i].symbol);
    }
    printf("\033[1;31m%d. Выход\033[0m\n", op_count+1);
    printf("\n\033[1;33mВведите номер операции: \033[0m");
}

double getNumber(const char *prompt) {
    char buffer[100];
    double value;
    
    while(1) {
        printf("%s: ", prompt);
        fflush(stdout); // Явный сброс буфера вывода
        
        if(fgets(buffer, sizeof(buffer), stdin)) {
            if(sscanf(buffer, "%lf", &value) == 1) {
                return value;
            }
        }
        
        // Очистка буфера только при ошибке
        printf("\033[1;31mОшибка! Введите число: \033[0m");
        CLEAR_BUFFER;
    }
}

void printResult(double result, int error) {
    if(error) {
        printf("\033[1;31mОшибка в вычислениях!\033[0m\n");
    } else {
        printf("\n\033[1;32mРезультат: ");
        if(floor(result) == result) printf("%.0f", result);
        else printf("%.4f", result);
        printf("\033[0m\n");
    }
    
    printf("\nНажмите Enter чтобы продолжить...");
    CLEAR_BUFFER;
    getchar();
}

void performOperation(int choice) {
    system("clear || cls");
    printf("\n\033[1;35m=== %s ===\033[0m\n", operations[choice].name);
    
    double a = getNumber("Введите первое число");
    double b = getNumber("Введите второе число");
    int error = 0;
    
    double result = operations[choice].operation(a, b, &error);
    printResult(result, error);
}

int main() {
    int choice;
    
    do {
        printMenu();
        int result = scanf("%d", &choice);
        CLEAR_BUFFER; // Важное исправление: очистка буфера после ввода
        
        if(result != 1 || choice < 1 || choice > op_count + 1) {
            printf("\033[1;31mНеверный выбор!\033[0m\n");
            sleep(1);
            continue;
        }
        
        if(choice == op_count + 1) break;
        
        performOperation(choice - 1);
    } while(1);
    
    printf("\n\033[1;34mРабота программы завершена!\033[0m\n");
    return 0;
}