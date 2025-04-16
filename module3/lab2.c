#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 256

// Функция для разбиения строки на токены
void tokenize_input(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " \n");
    while (args[i] != NULL) {
        args[++i] = strtok(NULL, " \n");
    }
}

// Функция для вычисления суммы аргументов
void sum(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: sum num1 num2 ...\n");
        return;
    }

    int total = 0;
    for (int i = 1; i < argc; i++) {
        total += atoi(argv[i]);
    }
    printf("Sum: %d\n", total);
}

// Главная функция интерпретатора
int main() {
    char input[MAX_INPUT]; // Буфер для ввода пользователя
    char *args[MAX_INPUT]; // Массив аргументов

    while (1) {
        printf("> "); // Выводим приглашение
        fflush(stdout);

        // Читаем ввод пользователя
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break; // Выход из цикла при EOF (Ctrl+D)
        }

        // Удаляем завершающий символ новой строки
        input[strcspn(input, "\n")] = '\0';

        // Разбиваем ввод на токены
        tokenize_input(input, args);

        // Проверяем, что ввод не пустой
        if (args[0] == NULL) {
            continue;
        }

        // Обработка встроенной команды exit
        if (strcmp(args[0], "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Подсчет количества аргументов
        int argc = 0;
        while (args[argc] != NULL) {
            argc++;
        }

        // Создаем дочерний процесс
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
        } else if (pid == 0) {
            // Дочерний процесс
            if (strcmp(args[0], "sum") == 0) {
                sum(argc, args);
                exit(EXIT_SUCCESS); // Завершаем дочерний процесс
            } else {
                // Попытка выполнить внешнюю команду
                if (execvp(args[0], args) == -1) {
                    perror("Exec failed");
                }
                exit(EXIT_FAILURE); // Если execvp не сработал
            }
        } else {
            // Родительский процесс
            wait(NULL); // Ждем завершения дочернего процесса
        }
    }

    return 0;
}

/*
Функции в коде:
> sum 1 2 3 
> concat Hello World
> max 10 20 5 30
> longest short longer longest
> unknown_command
> exit
*/
