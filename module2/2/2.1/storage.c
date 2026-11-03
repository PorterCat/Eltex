#include "storage.h"

#include <errno.h>
#include <stdio.h>

static int api_is_valid(const PhonebookAPI *pb)
{
    return pb != NULL && pb->context != NULL && pb->count != NULL && pb->get != NULL &&
           pb->add != NULL && pb->clear != NULL;
}

int storage_load(const PhonebookAPI *pb, const char *path)
{
    if (!api_is_valid(pb) || path == NULL || pb->count(pb->context) != 0)
        return -1;

    FILE *file = fopen(path, "rb");

    if (file == NULL)
        return errno == ENOENT ? 0 : -1;

    int count = 0;
    int result = 0;

    if (fread(&count, sizeof(count), 1, file) != 1 || count < 0)
        result = -1;

    for (int i = 0; result == 0 && i < count; ++i)
    {
        Contact contact;

        if (fread(&contact, sizeof(contact), 1, file) != 1 || pb->add(pb->context, &contact) != 0)
            result = -1;
    }

    if (fclose(file) != 0)
        result = -1;

    if (result != 0)
        pb->clear(pb->context);

    return result;
}

int storage_save(const PhonebookAPI *pb, const char *path)
{
    if (!api_is_valid(pb) || path == NULL)
        return -1;

    int count = pb->count(pb->context);

    if (count < 0)
        return -1;

    FILE *file = fopen(path, "wb");

    if (file == NULL)
        return -1;

    int result = fwrite(&count, sizeof(count), 1, file) == 1 ? 0 : -1;

    for (int i = 0; result == 0 && i < count; ++i)
    {
        Contact contact;

        if (pb->get(pb->context, i, &contact) != 0 ||
            fwrite(&contact, sizeof(contact), 1, file) != 1)
            result = -1;
    }

    if (fclose(file) != 0)
        result = -1;

    return result;
}