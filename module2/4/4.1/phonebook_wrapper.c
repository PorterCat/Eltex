#include "phonebook_wrapper.h"

#include <string.h>

int compare_contacts(const void *left, const void *right)
{
    const Contact *a = left;
    const Contact *b = right;

    int result = strcmp(a->last_name, b->last_name);

    if (result != 0)
        return result;

    return strcmp(a->first_name, b->first_name);
}

int api_count(const void *context)
{
    return list_count(context);
}

int api_get(const void *context, int index, Contact *contact)
{
    return list_get(context, index, contact);
}

int api_add(void *context, const Contact *contact)
{
    return list_insert_sorted(context, contact, compare_contacts);
}

int api_remove(void *context, int index)
{
    return list_remove(context, index);
}

int api_update(void *context, int index, const Contact *contact)
{
    return list_update_sorted(context, index, contact, compare_contacts);
}

void api_clear(void *context)
{
    list_clear(context);
}

int pb_bind(PhonebookAPI *api, LinkedList *list)
{
    if (api == NULL || list == NULL)
        return -1;

    *api = (PhonebookAPI){.context = list,
                          .count = api_count,
                          .get = api_get,
                          .add = api_add,
                          .remove = api_remove,
                          .update = api_update,
                          .clear = api_clear};

    return 0;
}