#include "phonebook_wrapper.h"

#include <string.h>

typedef int (*CompareFunc)(const void *left, const void *right);

int compare_contacts(const void *left, const void *right)
{
    const Contact *a = left;
    const Contact *b = right;

    int result = strcmp(a->last_name, b->last_name);

    if (result != 0)
        return result;

    return strcmp(a->first_name, b->first_name);
}

ListNode *find_position(const LinkedList *list, const Contact *contact, const ListNode *skip,
                        CompareFunc compare)
{
    ListNode *node = list->head;

    while (node != NULL)
    {
        if (node != skip && compare(contact, node->data) < 0)
            return node;

        node = node->next;
    }

    return NULL;
}

int api_count(const void *context)
{
    const LinkedList *list = context;

    return list != NULL ? list->count : -1;
}

PhonebookIterator api_begin(const void *context)
{
    const LinkedList *list = context;

    return list != NULL ? list->head : NULL;
}

PhonebookIterator api_next(const void *context, PhonebookIterator iterator)
{
    (void)context;

    const ListNode *node = iterator;

    return node != NULL ? node->next : NULL;
}

PhonebookIterator api_prev(const void *context, PhonebookIterator iterator)
{
    (void)context;

    const ListNode *node = iterator;

    return node != NULL ? node->prev : NULL;
}

Contact *api_get(void *context, PhonebookIterator iterator)
{
    (void)context;

    ListNode *node = (ListNode *)iterator;

    return node != NULL ? node->data : NULL;
}

int api_add(void *context, const Contact *contact, PhonebookIterator *inserted)
{
    LinkedList *list = context;

    if (inserted != NULL)
        *inserted = NULL;

    if (list == NULL || contact == NULL)
        return -1;

    ListNode *position = find_position(list, contact, NULL, compare_contacts);
    ListNode *node = list_insert(list, position, contact);

    if (node == NULL)
        return -1;

    if (inserted != NULL)
        *inserted = node;

    return 0;
}

int api_remove(void *context, PhonebookIterator iterator, PhonebookIterator *next)
{
    LinkedList *list = context;
    ListNode *node = (ListNode *)iterator;

    if (next != NULL)
        *next = NULL;

    if (list == NULL || node == NULL)
        return -1;

    ListNode *position = node->next != NULL ? node->next : node->prev;

    if (list_remove(list, node) != 0)
        return -1;

    if (next != NULL)
        *next = position;

    return 0;
}

int api_update(void *context, PhonebookIterator iterator, PhonebookIterator *updated)
{
    LinkedList *list = context;
    ListNode *node = (ListNode *)iterator;

    if (updated != NULL)
        *updated = NULL;

    if (list == NULL || node == NULL)
        return -1;

    Contact *contact = node->data;
    ListNode *position = find_position(list, contact, node, compare_contacts);

    if (list_move_before(list, node, position) != 0)
        return -1;

    if (updated != NULL)
        *updated = node;

    return 0;
}

void api_clear(void *context)
{
    list_clear(context);
}

int pb_bind(PhonebookAPI *api, LinkedList *list)
{
    if (api == NULL || list == NULL || list->data_size != sizeof(Contact))
        return -1;

    *api = (PhonebookAPI){.context = list,
                          .count = api_count,
                          .begin = api_begin,
                          .next = api_next,
                          .prev = api_prev,
                          .get = api_get,
                          .add = api_add,
                          .remove = api_remove,
                          .update = api_update,
                          .clear = api_clear};

    return 0;
}