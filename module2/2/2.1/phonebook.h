#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "contact.h"
#include "phonebook_api.h"

#define MAX_CONTACTS 100

typedef struct
{
    Contact items[MAX_CONTACTS];
    int count;
} PhonebookArray;

int pb_add(PhonebookArray *pb, const Contact *contact);
int pb_remove(PhonebookArray *pb, int index);

int pb_bind(PhonebookAPI *api, PhonebookArray *pb);

#endif