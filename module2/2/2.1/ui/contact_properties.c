#include "contact_properties.h"

#include <stdio.h>

static const struct
{
    const char *label;
    ContactField field;
} fields[] = {{"Last name *", FIELD_LAST_NAME},
              {"First name *", FIELD_FIRST_NAME},
              {"Middle name", FIELD_MIDDLE_NAME},
              {"Workplace", FIELD_WORKPLACE},
              {"Position", FIELD_POSITION}};

static const struct
{
    const char *label;
    ContactField base;
} lists[] = {{"Phone", FIELD_PHONE_BASE},
             {"Email", FIELD_EMAIL_BASE},
             {"Social network", FIELD_SOCIAL_BASE},
             {"Messenger", FIELD_MESSENGER_BASE}};

int contact_properties_init(ContactProperty *properties)
{
    int count = 0;

    if (properties == NULL)
        return -1;

    for (size_t i = 0; i < sizeof(fields) / sizeof(fields[0]); ++i)
    {
        snprintf(properties[count].label, sizeof(properties[count].label), "%s", fields[i].label);
        properties[count].field = fields[i].field;
        properties[count++].item_index = -1;
    }

    for (size_t type = 0; type < sizeof(lists) / sizeof(lists[0]); ++type)
    {
        for (int i = 0; i < MAX_ITEM; ++i)
        {
            snprintf(properties[count].label, sizeof(properties[count].label), "%s %d%s",
                     lists[type].label, i + 1, i ? " (add.)" : "");
            properties[count].field = (ContactField)(lists[type].base + i);
            properties[count++].item_index = i;
        }
    }

    return count;
}

int contact_is_complete(const Contact *contact)
{
    const char *last_name;
    const char *first_name;

    return contact != NULL && cont_get_field(contact, FIELD_LAST_NAME, &last_name) == 0 &&
           cont_get_field(contact, FIELD_FIRST_NAME, &first_name) == 0 && last_name[0] &&
           first_name[0];
}