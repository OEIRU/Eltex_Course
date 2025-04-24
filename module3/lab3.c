#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_CONTACTS 100
#define MAX_LENGTH 50
#define CLEAR_BUFFER while (getchar() != '\n')

typedef struct {
    char firstName[MAX_LENGTH];
    char lastName[MAX_LENGTH];
    char workplace[MAX_LENGTH];
    char job[MAX_LENGTH];
    char phone[MAX_LENGTH];
    char email[MAX_LENGTH];
    char social[MAX_LENGTH];
    char messenger[MAX_LENGTH];
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;
const char *filename = "contacts.txt";

// Прототипы функций
void printMenu();
void printContactHeader();
void printContactDetails(int index);
void getInput(char *buffer, int size);
int validateNumberInput(int min, int max);
void addContact();
void displayContacts();
void editContact();
void deleteContact();
void loadContacts();
void saveContacts();

int main() {
    loadContacts(); // Загрузка контактов из файла

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

    saveContacts(); // Сохранение контактов в файл
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

void printContactDetails(int index) {
    Contact c = contacts[index];
    printf("\n\033[1;35m=== Контакт #%d ======================================\033[0m\n", index + 1);
    printf("\033[1m%-15s:\033[0m %s %s\n", "Имя", c.firstName, c.lastName);
    
    if (strlen(c.workplace) > 0) 
        printf("\033[1m%-15s:\033[0m %s\n", "Место работы", c.workplace);
    if (strlen(c.job) > 0) 
        printf("\033[1m%-15s:\033[0m %s\n", "Должность", c.job);
    if (strlen(c.phone) > 0) 
        printf("\033[1m%-15s:\033[0m %s\n", "Телефон", c.phone);
    if (strlen(c.email) > 0) 
        printf("\033[1m%-15s:\033[0m %s\n", "Email", c.email);
    if (strlen(c.social) > 0) 
        printf("\033[1m%-15s:\033[0m %s\n", "Соцсети", c.social);
    if (strlen(c.messenger) > 0) 
        printf("\033[1m%-15s:\033[0m %s\n", "Мессенджер", c.messenger);
    
    printf("\033[1;35m=======================================================\033[0m\n");
}

void getInput(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Удаление символа новой строки
}

int validateNumberInput(int min, int max) {
    int input;
    char buffer[100];
    
    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Ошибка чтения ввода!\n");
            continue;
        }
        
        if (sscanf(buffer, "%d", &input) != 1) {
            printf("\033[1;31mОшибка: Введите число от %d до %d: \033[0m", min, max);
            continue;
        }
        
        if (input < min || input > max) {
            printf("\033[1;31mОшибка: Число должно быть от %d до %d: \033[0m", min, max);
            continue;
        }
        
        return input;
    }
}

void addContact() {
    if (contactCount >= MAX_CONTACTS) {
        printf("\033[1;31m\nДостигнуто максимальное количество контактов (%d)!\033[0m\n", MAX_CONTACTS);
        return;
    }

    Contact newContact = {0};
    int valid;

    do {
        printf("\nВведите имя: ");
        getInput(newContact.firstName, MAX_LENGTH);
        valid = strlen(newContact.firstName) > 0;
        if (!valid) printf("\033[1;31mИмя обязательно для заполнения!\033[0m\n");
    } while (!valid);

    do {
        printf("Введите фамилию: ");
        getInput(newContact.lastName, MAX_LENGTH);
        valid = strlen(newContact.lastName) > 0;
        if (!valid) printf("\033[1;31mФамилия обязательна для заполнения!\033[0m\n");
    } while (!valid);

    printf("Место работы (опционально): ");
    getInput(newContact.workplace, MAX_LENGTH);

    printf("Должность (опционально): ");
    getInput(newContact.job, MAX_LENGTH);

    printf("Телефон (опционально): ");
    getInput(newContact.phone, MAX_LENGTH);

    printf("Email (опционально): ");
    getInput(newContact.email, MAX_LENGTH);

    printf("Соцсети (опционально): ");
    getInput(newContact.social, MAX_LENGTH);

    printf("Мессенджер (опционально): ");
    getInput(newContact.messenger, MAX_LENGTH);

    contacts[contactCount++] = newContact;
    printf("\n\033[1;32mКонтакт успешно добавлен!\033[0m\n");
    sleep(1);
}

void displayContacts() {
    system("clear || cls");
    if (contactCount == 0) {
        printf("\n\033[1;33mСписок контактов пуст.\033[0m\n");
        sleep(1);
        return;
    }

    printf("\n\033[1;34m=== ВСЕ КОНТАКТЫ (%d) ===\033[0m\n", contactCount);
    printContactHeader();
    
    for (int i = 0; i < contactCount; i++) {
        // Обрезаем длинные строки
        char first[22], last[22];
        strncpy(first, contacts[i].firstName, 21);
        strncpy(last, contacts[i].lastName, 21);
        first[21] = last[21] = '\0';
        
        printf("│ \033[1;33m%-3d\033[0m │ %-21s │ %-21s │\n", 
               i + 1, first, last);
    }
    printf("\033[1;36m└─────┴───────────────────────┴───────────────────────┘\033[0m\n");
    
    printf("\nВведите номер контакта для деталей (0 - назад): ");
    int choice = validateNumberInput(0, contactCount);
    
    if (choice > 0) {
        printContactDetails(choice - 1);
        printf("\nНажмите Enter чтобы продолжить...");
        CLEAR_BUFFER;
        getchar();
    }
}

void editContact() {
    if (contactCount == 0) {
        printf("\033[1;33m\nСписок контактов пуст.\033[0m\n");
        sleep(1);
        return;
    }

    system("clear || cls");
    printf("\n\033[1;34m=== РЕДАКТИРОВАНИЕ КОНТАКТА ===\033[0m\n");
    displayContacts();
    
    printf("\nВведите номер контакта для редактирования (0 - отмена): ");
    int index = validateNumberInput(0, contactCount);
    
    if (index == 0) {
        printf("\033[1;33mРедактирование отменено\033[0m\n");
        sleep(1);
        return;
    }

    Contact *c = &contacts[index - 1];
    int valid;
    char input[MAX_LENGTH];

    do {
        system("clear || cls");
        printContactDetails(index - 1);
        
        printf("\n\033[1;36mРедактирование поля:\033[0m\n");
        printf("1. Имя (%s)\n", c->firstName);
        printf("2. Фамилия (%s)\n", c->lastName);
        printf("3. Место работы (%s)\n", c->workplace);
        printf("4. Должность (%s)\n", c->job);
        printf("5. Телефон (%s)\n", c->phone);
        printf("6. Email (%s)\n", c->email);
        printf("7. Соцсети (%s)\n", c->social);
        printf("8. Мессенджер (%s)\n", c->messenger);
        printf("9. \033[1;32mСохранить изменения\033[0m\n");
        printf("0. \033[1;31mОтменить изменения\033[0m\n");
        printf("\nВыберите поле для редактирования: ");
        
        int field = validateNumberInput(0, 9);
        
        switch(field) {
            case 1:
                do {
                    printf("Текущее имя: %s\nНовое имя: ", c->firstName);
                    getInput(input, MAX_LENGTH);
                    valid = strlen(input) > 0;
                    if (!valid) printf("\033[1;31mИмя не может быть пустым!\033[0m\n");
                } while (!valid);
                strcpy(c->firstName, input);
                break;
                
            case 2:
                do {
                    printf("Текущая фамилия: %s\nНовая фамилия: ", c->lastName);
                    getInput(input, MAX_LENGTH);
                    valid = strlen(input) > 0;
                    if (!valid) printf("\033[1;31mФамилия не может быть пустой!\033[0m\n");
                } while (!valid);
                strcpy(c->lastName, input);
                break;
                
            case 3:
                printf("Текущее место работы: %s\nНовое место работы: ", c->workplace);
                getInput(input, MAX_LENGTH);
                strcpy(c->workplace, input);
                break;
                
            case 4:
                printf("Текущая должность: %s\nНовая должность: ", c->job);
                getInput(input, MAX_LENGTH);
                strcpy(c->job, input);
                break;
                
            case 5:
                printf("Текущий телефон: %s\nНовый телефон: ", c->phone);
                getInput(input, MAX_LENGTH);
                strcpy(c->phone, input);
                break;
                
            case 6:
                printf("Текущий email: %s\nНовый email: ", c->email);
                getInput(input, MAX_LENGTH);
                strcpy(c->email, input);
                break;
                
            case 7:
                printf("Текущие соцсети: %s\nНовые соцсети: ", c->social);
                getInput(input, MAX_LENGTH);
                strcpy(c->social, input);
                break;
                
            case 8:
                printf("Текущий мессенджер: %s\nНовый мессенджер: ", c->messenger);
                getInput(input, MAX_LENGTH);
                strcpy(c->messenger, input);
                break;
                
            case 9:
                printf("\033[1;32mИзменения сохранены!\033[0m\n");
                sleep(1);
                return;
                
            case 0:
                printf("\033[1;33mИзменения отменены\033[0m\n");
                sleep(1);
                return;
        }
    } while(1);
}

void deleteContact() {
    if (contactCount == 0) {
        printf("\033[1;33m\nСписок контактов пуст.\033[0m\n");
        sleep(1);
        return;
    }

    system("clear || cls");
    printf("\n\033[1;34m=== УДАЛЕНИЕ КОНТАКТА ===\033[0m\n");
    displayContacts();
    
    printf("\nВведите номер контакта для удаления (0 - отмена): ");
    int index = validateNumberInput(0, contactCount);
    
    if (index == 0) {
        printf("\033[1;33mУдаление отменено\033[0m\n");
        sleep(1);
        return;
    }

    // Подтверждение удаления
    printf("\n\033[1;31mВы уверены что хотите удалить контакт #%d?\033[0m\n", index);
    printf("1. Да, удалить\n");
    printf("2. Нет, отменить\n");
    printf("Выберите действие: ");
    
    int confirm = validateNumberInput(1, 2);
    
    if (confirm == 2) {
        printf("\033[1;33mУдаление отменено\033[0m\n");
        sleep(1);
        return;
    }

    // Сдвиг контактов
    for (int i = index - 1; i < contactCount - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contactCount--;

    printf("\n\033[1;32mКонтакт успешно удален!\033[0m\n");
    sleep(1);
}

void loadContacts() {
    printf("\n[DEBUG] Загружаем контакты из файла: %s\n", filename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("[DEBUG] Файл не найден или ошибка при открытии.\n");
        return;
    }
    
    while (fscanf(file, "%49s %49s %49s %49s %49s %49s %49s %49s\n", 
                  contacts[contactCount].firstName,
                  contacts[contactCount].lastName,
                  contacts[contactCount].workplace,
                  contacts[contactCount].job,
                  contacts[contactCount].phone,
                  contacts[contactCount].email,
                  contacts[contactCount].social,
                  contacts[contactCount].messenger) == 8) {
        printf("[DEBUG] Загружен контакт: %s %s\n", contacts[contactCount].firstName, contacts[contactCount].lastName);
        contactCount++;
        if (contactCount >= MAX_CONTACTS) {
            break; // Достигнуто максимальное количество контактов
        }
    }
    fclose(file);
    printf("[DEBUG] Загрузка завершена. Всего контактов: %d\n", contactCount);
}

void saveContacts() {
    printf("\n[DEBUG] Сохраняем контакты в файл: %s\n", filename);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("\033[1;31mОшибка при открытии файла для записи!\033[0m\n");
        return;
    }

    for (int i = 0; i < contactCount; i++) {
        fprintf(file, "%s %s %s %s %s %s %s %s\n", 
                contacts[i].firstName,
                contacts[i].lastName,
                contacts[i].workplace,
                contacts[i].job,
                contacts[i].phone,
                contacts[i].email,
                contacts[i].social,
                contacts[i].messenger);
        printf("[DEBUG] Сохранен контакт: %s %s\n", contacts[i].firstName, contacts[i].lastName);
    }
    fclose(file);
    printf("[DEBUG] Сохранение завершено.\n");
}