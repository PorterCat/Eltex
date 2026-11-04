
#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

struct ListNode
{
    struct ListNode *prev;
    struct ListNode *next;
    void *data;
};

ListNode *find_node(const LinkedList *list, int index)
{
    if (list == NULL || index < 0 || index >= list->count)
        return NULL;

    ListNode *node;

    if (index < list->count / 2)
    {
        node = list->head;

        for (int i = 0; i < index; i++)
            node = node->next;
    }
    else
    {
        node = list->tail;

        for (int i = list->count - 1; i > index; i--)
            node = node->prev;
    }

    return node;
}

ListNode *find_position(LinkedList *list, const void *data, ListCompareFunc compare)
{
    ListNode *node = list->head;

    while (node != NULL && compare(data, node->data) >= 0)
        node = node->next;

    return node;
}

int insert_before_node(LinkedList *list, ListNode *position, const void *data)
{
    ListNode *node = malloc(sizeof(*node));

    if (node == NULL)
        return -1;

    node->data = malloc(list->data_size);

    if (node->data == NULL)
    {
        free(node);
        return -1;
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

    list->count++;

    return 0;
}

void remove_node(LinkedList *list, ListNode *node)
{
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

    list->count--;
}

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

int list_count(const LinkedList *list)
{
    return list != NULL ? list->count : -1;
}

int list_get(const LinkedList *list, int index, void *data)
{
    if (list == NULL || data == NULL)
        return -1;

    const ListNode *node = find_node(list, index);

    if (node == NULL)
        return -1;

    memcpy(data, node->data, list->data_size);

    return 0;
}

int list_insert_sorted(LinkedList *list, const void *data, ListCompareFunc compare)
{
    if (list == NULL || data == NULL || compare == NULL || list->data_size == 0)
        return -1;

    ListNode *position = find_position(list, data, compare);

    return insert_before_node(list, position, data);
}

int list_update_sorted(LinkedList *list, int index, const void *data, ListCompareFunc compare)
{
    if (list == NULL || data == NULL || compare == NULL || list->data_size == 0)
        return -1;

    ListNode *old_node = (ListNode *)find_node(list, index);

    if (old_node == NULL)
        return -1;

    ListNode *position = find_position(list, data, compare);

    if (insert_before_node(list, position, data) != 0)
        return -1;

    remove_node(list, old_node);

    return 0;
}

int list_remove(LinkedList *list, int index)
{
    if (list == NULL)
        return -1;

    ListNode *node = (ListNode *)find_node(list, index);

    if (node == NULL)
        return -1;

    remove_node(list, node);

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