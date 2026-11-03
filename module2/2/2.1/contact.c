#include "contact.h"

#include <stdarg.h>
#include <string.h>

static void copy_string(char *destination, const char *source)
{
    strncpy(destination, source, MAX_STR - 1);
    destination[MAX_STR - 1] = '\0';
}

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

        char *value = va_arg(args, char *);

        if (value == NULL)
            continue;

        if (flag >= FIELD_PHONE_BASE && flag < FIELD_MESSENGER_BASE + MAX_ITEM)
        {
            int base;
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

            int index = flag - base;

            if (index < 0 || index >= MAX_ITEM)
            {
                va_end(args);
                return -1;
            }

            copy_string(target_array[index], value);
        }
        else
        {
            switch (flag)
            {
            case FIELD_FIRST_NAME:
                copy_string(cont->first_name, value);
                break;
            case FIELD_LAST_NAME:
                copy_string(cont->last_name, value);
                break;
            case FIELD_MIDDLE_NAME:
                copy_string(cont->middle_name, value);
                break;
            case FIELD_WORKPLACE:
                copy_string(cont->workplace, value);
                break;
            case FIELD_POSITION:
                copy_string(cont->position, value);
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
        return -1;

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
    default:
        break;
    }

    if (field >= FIELD_PHONE_BASE && field < FIELD_EMAIL_BASE)
        *value = cont->phones[field - FIELD_PHONE_BASE];
    else if (field >= FIELD_EMAIL_BASE && field < FIELD_SOCIAL_BASE)
        *value = cont->emails[field - FIELD_EMAIL_BASE];
    else if (field >= FIELD_SOCIAL_BASE && field < FIELD_MESSENGER_BASE)
        *value = cont->socials[field - FIELD_SOCIAL_BASE];
    else if (field >= FIELD_MESSENGER_BASE && field < FIELD_MESSENGER_BASE + MAX_ITEM)
        *value = cont->messengers[field - FIELD_MESSENGER_BASE];
    else
        return -1;

    return 0;
}