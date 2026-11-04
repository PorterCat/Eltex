#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

typedef struct ListNode
{
    struct ListNode *prev;
    struct ListNode *next;
    void *data;
} ListNode;

typedef struct
{
    ListNode *head;
    ListNode *tail;
    int count;
    size_t data_size;
} LinkedList;

int list_init(LinkedList *list, size_t data_size);

ListNode *list_insert(LinkedList *list, ListNode *position, const void *data);
int list_move_before(LinkedList *list, ListNode *node, ListNode *position);
int list_remove(LinkedList *list, ListNode *node);

void list_clear(LinkedList *list);
void list_destroy(LinkedList *list);

#endif