#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

typedef struct ListNode ListNode;

typedef struct
{
    ListNode *head;
    ListNode *tail;
    int count;
    size_t data_size;
} LinkedList;

typedef int (*ListCompareFunc)(const void *left, const void *right);

int list_init(LinkedList *list, size_t data_size);

int list_count(const LinkedList *list);
int list_get(const LinkedList *list, int index, void *data);

int list_insert_sorted(LinkedList *list, const void *data, ListCompareFunc compare);
int list_update_sorted(LinkedList *list, int index, const void *data, ListCompareFunc compare);
int list_remove(LinkedList *list, int index);

void list_clear(LinkedList *list);
void list_destroy(LinkedList *list);

#endif