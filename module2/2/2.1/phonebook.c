#include "phonebook.h"
#include <stdarg.h>

int cont_set_fields(Contact *cont, ...)
{
    if (cont == NULL)
        return -1;

    va_list args;
    va_start(args, cont);

    while (1)
    {
        int flag = va_arg(args, int);
        if (flag == LAST_ARG)
            break;

        const char *value = va_arg(args, char *);
        if (value == NULL)
            continue;

        if (flag >= FIELD_PHONE_BASE && flag < FIELD_MESSENGER_BASE + MAX_ITEM)
        {
            int base, idx;
            char (*target_array)[MAX_STR];

            if (flag < FIELD_EMAIL_BASE)
            {
                base = FIELD_PHONE_BASE;
                target_array = cont->phones;
            }
            else if (flag < FIELD_SOCIAL_BASE)
            {
                base = FIELD_EMAIL_BASE;
                target_array = cont->emails;
            }
            else if (flag < FIELD_MESSENGER_BASE)
            {
                base = FIELD_SOCIAL_BASE;
                target_array = cont->socials;
            }
            else
            {
                base = FIELD_MESSENGER_BASE;
                target_array = cont->messengers;
            }

            idx = flag - base;

            if (idx < 0 || idx >= MAX_ITEM)
            {
                va_end(args);
                return -1;
            }

            strncpy(target_array[idx], value, MAX_STR - 1);
        }

        else
        {
            switch (flag)
            {
            case FIELD_FIRST_NAME:
                strncpy(cont->first_name, value, MAX_STR - 1);
                break;
            case FIELD_LAST_NAME:
                strncpy(cont->last_name, value, MAX_STR - 1);
                break;
            case FIELD_MIDDLE_NAME:
                strncpy(cont->middle_name, value, MAX_STR - 1);
                break;
            case FIELD_WORKPLACE:
                strncpy(cont->workplace, value, MAX_STR - 1);
                break;
            case FIELD_POSITION:
                strncpy(cont->position, value, MAX_STR - 1);
                break;
            default:
                va_end(args);
                return -1;
            }
        }
    }

    va_end(args);
    return 0;
}

int cont_get_field(const Contact *cont, ContactField field, const char **value)
{
    if (cont == NULL || value == NULL)
    {
        return -1;
    }

    switch (field)
    {
    case FIELD_FIRST_NAME:
        *value = cont->first_name;
        return 0;
    case FIELD_LAST_NAME:
        *value = cont->last_name;
        return 0;
    case FIELD_MIDDLE_NAME:
        *value = cont->middle_name;
        return 0;
    case FIELD_WORKPLACE:
        *value = cont->workplace;
        return 0;
    case FIELD_POSITION:
        *value = cont->position;
        return 0;
    }

    if (field >= FIELD_PHONE_BASE && field < FIELD_EMAIL_BASE)
    {
        *value = cont->phones[field - FIELD_PHONE_BASE];
    }
    else if (field >= FIELD_EMAIL_BASE && field < FIELD_SOCIAL_BASE)
    {
        *value = cont->emails[field - FIELD_EMAIL_BASE];
    }
    else if (field >= FIELD_SOCIAL_BASE && field < FIELD_MESSENGER_BASE)
    {
        *value = cont->socials[field - FIELD_SOCIAL_BASE];
    }
    else if (field >= FIELD_MESSENGER_BASE && field < FIELD_MESSENGER_BASE + MAX_ITEM)
    {
        *value = cont->messengers[field - FIELD_MESSENGER_BASE];
    }
    else
    {
        return -1;
    }

    return 0;
}

int pb_remove(Phonebook *pb, int index)
{
    if (pb == NULL || index < 0 || index >= pb->count)
        return -1;

    Contact *con_to_remove = &pb->items[index];

    int remaining = pb->count - index - 1;
    if (remaining > 0)
    {
        memmove(con_to_remove, con_to_remove + 1, remaining * sizeof(pb->items[0]));
    }

    pb->count--;
    return 0;
}

int pb_add(Phonebook *pb, const Contact *con)
{
    if (pb == NULL || con == NULL || pb->count >= MAX_CONTACTS)
        return -1;

    pb->items[pb->count] = *con;
    pb->count++;

    return 0;
}

int pb_get(const Phonebook *pb, int index, Contact *cont)
{
    if (pb == NULL || index < 0 || index >= pb->count)
        return -1;

    *cont = pb->items[index];
    return 0;
}