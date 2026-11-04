#include "ui.h"
#include "contact_card.h"
#include "contact_form.h"
#include "contact_list.h"
#include "contact_properties.h"
#include "../storage.h"

#include <ncurses.h>

#define DATA_FILE "phonebook.dat"

void save(const PhonebookAPI *pb)
{
    if (storage_save(pb, DATA_FILE) != 0)
    {
        mvprintw(LINES - 1, 0, "Save error!");
        clrtoeol();
        refresh();
        getch();
    }
}

void show_message(const char *message)
{
    mvaddstr(LINES - 1, 2, message);
    clrtoeol();
    refresh();
    getch();
}

int ui_run(PhonebookAPI *pb)
{
    if (pb == NULL || pb->begin == NULL || pb->next == NULL || pb->prev == NULL ||
        pb->get == NULL || pb->add == NULL || pb->remove == NULL || pb->update == NULL)
    {
        return -1;
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    set_escdelay(25);
    curs_set(0);

    ContactList list;
    contact_list_init(&list, pb);

    for (;;)
    {
        contact_list_draw(&list, pb);

        switch (getch())
        {
        case KEY_UP:
            contact_list_move_up(&list, pb);
            break;

        case KEY_DOWN:
            contact_list_move_down(&list, pb);
            break;

        case 'a':
        {
            Contact contact = {0};
            PhonebookIterator inserted = NULL;

            if (contact_form_run(&contact, 0) && contact_is_complete(&contact))
            {
                if (pb->add(pb->context, &contact, &inserted) == 0 && inserted != NULL)
                {
                    contact_list_select(&list, pb, inserted);
                    save(pb);
                }
                else
                {
                    show_message("Cannot add contact");
                }
            }

            break;
        }

        case 'd':
            if (list.selected != NULL)
            {
                mvprintw(LINES - 1, 0, "Delete contact? (y/n): ");
                clrtoeol();
                refresh();

                int confirm = getch();

                if (confirm == 'y' || confirm == 'Y')
                {
                    PhonebookIterator next = NULL;

                    if (pb->remove(pb->context, list.selected, &next) == 0)
                    {
                        contact_list_select(&list, pb, next);
                        save(pb);
                    }
                }
            }

            break;

        case '\n':
        case '\r':
        case KEY_ENTER:
            if (list.selected != NULL)
            {
                PhonebookIterator iterator = list.selected;
                Contact *contact = pb->get(pb->context, iterator);

                if (contact != NULL)
                {
                    contact_card_show(contact);

                    if (getch() == 'e')
                    {
                        PhonebookIterator updated = NULL;

                        if (contact_form_run(contact, 1))
                        {
                            if (pb->update(pb->context, iterator, &updated) == 0 && updated != NULL)
                            {
                                contact_list_select(&list, pb, updated);
                                save(pb);
                            }
                            else
                            {
                                show_message("Cannot update contact");
                            }
                        }
                    }
                }
            }

            break;

        case 'q':
            endwin();
            return 0;
        }
    }
}