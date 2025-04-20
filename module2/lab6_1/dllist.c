#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllist.h"

static Contact *head = NULL;
static Contact *tail = NULL;
static int      contactCount = 0;

void dllist_init(void) {
    head = tail = NULL;
    contactCount = 0;
}

int dllist_count(void) {
    return contactCount;
}

void dllist_add(Contact *newContact) {
    // Узел уже заполнен пользователем, только вставляем его
    if (!head || strcmp(newContact->lastName, head->lastName) < 0) {
        newContact->next = head;
        newContact->prev = NULL;
        if (head) head->prev = newContact;
        head = newContact;
        if (!tail) tail = newContact;
    } else {
        Contact *cur = head;
        while (cur->next && strcmp(cur->next->lastName, newContact->lastName) < 0) {
            cur = cur->next;
        }
        newContact->next = cur->next;
        newContact->prev = cur;
        if (cur->next) cur->next->prev = newContact;
        cur->next = newContact;
        if (!newContact->next) tail = newContact;
    }
    contactCount++;
}

Contact* dllist_get(int index) {
    if (index < 1 || index > contactCount) return NULL;
    Contact *cur = head;
    for (int i = 1; i < index; i++)
        cur = cur->next;
    return cur;
}

int dllist_remove(int index) {
    Contact *c = dllist_get(index);
    if (!c) return 0;
    if (c->prev) c->prev->next = c->next;
    else          head = c->next;
    if (c->next) c->next->prev = c->prev;
    else          tail = c->prev;
    free(c);
    contactCount--;
    return 1;
}

void dllist_free(void) {
    Contact *cur = head;
    while (cur) {
        Contact *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    head = tail = NULL;
    contactCount = 0;
}
