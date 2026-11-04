#ifndef PHONEBOOK_WRAPPER_H
#define PHONEBOOK_WRAPPER_H

#include "../../2/2.1/phonebook_api.h"
#include "linked_list.h"

int pb_bind(PhonebookAPI *api, LinkedList *list);

#endif