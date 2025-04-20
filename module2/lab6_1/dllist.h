#ifndef DLLIST_H
#define DLLIST_H

#define MAX_LENGTH 50

// Описание структуры контакта
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

// Инициализация списка
void dllist_init(void);

// Добавление контакта (упорядоченно по фамилии)
void dllist_add(Contact *newContact);

// Получение количества элементов
int  dllist_count(void);

// Поиск контакта по индексу (1‑based)
Contact* dllist_get(int index);

// Удаление контакта по индексу (1‑based)
int  dllist_remove(int index);

// Освобождение всей памяти списка
void dllist_free(void);

#endif // DLLIST_H
