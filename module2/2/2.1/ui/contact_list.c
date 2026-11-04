#include "contact_list.h"

#include <ncurses.h>
#include <stddef.h>

static int visible_rows(void)
{
    int visible = LINES - 4;

    return visible > 0 ? visible : 1;
}

void contact_list_init(ContactList *list, const PhonebookAPI *pb)
{
    list->selected = pb->begin(pb->context);
    list->top = list->selected;
    list->selected_row = 0;
}

void contact_list_select(ContactList *list, const PhonebookAPI *pb, PhonebookIterator iterator)
{
    list->selected = iterator;
    list->top = iterator;
    list->selected_row = 0;

    if (iterator == NULL || pb == NULL || pb->prev == NULL)
        return;

    int visible = LINES - 4;

    if (visible < 2)
        return;

    PhonebookIterator previous = pb->prev(pb->context, iterator);

    if (previous != NULL)
    {
        list->top = previous;
        list->selected_row = 1;
    }
}

void contact_list_move_up(ContactList *list, const PhonebookAPI *pb)
{
    PhonebookIterator previous;

    if (list->selected == NULL)
        return;

    previous = pb->prev(pb->context, list->selected);

    if (previous == NULL)
        return;

    list->selected = previous;

    if (list->selected_row > 0)
        --list->selected_row;
    else
        list->top = pb->prev(pb->context, list->top);
}

void contact_list_move_down(ContactList *list, const PhonebookAPI *pb)
{
    PhonebookIterator next;

    if (list->selected == NULL)
        return;

    next = pb->next(pb->context, list->selected);

    if (next == NULL)
        return;

    list->selected = next;

    if (list->selected_row < visible_rows() - 1)
        ++list->selected_row;
    else
        list->top = pb->next(pb->context, list->top);
}

void contact_list_draw(ContactList *list, const PhonebookAPI *pb)
{
    PhonebookIterator iterator = list->top;
    int visible = visible_rows();

    if (list->selected_row >= visible)
    {
        int shift = list->selected_row - visible + 1;

        while (shift-- > 0 && list->top != NULL)
            list->top = pb->next(pb->context, list->top);

        list->selected_row = visible - 1;
        iterator = list->top;
    }

    clear();

    attron(A_BOLD);
    mvprintw(0, 0, "%-24s %-24s %-24s", "Last name", "First name", "Phone");
    attroff(A_BOLD);

    if (iterator == NULL)
        mvaddstr(2, 2, "No contacts");

    for (int row = 0; row < visible && iterator != NULL; ++row)
    {
        const Contact *contact = pb->get(pb->context, iterator);

        if (contact != NULL)
        {
            const char *last_name = "";
            const char *first_name = "";
            const char *phone = "";

            cont_get_field(contact, FIELD_LAST_NAME, &last_name);
            cont_get_field(contact, FIELD_FIRST_NAME, &first_name);
            cont_get_field(contact, FIELD_PHONE_BASE, &phone);

            if (iterator == list->selected)
                attron(A_REVERSE);

            mvprintw(row + 1, 0, "%-24.24s %-24.24s %-24.24s", last_name, first_name, phone);

            if (iterator == list->selected)
                attroff(A_REVERSE);
        }

        iterator = pb->next(pb->context, iterator);
    }

    mvprintw(LINES - 2, 0, "Up/Down - select | Enter - open | a - add | d - delete | q - quit");
    clrtoeol();
    refresh();
}