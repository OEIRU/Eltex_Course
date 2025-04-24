#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <количество чисел>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int count = atoi(argv[1]);
    if (count <= 0) {
        fprintf(stderr, "Количество чисел должно быть положительным.\n");
        exit(EXIT_FAILURE);
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("Ошибка создания канала");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка при fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Дочерний процесс
        close(pipe_fd[0]); // Закрываем конец чтения канала

        srand(time(NULL)); // Инициализируем генератор случайных чисел
        for (int i = 0; i < count; i++) {
            int random_number = rand() % 100; // Генерируем случайное число
            write(pipe_fd[1], &random_number, sizeof(random_number)); // Отправляем число родителю
            printf("Дочерний процесс: отправлено число %d\n", random_number);
        }

        close(pipe_fd[1]); // Закрываем конец записи канала
        exit(EXIT_SUCCESS);
    } else { // Родительский процесс
        close(pipe_fd[1]); // Закрываем конец записи канала

        FILE *file = fopen("output.txt", "w");
        if (file == NULL) {
            perror("Ошибка открытия файла");
            exit(EXIT_FAILURE);
        }

        int received_number;
        for (int i = 0; i < count; i++) {
            read(pipe_fd[0], &received_number, sizeof(received_number)); // Читаем число из канала
            printf("Родительский процесс: получено число %d\n", received_number);
            fprintf(file, "%d\n", received_number); // Записываем число в файл
        }

        close(pipe_fd[0]); // Закрываем конец чтения канала
        fclose(file); // Закрываем файл
        wait(NULL); // Ожидаем завершения дочернего процесса
    }

    return 0;
}
