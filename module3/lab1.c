#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Функция для вычисления площадей квадратов
void calculate_areas(int start, int end, int *sides) {
    for (int i = start; i < end; i++) {
        int side = sides[i];
        printf("Process %d: Side = %d, Area = %d\n", getpid(), side, side * side);
    }
}

int main(int argc, char *argv[]) {
    // Проверка на наличие аргументов
    if (argc < 2) {
        fprintf(stderr, "Usage: %s side1 side2 side3 ...\n", argv[0]);
        return 1;
    }

    // Преобразование аргументов командной строки в массив целых чисел
    int num_sides = argc - 1;
    int *sides = malloc(num_sides * sizeof(int));
    for (int i = 0; i < num_sides; i++) {
        sides[i] = atoi(argv[i + 1]);
    }

    // Создание дочернего процесса
    pid_t pid = fork();

    if (pid < 0) {
        // Ошибка при создании процесса
        perror("Fork failed");
        free(sides);
        return 1;
    } else if (pid == 0) {
        // Дочерний процесс
        int half = num_sides / 2;
        calculate_areas(0, half, sides);
    } else {
        // Родительский процесс
        int half = num_sides / 2;
        calculate_areas(half, num_sides, sides);

        // Ожидание завершения дочернего процесса
        wait(NULL);
    }

    // Освобождение памяти
    free(sides);
    return 0;
}