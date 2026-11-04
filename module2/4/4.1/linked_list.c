#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

int list_init(LinkedList *list, size_t data_size)
{
    if (list == NULL || data_size == 0)
        return -1;

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    list->data_size = data_size;

    return 0;
}

ListNode *list_insert(LinkedList *list, ListNode *position, const void *data)
{
    if (list == NULL || data == NULL || list->data_size == 0)
        return NULL;

    ListNode *node = malloc(sizeof(*node));

    if (node == NULL)
        return NULL;

    node->data = malloc(list->data_size);

    if (node->data == NULL)
    {
        free(node);
        return NULL;
    }

    memcpy(node->data, data, list->data_size);

    ListNode *previous = position != NULL ? position->prev : list->tail;

    node->prev = previous;
    node->next = position;

    if (previous != NULL)
        previous->next = node;
    else
        list->head = node;

    if (position != NULL)
        position->prev = node;
    else
        list->tail = node;

    ++list->count;

    return node;
}

int list_move_before(LinkedList *list, ListNode *node, ListNode *position)
{
    if (list == NULL || node == NULL)
        return -1;

    if (node == position || node->next == position)
        return 0;

    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    ListNode *previous = position != NULL ? position->prev : list->tail;

    node->prev = previous;
    node->next = position;

    if (previous != NULL)
        previous->next = node;
    else
        list->head = node;

    if (position != NULL)
        position->prev = node;
    else
        list->tail = node;

    return 0;
}

int list_remove(LinkedList *list, ListNode *node)
{
    if (list == NULL || node == NULL)
        return -1;

    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    free(node->data);
    free(node);

    --list->count;

    return 0;
}

void list_clear(LinkedList *list)
{
    if (list == NULL)
        return;

    ListNode *node = list->head;

    while (node != NULL)
    {
        ListNode *next = node->next;

        free(node->data);
        free(node);

        node = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

void list_destroy(LinkedList *list)
{
    if (list == NULL)
        return;

    list_clear(list);
    list->data_size = 0;
}