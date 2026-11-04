#include "contact.h"

#include <stdarg.h>
#include <stddef.h>
#include <string.h>

int field_offset(int field, size_t *offset)
{
    if (field < FIELD_LAST_NAME || field >= FIELD_MESSENGER_BASE + MAX_ITEM)
        return -1;

    *offset = (size_t)(field - FIELD_LAST_NAME) * MAX_STR;

    return 0;
}

int cont_set_fields(Contact *cont, ...)
{
    if (cont == NULL)
        return -1;

    va_list args;
    va_start(args, cont);

    while (1)
    {
        int field = va_arg(args, int);

        if (field == LAST_ARG)
            break;

        char *value = va_arg(args, char *);
        size_t offset;

        if (field_offset(field, &offset) != 0)
        {
            va_end(args);
            return -1;
        }

        if (value != NULL)
        {
            char *dest = (char *)cont + offset;
            strncpy(dest, value, MAX_STR - 1);
            dest[MAX_STR - 1] = '\0';
        }
    }

    va_end(args);

    return 0;
}

int cont_get_field(const Contact *cont, ContactField field, const char **value)
{
    size_t offset;

    if (cont == NULL || value == NULL || field_offset(field, &offset) != 0)
        return -1;

    *value = (const char *)cont + offset;

    return 0;
}