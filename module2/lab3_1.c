#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define BUF_SIZE 1024

typedef struct {
    mode_t binary;    // Битовое представление
    int octal;        // Восьмеричное представление
    char symbolic[10];// Символьное представление
} Permissions;

// Прототипы функций
void parse_symbolic(const char *sym, Permissions *perm);
void parse_octal(int oct, Permissions *perm);
void update_symbolic(Permissions *perm);
void print_permissions(const Permissions *perm);
void process_chmod_commands(Permissions *perm, const char *command);
void get_file_permissions(const char *filename, Permissions *perm);

int main() {
    char input[BUF_SIZE];
    Permissions perm = {0};
    int running = 1;

    while(running) {
        printf("\n=== Меню управления правами ===\n");
        printf("1. Ввод прав в символьном/цифровом формате\n");
        printf("2. Показать права файла\n");
        printf("3. Изменить права (chmod-подобный синтаксис)\n");
        printf("4. Выход\n");
        printf("Выберите действие: ");
        
        fgets(input, BUF_SIZE, stdin);
        int choice = atoi(input);

        switch(choice) {
            case 1: {
                printf("Введите права (например, 755 или rwxr-xr-x): ");
                fgets(input, BUF_SIZE, stdin);
                input[strcspn(input, "\n")] = '\0';

                if (input[0] >= '0' && input[0] <= '9') {
                    parse_octal(strtol(input, NULL, 8), &perm);
                } else {
                    parse_symbolic(input, &perm);
                }
                print_permissions(&perm);
                break;
            }
            case 2: {
                printf("Введите имя файла: ");
                fgets(input, BUF_SIZE, stdin);
                input[strcspn(input, "\n")] = '\0';
                get_file_permissions(input, &perm);
                print_permissions(&perm);
                break;
            }
            case 3: {
                printf("Введите команду изменения прав (например, u+rwx,g-x): ");
                fgets(input, BUF_SIZE, stdin);
                input[strcspn(input, "\n")] = '\0';
                process_chmod_commands(&perm, input);
                print_permissions(&perm);
                break;
            }
            case 4:
                running = 0;
                break;
            default:
                printf("Неверный выбор!\n");
        }
    }
    return 0;
}

void parse_symbolic(const char *sym, Permissions *perm) {
    mode_t modes = 0;
    const char *p = sym;
    
    // User
    modes |= (*p++ == 'r') ? S_IRUSR : 0;
    modes |= (*p++ == 'w') ? S_IWUSR : 0;
    modes |= (*p++ == 'x') ? S_IXUSR : 0;
    
    // Group
    modes |= (*p++ == 'r') ? S_IRGRP : 0;
    modes |= (*p++ == 'w') ? S_IWGRP : 0;
    modes |= (*p++ == 'x') ? S_IXGRP : 0;
    
    // Other
    modes |= (*p++ == 'r') ? S_IROTH : 0;
    modes |= (*p++ == 'w') ? S_IWOTH : 0;
    modes |= (*p   == 'x') ? S_IXOTH : 0;
    
    perm->binary = modes;
    perm->octal = modes & 0777;
    update_symbolic(perm);
}

void parse_octal(int oct, Permissions *perm) {
    perm->octal = oct & 0777;
    perm->binary = oct & 0777;
    update_symbolic(perm);
}

void update_symbolic(Permissions *perm) {
    snprintf(perm->symbolic, 10, "%c%c%c%c%c%c%c%c%c",
        (perm->binary & S_IRUSR) ? 'r' : '-',
        (perm->binary & S_IWUSR) ? 'w' : '-',
        (perm->binary & S_IXUSR) ? 'x' : '-',
        (perm->binary & S_IRGRP) ? 'r' : '-',
        (perm->binary & S_IWGRP) ? 'w' : '-',
        (perm->binary & S_IXGRP) ? 'x' : '-',
        (perm->binary & S_IROTH) ? 'r' : '-',
        (perm->binary & S_IWOTH) ? 'w' : '-',
        (perm->binary & S_IXOTH) ? 'x' : '-');
}

void print_permissions(const Permissions *perm) {
    printf("\nТекущие права:\n");
    printf("Символьный формат: %s\n", perm->symbolic);
    printf("Цифровой формат:  %04o\n", perm->octal);
    printf("Битовое представление: ");
    for(int i = 8; i >= 0; i--) {
        printf("%d", (perm->binary >> i) & 1);
        if(i % 3 == 0 && i != 0) printf(" ");
    }
    printf("\n\n");
}

void process_chmod_commands(Permissions *perm, const char *command) {
    char *cmd_copy = strdup(command);
    char *token = strtok(cmd_copy, ",");
    
    while(token != NULL) {
        // Пропускаем пробелы
        char *part = token;
        while(*part == ' ') part++;
        
        // Разделяем на части: [флаги][операция][права]
        char *op_ptr = part;
        while(*op_ptr && !strchr("+-=", *op_ptr)) op_ptr++;
        
        if(*op_ptr == '\0') {
            printf("Неверный формат команды: %s\n", token);
            continue;
        }
        
        char op = *op_ptr;
        *op_ptr = '\0';
        char *rights = op_ptr + 1;
        
        // Определяем область действия
        mode_t mask = 0;
        char *who = part;
        while(*who) {
            switch(*who) {
                case 'u': mask |= S_IRWXU; break;
                case 'g': mask |= S_IRWXG; break;
                case 'o': mask |= S_IRWXO; break;
                case 'a': mask |= S_IRWXU | S_IRWXG | S_IRWXO; break;
                default: printf("Неизвестный флаг: %c\n", *who);
            }
            who++;
        }
        
        // Определяем биты для установки
        mode_t bits = 0;
        while(*rights) {
            switch(*rights) {
                case 'r': bits |= S_IRUSR | S_IRGRP | S_IROTH; break;
                case 'w': bits |= S_IWUSR | S_IWGRP | S_IWOTH; break;
                case 'x': bits |= S_IXUSR | S_IXGRP | S_IXOTH; break;
                case 's': bits |= S_ISUID | S_ISGID; break;
                case 't': bits |= S_ISVTX; break;
                default: printf("Неизвестное право: %c\n", *rights);
            }
            rights++;
        }
        
        // Применяем операцию
        switch(op) {
            case '+':
                perm->binary |= (bits & mask);
                break;
            case '-':
                perm->binary &= ~(bits & mask);
                break;
            case '=':
                perm->binary = (perm->binary & ~mask) | (bits & mask);
                break;
        }
        
        token = strtok(NULL, ",");
    }
    
    free(cmd_copy);
    perm->binary &= 07777;  // Сохраняем только биты прав
    perm->octal = perm->binary;
    update_symbolic(perm);
}

void get_file_permissions(const char *filename, Permissions *perm) {
    struct stat st;
    if (stat(filename, &st)) {
        perror("Ошибка получения информации о файле");
        return;
    }
    
    perm->binary = st.st_mode & 0777;
    perm->octal = perm->binary;
    update_symbolic(perm);
    
    printf("\nСравнение с ls -l:\n");
    char command[BUF_SIZE];
    snprintf(command, BUF_SIZE, "ls -l %s", filename);
    system(command);
}