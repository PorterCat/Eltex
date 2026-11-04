#include "contact_form.h"
#include "contact_properties.h"

#include <ncurses.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static void prompt(const char *label, char *dst, int keep)
{
    char input[MAX_STR] = "";

    echo();
    curs_set(1);
    mvprintw(LINES - 2, 0, "%s: ", label);
    clrtoeol();
    getnstr(input, MAX_STR - 1);
    noecho();
    curs_set(0);

    if (input[0] || !keep)
        snprintf(dst, MAX_STR, "%s", input);
}

int contact_form_run(Contact *contact, int editing)
{
    ContactProperty properties[CONTACT_PROPERTY_COUNT];
    int count = contact_properties_init(properties);
    int current = 0;

    if (contact == NULL || count < 0)
        return 0;

    for (;;)
    {
        const char *last_name = "";
        const char *first_name = "";
        const char *middle_name = "";
        int visible;
        int top;

        clear();

        if (editing)
        {
            cont_get_field(contact, FIELD_LAST_NAME, &last_name);
            cont_get_field(contact, FIELD_FIRST_NAME, &first_name);
            cont_get_field(contact, FIELD_MIDDLE_NAME, &middle_name);
            mvprintw(0, 2, "%s %s %s", last_name, first_name, middle_name);
        }
        else
        {
            mvaddstr(0, 2, "New contact");
        }

        mvaddstr(1, 2, "Up/Down: select | Enter: edit | q: cancel");

        visible = LINES - 5;

        if (visible < 1)
            visible = 1;

        top = current >= visible ? current - visible + 1 : 0;

        for (int i = top; i < count && i < top + visible; ++i)
        {
            const char *value = "";
            int row = i - top + 3;

            cont_get_field(contact, properties[i].field, &value);

            if (i == current)
            {
                attron(A_REVERSE);
                mvhline(row, 0, ' ', COLS);
            }

            mvprintw(row, 2, "%-24s %s", properties[i].label, value[0] ? value : "-");

            if (i == current)
                attroff(A_REVERSE);
        }

        if (editing)
            mvaddstr(LINES - 2, 2, "Enter: edit field | Esc: back");
        else
            mvaddstr(LINES - 2, 2, "Required: Last name, First name | Esc: finish | q: cancel");

        refresh();

        switch (getch())
        {
        case KEY_UP:
            if (current > 0)
                --current;
            break;

        case KEY_DOWN:
            if (current < count - 1)
                ++current;
            break;

        case '\n':
        case '\r':
        case KEY_ENTER:
        {
            const char *source = "";
            char old[MAX_STR];
            char value[MAX_STR];

            if (properties[current].item_index > 0)
            {
                const char *previous = "";

                if (cont_get_field(contact, (ContactField)(properties[current].field - 1),
                                   &previous) != 0 ||
                    !previous[0])
                {
                    mvaddstr(LINES - 1, 2, "Set previous item first");
                    clrtoeol();
                    refresh();
                    getch();
                    break;
                }
            }

            if (cont_get_field(contact, properties[current].field, &source) != 0)
                break;

            snprintf(old, sizeof(old), "%s", source);
            snprintf(value, sizeof(value), "%s", source);
            prompt(properties[current].label, value, editing);

            if (strcmp(old, value) != 0 &&
                cont_edit(contact, properties[current].field, value) != 0)
            {
                mvaddstr(LINES - 1, 2, "Cannot set property");
                clrtoeol();
                refresh();
                getch();
            }

            break;
        }

        case 'q':
            if (!editing)
                return 0;
            break;

        case 27:
            if (!contact_is_complete(contact))
            {
                move(LINES - 1, 0);
                clrtoeol();
                mvaddstr(LINES - 1, 2, "Last name and First name are required!");
                refresh();
                beep();
                getch();
                break;
            }

            return 1;
        }
    }
}