#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "dllist.h"

#define CLEAR_BUFFER while (getchar() != '\n')
#define MAX_LENGTH 50

// Прототипы функций пользовательского интерфейса
void printMenu(void);
void printContactHeader(void);
void printContactDetails(Contact *c, int index);
void getInput(char *buffer, int size);
int  validateNumberInput(int min, int max);

void addContactInteractive(void);
void displayContactsInteractive(void);
void editContactInteractive(void);
void deleteContactInteractive(void);

int main(void) {
    dllist_init();

    int choice;
    do {
        printMenu();
        choice = validateNumberInput(1, 5);
        switch (choice) {
            case 1: addContactInteractive();     break;
            case 2: editContactInteractive();    break;
            case 3: deleteContactInteractive();  break;
            case 4: displayContactsInteractive();break;
            case 5: printf("\nВыход из программы...\n"); break;
        }
    } while (choice != 5);

    dllist_free();
    return 0;
}

void printMenu(void) {
    system("clear || cls");
    printf("\n\033[1;34m=== ТЕЛЕФОННАЯ КНИГА ===\033[0m\n");
    printf("\033[1mУправление:\033[0m\n");
    printf("1. Добавить контакт\n");
    printf("2. Редактировать контакт\n");
    printf("3. Удалить контакт\n");
    printf("4. Показать все контакты\n");
    printf("5. Выход\n");
    printf("\nВыберите действие: ");
}

void printContactHeader(void) {
    printf("\n\033[1;36m┌─────┬───────────────────────┬───────────────────────┐\n");
    printf("│ %-4s │ %-21s │ %-21s │\n",
           "№", "Имя", "Фамилия");
    printf("├─────┼───────────────────────┼───────────────────────┤\033[0m\n");
}

void printContactDetails(Contact *c, int index) {
    printf("\n\033[1;35m=== Контакт #%d ======================================\033[0m\n", index);
    printf("\033[1m%-15s:\033[0m %s %s\n", "Имя", c->firstName, c->lastName);
    if (c->workplace[0])
        printf("\033[1m%-15s:\033[0m %s\n", "Место работы", c->workplace);
    if (c->job[0])
        printf("\033[1m%-15s:\033[0m %s\n", "Должность", c->job);
    if (c->phone[0])
        printf("\033[1m%-15s:\033[0m %s\n", "Телефон", c->phone);
    if (c->email[0])
        printf("\033[1m%-15s:\033[0m %s\n", "Email", c->email);
    if (c->social[0])
        printf("\033[1m%-15s:\033[0m %s\n", "Соцсети", c->social);
    if (c->messenger[0])
        printf("\033[1m%-15s:\033[0m %s\n", "Мессенджер", c->messenger);
    printf("\033[1;35m=======================================================\033[0m\n");
}

void getInput(char *buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

int validateNumberInput(int min, int max) {
    char buf[100];
    int x;
    while (1) {
        if (!fgets(buf, sizeof buf, stdin)) {
            printf("Ошибка ввода, попробуйте снова: ");
            continue;
        }
        if (sscanf(buf, "%d", &x) != 1 || x < min || x > max) {
            printf("\033[1;31mОшибка: введите число от %d до %d: \033[0m", min, max);
        } else {
            return x;
        }
    }
}

void addContactInteractive(void) {
    Contact *c = malloc(sizeof *c);
    if (!c) {
        printf("\033[1;31mОшибка выделения памяти!\033[0m\n");
        sleep(1);
        return;
    }
    memset(c, 0, sizeof *c);

    // Обязательные поля
    do {
        printf("\nВведите имя: ");
        getInput(c->firstName, MAX_LENGTH);
        if (!c->firstName[0]) printf("\033[1;31mИмя обязательно!\033[0m\n");
    } while (!c->firstName[0]);

    do {
        printf("Введите фамилию: ");
        getInput(c->lastName, MAX_LENGTH);
        if (!c->lastName[0]) printf("\033[1;31mФамилия обязательна!\033[0m\n");
    } while (!c->lastName[0]);

    // Опциональные поля
    printf("Место работы (опционально): ");
    getInput(c->workplace, MAX_LENGTH);
    printf("Должность (опционально): ");
    getInput(c->job, MAX_LENGTH);
    printf("Телефон (опционально): ");
    getInput(c->phone, MAX_LENGTH);
    printf("Email (опционально): ");
    getInput(c->email, MAX_LENGTH);
    printf("Соцсети (опционально): ");
    getInput(c->social, MAX_LENGTH);
    printf("Мессенджер (опционально): ");
    getInput(c->messenger, MAX_LENGTH);

    dllist_add(c);
    printf("\n\033[1;32mКонтакт успешно добавлен!\033[0m\n");
    sleep(1);
}

void displayContactsInteractive(void) {
    system("clear || cls");
    int count = dllist_count();
    if (count == 0) {
        printf("\n\033[1;33mСписок контактов пуст.\033[0m\n");
        sleep(1);
        return;
    }

    printf("\n\033[1;34m=== ВСЕ КОНТАКТЫ ===\033[0m\n");
    printContactHeader();
    for (int i = 1; i <= count; i++) {
        Contact *c = dllist_get(i);
        char fn[22] = {0}, ln[22] = {0};
        strncpy(fn, c->firstName, 21);
        strncpy(ln, c->lastName, 21);
        printf("│ \033[1;33m%-3d\033[0m │ %-21s │ %-21s │\n",
               i, fn, ln);
    }
    printf("\033[1;36m└─────┴───────────────────────┴───────────────────────┘\033[0m\n");

    printf("\nВведите № контакта для подробностей (0 – назад): ");
    int choice = validateNumberInput(0, count);
    if (choice > 0) {
        Contact *sel = dllist_get(choice);
        printContactDetails(sel, choice);
        printf("\nНажмите Enter для продолжения...");
        CLEAR_BUFFER; getchar();
    }
}

void editContactInteractive(void) {
    int count = dllist_count();
    if (count == 0) {
        printf("\n\033[1;33mСписок контактов пуст.\033[0m\n");
        sleep(1);
        return;
    }

    displayContactsInteractive();
    printf("\nВведите № контакта для редактирования (0 – отмена): ");
    int idx = validateNumberInput(0, count);
    if (idx == 0) return;

    Contact *c = dllist_get(idx);
    if (!c) return;

    char buf[MAX_LENGTH];
    int field;
    do {
        system("clear || cls");
        printContactDetails(c, idx);
        printf("\n\033[1;36mВыберите поле для редактирования:\033[0m\n");
        printf("1. Имя (%s)\n", c->firstName);
        printf("2. Фамилия (%s)\n", c->lastName);
        printf("3. Место работы (%s)\n", c->workplace);
        printf("4. Должность (%s)\n", c->job);
        printf("5. Телефон (%s)\n", c->phone);
        printf("6. Email (%s)\n", c->email);
        printf("7. Соцсети (%s)\n", c->social);
        printf("8. Мессенджер (%s)\n", c->messenger);
        printf("9. Сохранить и выйти\n");
        printf("0. Отмена\n");
        printf("Ваш выбор: ");
        field = validateNumberInput(0, 9);

        switch (field) {
            case 1:
                do {
                    printf("Новое имя: ");
                    getInput(buf, MAX_LENGTH);
                    if (!buf[0]) printf("Имя не может быть пустым!\n");
                } while (!buf[0]);
                strcpy(c->firstName, buf);
                break;
            case 2:
                do {
                    printf("Новая фамилия: ");
                    getInput(buf, MAX_LENGTH);
                    if (!buf[0]) printf("Фамилия не может быть пустой!\n");
                } while (!buf[0]);
                strcpy(c->lastName, buf);
                break;
            case 3: printf("Новое место работы: "); getInput(c->workplace, MAX_LENGTH); break;
            case 4: printf("Новая должность:    "); getInput(c->job, MAX_LENGTH); break;
            case 5: printf("Новый телефон:     "); getInput(c->phone, MAX_LENGTH); break;
            case 6: printf("Новый email:       "); getInput(c->email, MAX_LENGTH); break;
            case 7: printf("Новые соцсети:      "); getInput(c->social, MAX_LENGTH); break;
            case 8: printf("Новый мессенджер:   "); getInput(c->messenger, MAX_LENGTH); break;
        }
    } while (field > 0 && field < 9);
}

void deleteContactInteractive(void) {
    int count = dllist_count();
    if (count == 0) {
        printf("\n\033[1;33mСписок контактов пуст.\033[0m\n");
        sleep(1);
        return;
    }

    displayContactsInteractive();
    printf("\nВведите № контакта для удаления (0 – отмена): ");
    int idx = validateNumberInput(0, count);
    if (idx == 0) return;

    if (dllist_remove(idx)) {
        printf("\n\033[1;32mКонтакт удалён.\033[0m\n");
    } else {
        printf("\n\033[1;31mОшибка удаления.\033[0m\n");
    }
    sleep(1);
}
