#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

#define CLEAR_BUFFER while (getchar() != '\n')

void printMenu();
double getNumberInput(const char *prompt);
void printResult(double result);
void performOperation(int choice);

int main() {
    int choice;
    
    do {
        printMenu();
        choice = getNumberInput("Выберите операцию (1-5)");
        
        if(choice < 1 || choice > 5) {
            printf("\033[1;31mОшибка: Введите число от 1 до 5!\033[0m\n");
            sleep(1);
            continue;
        }
        
        if(choice == 5) break;
        
        performOperation(choice);
        
    } while(1);
    
    printf("\n\033[1;34mРабота программы завершена. До свидания!\033[0m\n");
    return 0;
}

void printMenu() {
    system("clear || cls");
    printf("\n\033[1;35m=== КАЛЬКУЛЯТОР ===\033[0m\n");
    printf("\033[1;36m1. Сложение\033[0m\n");
    printf("\033[1;36m2. Вычитание\033[0m\n");
    printf("\033[1;36m3. Умножение\033[0m\n");
    printf("\033[1;36m4. Деление\033[0m\n");
    printf("\033[1;31m5. Выход\033[0m\n");
    printf("\n\033[1;33mВведите номер операции: \033[0m");
}

double getNumberInput(const char *prompt) {
    char buffer[100];
    double value;
    int success = 0;
    
    do {
        printf("%s: ", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\033[1;31mОшибка чтения ввода!\033[0m\n");
            continue;
        }
        
        success = sscanf(buffer, "%lf", &value);
        
        if (!success) {
            printf("\033[1;31mНекорректный ввод! Введите число.\033[0m\n");
        }
    } while (!success);
    
    return value;
}

void printResult(double result) {
    printf("\n\033[1;32mРезультат: ");
    
    if(fmod(result, 1.0) == 0.0) {
        printf("%.0f\033[0m\n", result);
    } else {
        printf("%.4f\033[0m\n", result);
    }
    
    printf("\nНажмите Enter чтобы продолжить...");
    CLEAR_BUFFER;
    getchar();
}

void performOperation(int choice) {
    system("clear || cls");
    printf("\n\033[1;35m=== ");
    
    double a = getNumberInput("Введите первое число");
    double b = getNumberInput("Введите второе число");
    double result = 0;
    
    switch(choice) {
        case 1:
            result = a + b;
            printf("\033[1;35mСложение: \033[0m");
            break;
        case 2:
            result = a - b;
            printf("\033[1;35mВычитание: \033[0m");
            break;
        case 3:
            result = a * b;
            printf("\033[1;35mУмножение: \033[0m");
            break;
        case 4:
            if(b == 0) {
                printf("\033[1;31mОшибка: Деление на ноль!\033[0m\n");
                sleep(2);
                return;
            }
            result = a / b;
            printf("\033[1;35mДеление: \033[0m");
            break;
    }
    
    printResult(result);
}