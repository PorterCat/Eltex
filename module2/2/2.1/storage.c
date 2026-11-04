#include "storage.h"

#include <errno.h>
#include <stdio.h>

int storage_load(PhonebookAPI *pb, const char *path)
{
    FILE *file;
    int count;

    if (pb == NULL || path == NULL || pb->clear == NULL || pb->add == NULL)
        return -1;

    file = fopen(path, "rb");

    if (file == NULL)
    {
        if (errno == ENOENT)
        {
            pb->clear(pb->context);
            return 0;
        }

        return -1;
    }

    if (fread(&count, sizeof(count), 1, file) != 1 || count < 0)
    {
        fclose(file);
        pb->clear(pb->context);
        return -1;
    }

    pb->clear(pb->context);

    for (int i = 0; i < count; ++i)
    {
        Contact contact;

        if (fread(&contact, sizeof(contact), 1, file) != 1 ||
            pb->add(pb->context, &contact, NULL) != 0)
        {
            fclose(file);
            pb->clear(pb->context);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

int storage_save(const PhonebookAPI *pb, const char *path)
{
    FILE *file;
    PhonebookIterator iterator;
    int count;

    if (pb == NULL || path == NULL || pb->count == NULL || pb->begin == NULL || pb->next == NULL ||
        pb->get == NULL)
    {
        return -1;
    }

    count = pb->count(pb->context);

    if (count < 0)
        return -1;

    file = fopen(path, "wb");

    if (file == NULL)
        return -1;

    if (fwrite(&count, sizeof(count), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }

    iterator = pb->begin(pb->context);

    for (int i = 0; i < count; ++i)
    {
        const Contact *contact;

        if (iterator == NULL)
        {
            fclose(file);
            return -1;
        }

        contact = pb->get(pb->context, iterator);

        if (contact == NULL || fwrite(contact, sizeof(*contact), 1, file) != 1)
        {
            fclose(file);
            return -1;
        }

        iterator = pb->next(pb->context, iterator);
    }

    return fclose(file) == 0 ? 0 : -1;
}