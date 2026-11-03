#ifndef PHONEBOOK_API_H
#define PHONEBOOK_API_H

#include "contact.h"

typedef struct
{
    void *context;

    int (*count)(const void *context);
    int (*get)(const void *context, int index, Contact *contact);
    int (*add)(void *context, const Contact *contact);
    int (*remove)(void *context, int index);
    int (*update)(void *context, int index, const Contact *contact);
    void (*clear)(void *context);

} PhonebookAPI;

#endif