#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LENGTH 50
#define CLEAR_BUFFER while (getchar() != '\n')

typedef struct Contact {
    char firstName[MAX_LENGTH];
    char lastName[MAX_LENGTH];
    char workplace[MAX_LENGTH];
    char job[MAX_LENGTH];
    char phone[MAX_LENGTH];
    char email[MAX_LENGTH];
    char social[MAX_LENGTH];
    char messenger[MAX_LENGTH];
    struct Contact *prev;
    struct Contact *next;
} Contact;

Contact *head = NULL;
Contact *tail = NULL;
int contactCount = 0;

// Прототипы функций
void printMenu();
void printContactHeader();
void printContactDetails(Contact *c, int index);
void getInput(char *buffer, int size);
int validateNumberInput(int min, int max);
void addContact();
void displayContacts();
void editContact();
void deleteContact();
void insertContactSorted(Contact *newContact);
Contact* getContactByIndex(int index);

int main() {
    int choice;
    
    do {
        printMenu();
        choice = validateNumberInput(1, 5);
        
        switch (choice) {
            case 1: addContact(); break;
            case 2: editContact(); break;
            case 3: deleteContact(); break;
            case 4: displayContacts(); break;
            case 5: printf("\nВыход из программы...\n"); break;
        }
    } while (choice != 5);

    return 0;
}

void printMenu() {
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

void printContactHeader() {
    printf("\n\033[1;36m┌─────┬───────────────────────┬───────────────────────┐\n");
    printf("│ %-4s │ %-21s │ %-21s │\n", "№  ", "Имя                  ", "Фамилия              ");
    printf("├─────┼───────────────────────┼───────────────────────┤\033[0m\n");
}

void printContactDetails(Contact *c, int index) {
    printf("\n\033[1;35m=== Контакт #%d ======================================\033[0m\n", index);
    printf("\033[1m%-15s:\033[0m %s %s\n", "Имя", c->firstName, c->lastName);
    if(strlen(c->workplace) > 0)
        printf("\033[1m%-15s:\033[0m %s\n", "Место работы", c->workplace);
    if(strlen(c->job) > 0)
        printf("\033[1m%-15s:\033[0m %s\n", "Должность", c->job);
    if(strlen(c->phone) > 0)
        printf("\033[1m%-15s:\033[0m %s\n", "Телефон", c->phone);
    if(strlen(c->email) > 0)
        printf("\033[1m%-15s:\033[0m %s\n", "Email", c->email);
    if(strlen(c->social) > 0)
        printf("\033[1m%-15s:\033[0m %s\n", "Соцсети", c->social);
    if(strlen(c->messenger) > 0)
        printf("\033[1m%-15s:\033[0m %s\n", "Мессенджер", c->messenger);
    printf("\033[1;35m=======================================================\033[0m\n");
}

void getInput(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int validateNumberInput(int min, int max) {
    int input;
    char buffer[100];

    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Ошибка чтения ввода!\n");
            continue;
        }
        if (sscanf(buffer, "%d", &input) != 1 || input < min || input > max) {
            printf("\033[1;31mОшибка: Введите число от %d до %d: \033[0m", min, max);
            continue;
        }
        return input;
    }
}

void insertContactSorted(Contact *newContact) {
    if (!head || strcmp(newContact->lastName, head->lastName) < 0) {
        newContact->next = head;
        if (head) head->prev = newContact;
        head = newContact;
        if (!tail) tail = newContact;
        contactCount++;
        return;
    }

    Contact *current = head;
    while (current->next && strcmp(current->next->lastName, newContact->lastName) < 0) {
        current = current->next;
    }

    newContact->next = current->next;
    newContact->prev = current;
    if (current->next) current->next->prev = newContact;
    current->next = newContact;
    if (!newContact->next) tail = newContact;
    contactCount++;
}

void addContact() {
    Contact *newContact = malloc(sizeof(Contact));
    if (!newContact) {
        printf("\033[1;31mОшибка выделения памяти!\033[0m\n");
        return;
    }
    memset(newContact, 0, sizeof(Contact));

    int valid;
    do {
        printf("\nВведите имя: ");
        getInput(newContact->firstName, MAX_LENGTH);
        valid = strlen(newContact->firstName) > 0;
        if (!valid) printf("\033[1;31mИмя обязательно для заполнения!\033[0m\n");
    } while (!valid);

    do {
        printf("Введите фамилию: ");
        getInput(newContact->lastName, MAX_LENGTH);
        valid = strlen(newContact->lastName) > 0;
        if (!valid) printf("\033[1;31mФамилия обязательна для заполнения!\033[0m\n");
    } while (!valid);

    printf("Место работы (опционально): ");
    getInput(newContact->workplace, MAX_LENGTH);
    printf("Должность (опционально): ");
    getInput(newContact->job, MAX_LENGTH);
    printf("Телефон (опционально): ");
    getInput(newContact->phone, MAX_LENGTH);
    printf("Email (опционально): ");
    getInput(newContact->email, MAX_LENGTH);
    printf("Соцсети (опционально): ");
    getInput(newContact->social, MAX_LENGTH);
    printf("Мессенджер (опционально): ");
    getInput(newContact->messenger, MAX_LENGTH);

    insertContactSorted(newContact);
    printf("\n\033[1;32mКонтакт успешно добавлен!\033[0m\n");
    sleep(1);
}

Contact* getContactByIndex(int index) {
    Contact *current = head;
    for (int i = 1; current && i < index; i++) {
        current = current->next;
    }
    return current;
}

void displayContacts() {
    system("clear || cls");
    if (!head) {
        printf("\n\033[1;33mСписок контактов пуст.\033[0m\n");
        sleep(1);
        return;
    }

    Contact *current = head;
    int i = 1;
    printf("\n\033[1;34m=== ВСЕ КОНТАКТЫ ===\033[0m\n");
    printContactHeader();
    while (current) {
        char first[22], last[22];
        strncpy(first, current->firstName, 21);
        strncpy(last, current->lastName, 21);
        first[21] = last[21] = '\0';
        printf("│ \033[1;33m%-3d\033[0m │ %-21s │ %-21s │\n", i, first, last);
        current = current->next;
        i++;
    }
    printf("\033[1;36m└─────┴───────────────────────┴───────────────────────┘\033[0m\n");

    printf("\nВведите номер контакта для деталей (0 - назад): ");
    int choice = validateNumberInput(0, contactCount);
    if (choice > 0) {
        Contact *selected = getContactByIndex(choice);
        if (selected) {
            printContactDetails(selected, choice);
            printf("\nНажмите Enter чтобы продолжить...");
            CLEAR_BUFFER; getchar();
        }
    }
}

void editContact() {
    if (!head) {
        printf("\033[1;33m\nСписок контактов пуст.\033[0m\n");
        sleep(1);
        return;
    }

    system("clear || cls");
    printf("\n\033[1;34m=== РЕДАКТИРОВАНИЕ КОНТАКТА ===\033[0m\n");
    displayContacts();
    printf("\nВведите номер контакта для редактирования (0 - отмена): ");
    int index = validateNumberInput(0, contactCount);
    if (index == 0) return;

    Contact *c = getContactByIndex(index);
    if (!c) return;

    char input[MAX_LENGTH];
    int valid;

    do {
        system("clear || cls");
        printContactDetails(c, index);

        printf("\n\033[1;36mРедактирование поля:\033[0m\n");
        printf("1. Имя (%s)\n", c->firstName);
        printf("2. Фамилия (%s)\n", c->lastName);
        printf("3. Место работы (%s)\n", c->workplace);
        printf("4. Должность (%s)\n", c->job);
        printf("5. Телефон (%s)\n", c->phone);
        printf("6. Email (%s)\n", c->email);
        printf("7. Соцсети (%s)\n", c->social);
        printf("8. Мессенджер (%s)\n", c->messenger);
        printf("9. Сохранить и выйти\n0. Отмена\n");
        printf("Выберите поле для редактирования: ");
        int field = validateNumberInput(0, 9);

        switch (field) {
            case 1:
                do {
                    printf("Новое имя: ");
                    getInput(input, MAX_LENGTH);
                    valid = strlen(input) > 0;
                    if (!valid) printf("Имя не может быть пустым!\n");
                } while (!valid);
                strcpy(c->firstName, input);
                break;
            case 2:
                do {
                    printf("Новая фамилия: ");
                    getInput(input, MAX_LENGTH);
                    valid = strlen(input) > 0;
                    if (!valid) printf("Фамилия не может быть пустой!\n");
                } while (!valid);
                strcpy(c->lastName, input);
                break;
            case 3: printf("Новое место работы: "); getInput(c->workplace, MAX_LENGTH); break;
            case 4: printf("Новая должность: "); getInput(c->job, MAX_LENGTH); break;
            case 5: printf("Новый телефон: "); getInput(c->phone, MAX_LENGTH); break;
            case 6: printf("Новый email: "); getInput(c->email, MAX_LENGTH); break;
            case 7: printf("Новые соцсети: "); getInput(c->social, MAX_LENGTH); break;
            case 8: printf("Новый мессенджер: "); getInput(c->messenger, MAX_LENGTH); break;
            case 9: return;
            case 0: return;
        }
    } while (1);
}

void deleteContact() {
    if (!head) {
        printf("\033[1;33m\nСписок контактов пуст.\033[0m\n");
        sleep(1);
        return;
    }

    system("clear || cls");
    printf("\n\033[1;34m=== УДАЛЕНИЕ КОНТАКТА ===\033[0m\n");
    displayContacts();
    printf("\nВведите номер контакта для удаления (0 - отмена): ");
    int index = validateNumberInput(0, contactCount);
    if (index == 0) return;

    Contact *c = getContactByIndex(index);
    if (!c) return;

    if (c->prev) c->prev->next = c->next;
    else head = c->next;

    if (c->next) c->next->prev = c->prev;
    else tail = c->prev;

    free(c);
    contactCount--;
    printf("\n\033[1;32mКонтакт удален.\033[0m\n");
    sleep(1);
}
