// Разобраться с библиотеками. 
// Пока не работает

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
#include <math.h>
#include "operation.h"

#define MAX_OPS 32
#define LIB_FOLDER "libs"
#define CLEAR_BUFFER while (getchar() != '\n')

typedef struct {
    void* handle;
    Operation* op;
} LoadedOp;

LoadedOp loadedOps[MAX_OPS];
int loadedCount = 0;

void loadLibraries(const char* folder) {
    struct dirent* entry;
    DIR* dir = opendir(folder);
    if (!dir) {
        perror("Не удалось открыть папку с библиотеками");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!strstr(entry->d_name, ".so")) continue;

        char path[256];
        snprintf(path, sizeof(path), "%s/%s", folder, entry->d_name);

        void* handle = dlopen(path, RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Ошибка загрузки %s: %s\n", entry->d_name, dlerror());
            continue;
        }

        Operation* (*getOp)() = dlsym(handle, "getOperation");
        if (!getOp) {
            fprintf(stderr, "Ошибка импорта функции getOperation из %s\n", entry->d_name);
            dlclose(handle);
            continue;
        }

        Operation* op = getOp();
        if (loadedCount < MAX_OPS) {
            loadedOps[loadedCount].handle = handle;
            loadedOps[loadedCount].op = op;
            loadedCount++;
        }
    }

    closedir(dir);
}

double getNumber(const char* prompt) {
    char buffer[100];
    double value;
    while (1) {
        printf("%s: ", prompt);
        fflush(stdout);
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%lf", &value) == 1)
            return value;
        printf("\033[1;31mОшибка! Введите число:\033[0m ");
        CLEAR_BUFFER;
    }
}

void printMenu() {
    system("clear || cls");
    printf("\n\033[1;35m=== КАЛЬКУЛЯТОР ===\033[0m\n");
    for (int i = 0; i < loadedCount; ++i) {
        printf("\033[1;36m%d. %s (%c)\033[0m\n", i+1, loadedOps[i].op->name, loadedOps[i].op->symbol);
    }
    printf("\033[1;31m%d. Выход\033[0m\n", loadedCount+1);
    printf("\n\033[1;33mВведите номер операции: \033[0m");
}

void performOperation(int index) {
    Operation* op = loadedOps[index].op;
    system("clear || cls");
    printf("\n\033[1;35m=== %s ===\033[0m\n", op->name);

    double a = getNumber("Введите первое число");
    double b = getNumber("Введите второе число");
    int error = 0;
    double result = op->func(a, b, &error);

    if (error)
        printf("\n\033[1;31mОшибка в вычислениях!\033[0m\n");
    else
        printf("\n\033[1;32mРезультат: %.4f\033[0m\n", result);

    printf("\nНажмите Enter чтобы продолжить...");
    CLEAR_BUFFER;
    getchar();
}

void cleanup() {
    for (int i = 0; i < loadedCount; ++i)
        dlclose(loadedOps[i].handle);
}

int main(int argc, char* argv[]) {
    const char* folder = (argc > 1) ? argv[1] : LIB_FOLDER;
    loadLibraries(folder);

    if (loadedCount == 0) {
        fprintf(stderr, "Нет доступных операций.\n");
        return EXIT_FAILURE;
    }

    int choice;
    do {
        printMenu();
        int res = scanf("%d", &choice);
        CLEAR_BUFFER;

        if (res != 1 || choice < 1 || choice > loadedCount + 1) {
            printf("\033[1;31mНеверный выбор!\033[0m\n");
            sleep(1);
            continue;
        }

        if (choice == loadedCount + 1) break;

        performOperation(choice - 1);
    } while (1);

    cleanup();
    printf("\n\033[1;34mРабота программы завершена!\033[0m\n");
    return 0;
}
