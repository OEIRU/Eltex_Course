#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFFER_SIZE 256

volatile sig_atomic_t file_blocked = 0; // Флаг блокировки файла

void handle_sigusr1(int sig) {
    if (sig == SIGUSR1) {
        file_blocked = 1; // Блокировка файла
        printf("Дочерний процесс: получено SIGUSR1 - файл заблокирован.\n");
    }
}

void handle_sigusr2(int sig) {
    if (sig == SIGUSR2) {
        file_blocked = 0; // Разблокировка файла
        printf("Дочерний процесс: получено SIGUSR2 - файл разблокирован.\n");
    }
}

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
    int ack_pipe_fd[2]; // Канал для подтверждений

    if (pipe(pipe_fd) == -1 || pipe(ack_pipe_fd) == -1) {
        perror("Ошибка создания канала");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка при fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Дочерний процесс
        close(pipe_fd[0]); // Закрываем конец чтения основного канала
        close(ack_pipe_fd[1]); // Закрываем конец записи канала подтверждений

        // Устанавливаем обработчики сигналов
        signal(SIGUSR1, handle_sigusr1);
        signal(SIGUSR2, handle_sigusr2);

        srand(time(NULL)); // Инициализируем генератор случайных чисел

        for (int i = 0; i < count; i++) {
            // Читаем файл, если он не заблокирован
            while (file_blocked) {
                sleep(1); // Ожидаем разблокировки файла
            }

            FILE *file = fopen("output.txt", "r");
            if (file == NULL) {
                perror("Ошибка открытия файла для чтения");
                exit(EXIT_FAILURE);
            }

            printf("Дочерний процесс: чтение содержимого файла:\n");
            char buffer[BUFFER_SIZE];
            while (fgets(buffer, sizeof(buffer), file)) {
                printf("%s", buffer);
            }
            fclose(file);

            // Генерируем и отправляем новое случайное число
            int random_number = rand() % 100;
            write(pipe_fd[1], &random_number, sizeof(random_number));
            printf("Дочерний процесс: отправлено число %d\n", random_number);

            // Отправляем подтверждение родительскому процессу
            char ack = 'A'; // Подтверждение
            write(ack_pipe_fd[1], &ack, sizeof(ack));
        }

        close(pipe_fd[1]); // Закрываем конец записи основного канала
        close(ack_pipe_fd[1]); // Закрываем конец записи канала подтверждений
        exit(EXIT_SUCCESS);
    } else { // Родительский процесс
        close(pipe_fd[1]); // Закрываем конец записи основного канала
        close(ack_pipe_fd[0]); // Закрываем конец чтения канала подтверждений

        FILE *file = fopen("output.txt", "w");
        if (file == NULL) {
            perror("Ошибка открытия файла");
            exit(EXIT_FAILURE);
        }

        int received_number;
        for (int i = 0; i < count; i++) {
            // Блокируем файл перед модификацией
            kill(pid, SIGUSR1);
            sleep(1); // Даём время дочернему процессу обработать сигнал

            // Ждём подтверждения от дочернего процесса
            char ack;
            read(ack_pipe_fd[0], &ack, sizeof(ack));

            read(pipe_fd[0], &received_number, sizeof(received_number)); // Читаем число из канала
            printf("Родительский процесс: получено число %d\n", received_number);

            // Модифицируем файл
            fprintf(file, "%d\n", received_number);

            // Разблокируем файл после модификации
            kill(pid, SIGUSR2);
            sleep(1); // Даём время дочернему процессу обработать сигнал
        }

        close(pipe_fd[0]); // Закрываем конец чтения основного канала
        close(ack_pipe_fd[0]); // Закрываем конец чтения канала подтверждений
        fclose(file); // Закрываем файл
        wait(NULL); // Ожидаем завершения дочернего процесса
    }

    return 0;
}