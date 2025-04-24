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
    printf("Debug: Entering sum function.\n");
    if (argc < 2) {
        printf("Usage: sum num1 num2 ...\n");
        return;
    }

    int total = 0;
    for (int i = 1; i < argc; i++) {
        printf("Debug: Adding %d to total.\n", atoi(argv[i]));
        total += atoi(argv[i]);
    }
    printf("Sum: %d\n", total);
    printf("Debug: Exiting sum function.\n");
}

// Главная функция интерпретатора
int main() {
    char input[MAX_INPUT]; // Буфер для ввода пользователя
    char *args[MAX_INPUT]; // Массив аргументов

    printf("Debug: Interpreter started.\n");

    while (1) {
        printf("> "); // Выводим приглашение
        fflush(stdout);

        // Читаем ввод пользователя
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nDebug: EOF detected. Exiting...\n");
            break; // Выход из цикла при EOF (Ctrl+D)
        }

        printf("Debug: User input received: '%s'\n", input);

        // Удаляем завершающий символ новой строки
        input[strcspn(input, "\n")] = '\0';

        // Разбиваем ввод на токены
        printf("Debug: Tokenizing input...\n");
        tokenize_input(input, args);

        // Проверяем, что ввод не пустой
        if (args[0] == NULL) {
            printf("Debug: Input is empty. Skipping...\n");
            continue;
        }

        // Обработка встроенной команды exit
        if (strcmp(args[0], "exit") == 0) {
            printf("Debug: Exit command detected.\n");
            printf("Exiting...\n");
            break;
        }

        // Подсчет количества аргументов
        int argc = 0;
        while (args[argc] != NULL) {
            argc++;
        }
        printf("Debug: Parsed %d arguments.\n", argc);

        // Создаем дочерний процесс
        pid_t pid = fork();
        printf("Debug: Fork called. Fork returned PID = %d\n", pid);

        if (pid < 0) {
            perror("Fork failed");
        } else if (pid == 0) {
            // Дочерний процесс
            printf("Debug: Child process started. PID = %d\n", getpid());
            if (strcmp(args[0], "sum") == 0) {
                printf("Debug: Child process executing 'sum' command.\n");
                sum(argc, args);
                printf("Debug: Child process finished executing 'sum'. Exiting...\n");
                exit(EXIT_SUCCESS); // Завершаем дочерний процесс
            } else {
                // Попытка выполнить внешнюю команду
                printf("Debug: Child process attempting to execute external command '%s'.\n", args[0]);
                if (execvp(args[0], args) == -1) {
                    perror("Exec failed");
                }
                printf("Debug: External command execution failed. Exiting child process...\n");
                exit(EXIT_FAILURE); // Если execvp не сработал
            }
        } else {
            // Родительский процесс
            printf("Debug: Parent process waiting for child (PID = %d) to finish.\n", pid);
            wait(NULL); // Ждем завершения дочернего процесса
            printf("Debug: Child process has finished. Parent process continuing...\n");
        }
    }

    printf("Debug: Interpreter exiting.\n");
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
