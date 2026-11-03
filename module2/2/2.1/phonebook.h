#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "contact.h"
#include "phonebook_api.h"

#define MAX_CONTACTS 100

typedef struct
{
    Contact items[MAX_CONTACTS];
    int count;
} Phonebook;

int pb_add(Phonebook *pb, const Contact *contact);
int pb_remove(Phonebook *pb, int index);
int pb_get(const Phonebook *pb, int index, Contact *contact);
int pb_update(Phonebook *pb, int index, const Contact *cont);

int pb_bind(PhonebookAPI *api, Phonebook *pb);

#endif
