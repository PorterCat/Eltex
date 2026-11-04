#include "phonebook.h"

#include <stddef.h>
#include <string.h>

int pb_add(PhonebookArray *pb, const Contact *contact)
{
    if (pb == NULL || contact == NULL || pb->count < 0 || pb->count >= MAX_CONTACTS)
        return -1;

    pb->items[pb->count++] = *contact;

    return 0;
}

int pb_remove(PhonebookArray *pb, int index)
{
    if (pb == NULL || index < 0 || index >= pb->count)
        return -1;

    int remaining = pb->count - index - 1;

    if (remaining > 0)
    {
        memmove(&pb->items[index], &pb->items[index + 1], (size_t)remaining * sizeof(pb->items[0]));
    }

    --pb->count;
    memset(&pb->items[pb->count], 0, sizeof(pb->items[0]));

    return 0;
}

int array_index(const PhonebookArray *pb, PhonebookIterator iterator, int *index)
{
    if (pb == NULL || iterator == NULL || index == NULL)
        return -1;

    ptrdiff_t offset = (const Contact *)iterator - pb->items;

    if (offset < 0 || offset >= pb->count)
        return -1;

    *index = (int)offset;

    return 0;
}

int api_count(const void *context)
{
    const PhonebookArray *pb = context;

    return pb != NULL ? pb->count : -1;
}

PhonebookIterator api_begin(const void *context)
{
    const PhonebookArray *pb = context;

    return pb != NULL && pb->count > 0 ? (PhonebookIterator)&pb->items[0] : NULL;
}

PhonebookIterator api_next(const void *context, PhonebookIterator iterator)
{
    const PhonebookArray *pb = context;
    int index;

    if (array_index(pb, iterator, &index) != 0 || index + 1 >= pb->count)
        return NULL;

    return &pb->items[index + 1];
}

PhonebookIterator api_prev(const void *context, PhonebookIterator iterator)
{
    const PhonebookArray *pb = context;
    int index;

    if (array_index(pb, iterator, &index) != 0 || index == 0)
        return NULL;

    return &pb->items[index - 1];
}

Contact *api_get(void *context, PhonebookIterator iterator)
{
    PhonebookArray *pb = context;
    int index;

    if (array_index(pb, iterator, &index) != 0)
        return NULL;

    return &pb->items[index];
}

int api_add(void *context, const Contact *contact, PhonebookIterator *inserted)
{
    PhonebookArray *pb = context;

    if (inserted != NULL)
        *inserted = NULL;

    if (pb_add(pb, contact) != 0)
        return -1;

    if (inserted != NULL)
        *inserted = &pb->items[pb->count - 1];

    return 0;
}

int api_remove(void *context, PhonebookIterator iterator, PhonebookIterator *next)
{
    PhonebookArray *pb = context;
    int index;

    if (next != NULL)
        *next = NULL;

    if (array_index(pb, iterator, &index) != 0)
        return -1;

    if (pb_remove(pb, index) != 0)
        return -1;

    if (next != NULL && pb->count > 0)
    {
        int next_index = index < pb->count ? index : pb->count - 1;
        *next = &pb->items[next_index];
    }

    return 0;
}

int api_update(void *context, PhonebookIterator iterator, PhonebookIterator *updated)
{
    PhonebookArray *pb = context;
    int index;

    if (updated != NULL)
        *updated = NULL;

    if (array_index(pb, iterator, &index) != 0)
        return -1;

    if (updated != NULL)
        *updated = &pb->items[index];

    return 0;
}

void api_clear(void *context)
{
    PhonebookArray *pb = context;

    if (pb == NULL)
        return;

    memset(pb->items, 0, sizeof(pb->items));
    pb->count = 0;
}

int pb_bind(PhonebookAPI *api, PhonebookArray *pb)
{
    if (api == NULL || pb == NULL)
        return -1;

    *api = (PhonebookAPI){.context = pb,
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