#ifndef PHONEBOOK_API_H
#define PHONEBOOK_API_H

#include "contact.h"

typedef const void *PhonebookIterator;

typedef struct
{
    void *context;

    int (*count)(const void *context);

    PhonebookIterator (*begin)(const void *context);
    PhonebookIterator (*next)(const void *context, PhonebookIterator iterator);
    PhonebookIterator (*prev)(const void *context, PhonebookIterator iterator);

    Contact *(*get)(void *context, PhonebookIterator iterator);

    int (*add)(void *context, const Contact *contact, PhonebookIterator *inserted);

    int (*remove)(void *context, PhonebookIterator iterator, PhonebookIterator *next);

    int (*update)(void *context, PhonebookIterator iterator, PhonebookIterator *updated);

    void (*clear)(void *context);

} PhonebookAPI;

#endif