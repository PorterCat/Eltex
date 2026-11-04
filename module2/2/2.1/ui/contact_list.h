#ifndef CONTACT_LIST_H
#define CONTACT_LIST_H

#include "../phonebook_api.h"

typedef struct
{
    PhonebookIterator selected;
    PhonebookIterator top;
    int selected_row;
} ContactList;

void contact_list_init(ContactList *list, const PhonebookAPI *pb);
void contact_list_select(ContactList *list, const PhonebookAPI *pb, PhonebookIterator iterator);
void contact_list_move_up(ContactList *list, const PhonebookAPI *pb);
void contact_list_move_down(ContactList *list, const PhonebookAPI *pb);
void contact_list_draw(ContactList *list, const PhonebookAPI *pb);

#endif