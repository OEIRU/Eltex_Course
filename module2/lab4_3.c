#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> 
#define MAX_LENGTH 50
#define BALANCE_THRESHOLD 10
#define MAX_CONTACTS 100
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

// Узел дерева
typedef struct TreeNode {
    Contact contact;
    struct TreeNode *left, *right;
} TreeNode;

// Корень дерева с метриками
typedef struct {
    TreeNode *root;
    int count;
    int insertionsSinceLastBalance;
} TreeRoot;

TreeRoot contactTree = { NULL, 0, 0 };

// Прототипы
void printMenu();
void printContactHeader();
void printContactDetailsNode(const Contact *c);
void getInput(char *buffer, int size);
int validateNumberInput(int min, int max);
void addContact();
void displayContacts();
void editContact();
void deleteContact();

// --- BST: вставка, хранение, балансировка ---
TreeNode* insertNode(TreeNode* node, const Contact *c) {
    if (!node) {
        TreeNode *newNode = malloc(sizeof(TreeNode));
        newNode->contact = *c;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    int cmp = strcmp(c->lastName, node->contact.lastName);
    if (cmp == 0)
        cmp = strcmp(c->firstName, node->contact.firstName);

    if (cmp < 0)
        node->left  = insertNode(node->left,  c);
    else
        node->right = insertNode(node->right, c);

    return node;
}

void storeInOrder(TreeNode *node, Contact *arr, int *idx) {
    if (!node) return;
    storeInOrder(node->left,  arr, idx);
    arr[(*idx)++] = node->contact;
    storeInOrder(node->right, arr, idx);
}

TreeNode* buildBalancedTree(Contact *arr, int start, int end) {
    if (start > end) return NULL;
    int mid = (start + end) / 2;
    TreeNode *node = malloc(sizeof(TreeNode));
    node->contact = arr[mid];
    node->left  = buildBalancedTree(arr, start,   mid-1);
    node->right = buildBalancedTree(arr, mid+1, end);
    return node;
}

void freeTree(TreeNode *node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

void balanceTree() {
    if (contactTree.count == 0) return;
    Contact *arr = malloc(sizeof(Contact) * contactTree.count);
    int n = 0;
    storeInOrder(contactTree.root, arr, &n);
    freeTree(contactTree.root);
    contactTree.root = buildBalancedTree(arr, 0, n-1);
    free(arr);
}

// Обёртка добавления с учётом порога балансировки
void addContactToTree(const Contact *c) {
    if (contactTree.count >= MAX_CONTACTS) {
        printf("\n\033[1;31mДостигнуто максимальное число (%d).\033[0m\n", MAX_CONTACTS);
        sleep(1);
        return;
    }
    contactTree.root = insertNode(contactTree.root, c);
    contactTree.count++;
    if (++contactTree.insertionsSinceLastBalance >= BALANCE_THRESHOLD) {
        balanceTree();
        contactTree.insertionsSinceLastBalance = 0;
    }
}

// --- Утилиты для индексации при редактировании/удалении/просмотре ---
void collectNodes(TreeNode *node, TreeNode **arr, int *idx) {
    if (!node) return;
    collectNodes(node->left,  arr, idx);
    arr[(*idx)++] = node;
    collectNodes(node->right, arr, idx);
}

// --- Меню и основные функции ---
int main() {
    int choice;
    do {
        printMenu();
        choice = validateNumberInput(1, 5);
        switch (choice) {
            case 1: addContact();    break;
            case 2: editContact();   break;
            case 3: deleteContact(); break;
            case 4: displayContacts(); break;
            case 5: printf("\nВыход...\n"); break;
        }
    } while (choice != 5);
    // Перед выходом можно почистить дерево:
    freeTree(contactTree.root);
    return 0;
}

void printMenu() {
    system("clear || cls");
    printf("\n\033[1;34m=== ТЕЛЕФОННАЯ КНИГА ===\033[0m\n");
    printf("1. Добавить контакт\n");
    printf("2. Редактировать контакт\n");
    printf("3. Удалить контакт\n");
    printf("4. Показать все контакты\n");
    printf("5. Выход\n");
    printf("\nВыберите действие: ");
}

void printContactHeader() {
    printf("\n\033[1;36m┌─────┬───────────────────────┬───────────────────────┐\n");
    printf("│ %-4s │ %-21s │ %-21s │\n", "№", "Имя", "Фамилия");
    printf("├─────┼───────────────────────┼───────────────────────┤\033[0m\n");
}

void printContactDetailsNode(const Contact *c) {
    printf("\033[1;35m============================================\033[0m\n");
    printf("\033[1mИмя           :\033[0m %s %s\n", c->firstName, c->lastName);
    if (c->workplace[0]) printf("\033[1mМесто работы  :\033[0m %s\n", c->workplace);
    if (c->job[0])       printf("\033[1mДолжность     :\033[0m %s\n", c->job);
    if (c->phone[0])     printf("\033[1mТелефон       :\033[0m %s\n", c->phone);
    if (c->email[0])     printf("\033[1mEmail         :\033[0m %s\n", c->email);
    if (c->social[0])    printf("\033[1mСоцсети       :\033[0m %s\n", c->social);
    if (c->messenger[0]) printf("\033[1mМессенджер    :\033[0m %s\n", c->messenger);
    printf("\033[1;35m============================================\033[0m\n");
}

void getInput(char *buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

int validateNumberInput(int min, int max) {
    int num; char buf[64];
    while (1) {
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        if (sscanf(buf, "%d", &num)!=1 || num<min || num>max) {
            printf("\033[1;31mОшибка: введите число от %d до %d: \033[0m", min, max);
        } else return num;
    }
}

void addContact() {
    Contact c = {0};
    do {
        printf("\nВведите имя: ");
        getInput(c.firstName, MAX_LENGTH);
        if (!c.firstName[0]) printf("\033[1;31mИмя обязательно!\033[0m\n");
    } while (!c.firstName[0]);
    do {
        printf("Введите фамилию: ");
        getInput(c.lastName, MAX_LENGTH);
        if (!c.lastName[0]) printf("\033[1;31mФамилия обязательна!\033[0m\n");
    } while (!c.lastName[0]);
    printf("Место работы (опц.): "); getInput(c.workplace, MAX_LENGTH);
    printf("Должность (опц.): ");     getInput(c.job, MAX_LENGTH);
    printf("Телефон (опц.): ");      getInput(c.phone, MAX_LENGTH);
    printf("Email (опц.): ");        getInput(c.email, MAX_LENGTH);
    printf("Соцсети (опц.): ");      getInput(c.social, MAX_LENGTH);
    printf("Мессенджер (опц.): ");   getInput(c.messenger, MAX_LENGTH);

    addContactToTree(&c);
    printf("\n\033[1;32mКонтакт добавлен!\033[0m\n");
    sleep(1);
}

void displayContacts() {
    system("clear || cls");
    if (contactTree.count==0) {
        printf("\n\033[1;33mСписок пуст.\033[0m\n");
        sleep(1); return;
    }
    // Собираем указатели в массив
    TreeNode **arr = malloc(sizeof(TreeNode*) * contactTree.count);
    int n = 0;
    collectNodes(contactTree.root, arr, &n);

    printf("\n\033[1;34m=== ВСЕ КОНТАКТЫ (%d) ===\033[0m\n", contactTree.count);
    printContactHeader();
    for (int i = 0; i < n; i++) {
        char first[22], last[22];
        strncpy(first, arr[i]->contact.firstName, 21);
        strncpy(last,  arr[i]->contact.lastName,  21);
        first[21]=last[21]=0;
        printf("│ \033[1;33m%-3d\033[0m │ %-21s │ %-21s │\n",
               i+1, first, last);
    }
    printf("\033[1;36m└─────┴───────────────────────┴───────────────────────┘\033[0m\n");

    printf("\nВведите № для подробностей (0 - назад): ");
    int choice = validateNumberInput(0, contactTree.count);
    if (choice>0) {
        printContactDetailsNode(&arr[choice-1]->contact);
        printf("\nНажмите Enter...");
        CLEAR_BUFFER; getchar();
    }
    free(arr);
}

void editContact() {
    system("clear || cls");
    if (contactTree.count==0) {
        printf("\n\033[1;33mНет контактов для редактирования.\033[0m\n");
        sleep(1); return;
    }
    // собираем
    TreeNode **arr = malloc(sizeof(TreeNode*) * contactTree.count);
    int n=0; collectNodes(contactTree.root, arr, &n);
    printf("\n\033[1;34m=== РЕДАКТИРОВАНИЕ ===\033[0m\n");
    for (int i = 0; i < n; i++) {
        printf("%2d. %s %s\n",
               i+1,
               arr[i]->contact.firstName,
               arr[i]->contact.lastName);
    }
    printf("0. Отмена\nВыберите №: ");
    int idx = validateNumberInput(0, n);
    if (idx==0) { free(arr); return; }

    // Удаляем старый
    Contact old = arr[idx-1]->contact;
    // удалить узел
    int success=0;
    // функция удаления реализована ниже
    extern TreeNode* deleteNode(TreeNode*, const Contact*, int*);
    contactTree.root = deleteNode(contactTree.root, &old, &success);
    if (success) contactTree.count--;

    // Ввод нового
    printf("\n\033[1;34mВведите новые данные:\033[0m\n");
    Contact c = {0};
    do {
        printf("Имя: "); getInput(c.firstName, MAX_LENGTH);
    } while (!c.firstName[0]);
    do {
        printf("Фамилия: "); getInput(c.lastName, MAX_LENGTH);
    } while (!c.lastName[0]);
    printf("Место работы: "); getInput(c.workplace, MAX_LENGTH);
    printf("Должность: ");    getInput(c.job, MAX_LENGTH);
    printf("Телефон: ");      getInput(c.phone, MAX_LENGTH);
    printf("Email: ");        getInput(c.email, MAX_LENGTH);
    printf("Соцсети: ");      getInput(c.social, MAX_LENGTH);
    printf("Мессенджер: ");   getInput(c.messenger, MAX_LENGTH);

    addContactToTree(&c);
    printf("\n\033[1;32mКонтакт обновлён!\033[0m\n");
    sleep(1);
    free(arr);
}

void deleteContact() {
    system("clear || cls");
    if (contactTree.count==0) {
        printf("\n\033[1;33mНет контактов для удаления.\033[0m\n");
        sleep(1); return;
    }
    // собираем
    TreeNode **arr = malloc(sizeof(TreeNode*) * contactTree.count);
    int n=0; collectNodes(contactTree.root, arr, &n);
    printf("\n\033[1;34m=== УДАЛЕНИЕ ===\033[0m\n");
    for (int i = 0; i < n; i++) {
        printf("%2d. %s %s\n",
               i+1,
               arr[i]->contact.firstName,
               arr[i]->contact.lastName);
    }
    printf("0. Отмена\nВыберите №: ");
    int idx = validateNumberInput(0, n);
    if (idx==0) { free(arr); return; }

    printf("Подтвердить удаление %s %s?\n1. Да  2. Нет\nВыбор: ",
           arr[idx-1]->contact.firstName,
           arr[idx-1]->contact.lastName);
    if (validateNumberInput(1,2)==1) {
        Contact c = arr[idx-1]->contact;
        int success=0;
        extern TreeNode* deleteNode(TreeNode*, const Contact*, int*);
        contactTree.root = deleteNode(contactTree.root, &c, &success);
        if (success) {
            contactTree.count--;
            printf("\n\033[1;32mУдалено.\033[0m\n");
        }
    }
    sleep(1);
    free(arr);
}

// --- Реализация удаления узла по ключу (lastName+firstName) ---
TreeNode* findMin(TreeNode* node) {
    while (node->left) node = node->left;
    return node;
}

TreeNode* deleteNode(TreeNode* root, const Contact *c, int *success) {
    if (!root) return NULL;
    int cmp = strcmp(c->lastName, root->contact.lastName);
    if (cmp==0) cmp = strcmp(c->firstName, root->contact.firstName);

    if (cmp < 0)
        root->left  = deleteNode(root->left,  c, success);
    else if (cmp > 0)
        root->right = deleteNode(root->right, c, success);
    else {
        // нашли
        *success = 1;
        if (!root->left || !root->right) {
            TreeNode *tmp = root->left ? root->left : root->right;
            free(root);
            return tmp;
        }
        // два потомка: берём следующий по порядку
        TreeNode *succ = findMin(root->right);
        root->contact = succ->contact;
        root->right = deleteNode(root->right, &succ->contact, success);
    }
    return root;
}
