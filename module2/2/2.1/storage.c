#include "storage.h"

#include <stdio.h>
#include <string.h>

int storage_load(Phonebook *pb, const char *path)
{
    if (!pb || !path)
        return -1;

    FILE *file = fopen(path, "rb");

    if (!file)
    {
        memset(pb, 0, sizeof(*pb));
        return 0;
    }

    int count;

    if (fread(&count, sizeof(count), 1, file) != 1 || count < 0 || count > MAX_CONTACTS ||
        fread(pb->items, sizeof(Contact), count, file) != (size_t)count)
    {
        fclose(file);
        memset(pb, 0, sizeof(*pb));
        return -1;
    }

    pb->count = count;
    fclose(file);
    return 0;
}

int storage_save(const Phonebook *pb, const char *path)
{
    if (!pb || !path || pb->count < 0 || pb->count > MAX_CONTACTS)
        return -1;

    FILE *file = fopen(path, "wb");

    if (!file)
        return -1;

    int ok = fwrite(&pb->count, sizeof(pb->count), 1, file) == 1 &&
             fwrite(pb->items, sizeof(Contact), pb->count, file) == (size_t)pb->count;

    if (fclose(file) != 0)
        ok = 0;

    return ok ? 0 : -1;
}