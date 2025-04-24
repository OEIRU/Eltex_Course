#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Функция для вычисления площадей квадратов
void calculate_areas(int start, int end, int *sides) {
    for (int i = start; i < end; i++) {
        int side = sides[i];
        printf("Process %d: Calculating area for side %d -> Area = %d\n", getpid(), side, side * side);
    }
}

int main(int argc, char *argv[]) {
    printf("Debug: Program started. PID = %d\n", getpid());

    // Проверка на наличие аргументов
    if (argc < 2) {
        fprintf(stderr, "Usage: %s side1 side2 side3 ...\n", argv[0]);
        return 1;
    }

    // Преобразование аргументов командной строки в массив целых чисел
    int num_sides = argc - 1;
    int *sides = malloc(num_sides * sizeof(int));
    if (sides == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    printf("Debug: Parsing %d sides from command line arguments.\n", num_sides);
    for (int i = 0; i < num_sides; i++) {
        sides[i] = atoi(argv[i + 1]);
        printf("Debug: Parsed side[%d] = %d\n", i, sides[i]);
    }

    // Создание дочернего процесса
    pid_t pid = fork();
    printf("Debug: Fork called. Fork returned PID = %d\n", pid);

    if (pid < 0) {
        // Ошибка при создании процесса
        perror("Fork failed");
        free(sides);
        return 1;
    } else if (pid == 0) {
        // Дочерний процесс
        printf("Debug: Child process started. PID = %d\n", getpid());
        int half = num_sides / 2;
        printf("Debug: Child process will calculate areas for sides [%d, %d).\n", 0, half);
        calculate_areas(0, half, sides);
        printf("Debug: Child process finished calculations.\n");
    } else {
        // Родительский процесс
        printf("Debug: Parent process continues execution. PID = %d\n", getpid());
        int half = num_sides / 2;
        printf("Debug: Parent process will calculate areas for sides [%d, %d).\n", half, num_sides);
        calculate_areas(half, num_sides, sides);
        printf("Debug: Parent process waiting for child to finish.\n");

        // Ожидание завершения дочернего процесса
        wait(NULL);
        printf("Debug: Child process has finished. Parent process exiting.\n");
    }

    // Освобождение памяти
    printf("Debug: Freeing allocated memory.\n");
    free(sides);
    printf("Debug: Program finished.\n");
    return 0;
}