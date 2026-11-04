#ifndef CONTACT_PROPERTIES_H
#define CONTACT_PROPERTIES_H

#include "../contact.h"

#define CONTACT_PROPERTY_COUNT (5 + 4 * MAX_ITEM)

typedef struct
{
    char label[48];
    ContactField field;
    int item_index;
} ContactProperty;

int contact_properties_init(ContactProperty *properties);
int contact_is_complete(const Contact *contact);

#endif