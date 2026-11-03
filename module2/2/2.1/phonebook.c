#include "phonebook.h"

#include <string.h>

int pb_remove(Phonebook *pb, int index)
{
    if (pb == NULL || index < 0 || index >= pb->count)
        return -1;

    int remaining = pb->count - index - 1;

    if (remaining > 0)
        memmove(&pb->items[index], &pb->items[index + 1], remaining * sizeof(pb->items[0]));

    pb->count--;
    return 0;
}

int pb_add(Phonebook *pb, const Contact *cont)
{
    if (pb == NULL || cont == NULL || pb->count < 0 || pb->count >= MAX_CONTACTS)
        return -1;

    pb->items[pb->count++] = *cont;
    return 0;
}

int pb_get(const Phonebook *pb, int index, Contact *cont)
{
    if (pb == NULL || cont == NULL || index < 0 || index >= pb->count)
        return -1;

    *cont = pb->items[index];
    return 0;
}

int pb_update(Phonebook *pb, int index, const Contact *cont)
{
    if (pb == NULL || cont == NULL || index < 0 || index >= pb->count)
        return -1;

    pb->items[index] = *cont;

    return 0;
}

int api_count(const void *context)
{
    const Phonebook *pb = context;

    return pb != NULL ? pb->count : -1;
}

int api_get(const void *context, int index, Contact *contact)
{
    return pb_get(context, index, contact);
}

int api_add(void *context, const Contact *contact)
{
    return pb_add(context, contact);
}

int api_remove(void *context, int index)
{
    return pb_remove(context, index);
}

int api_update(void *context, int index, const Contact *cont)
{
    return pb_update(context, index, cont);
}

void api_clear(void *context)
{
    Phonebook *pb = context;

    if (pb != NULL)
        pb->count = 0;
}

int pb_bind(PhonebookAPI *api, Phonebook *pb)
{
    if (api == NULL || pb == NULL)
        return -1;

    *api = (PhonebookAPI){
        .context = pb,
        .count = api_count,
        .get = api_get,
        .add = api_add,
        .remove = api_remove,
        .update = api_update,
        .clear = api_clear
    };

    return 0;
}