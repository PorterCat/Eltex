#include "ui.h"
#include "storage.h"

#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#define DATA_FILE "phonebook.dat"

typedef struct
{
    const char *label;
    ContactField field;
} FieldInfo;

typedef struct
{
    const char *label;
    ContactField base;
} ListInfo;

typedef struct
{
    char label[48];
    ContactField field;
    int item_index;
} Property;

static const FieldInfo fields[] = {{"Last name *", FIELD_LAST_NAME},
                                   {"First name *", FIELD_FIRST_NAME},
                                   {"Middle name", FIELD_MIDDLE_NAME},
                                   {"Workplace", FIELD_WORKPLACE},
                                   {"Position", FIELD_POSITION}};

static const ListInfo lists[] = {{"Phone", FIELD_PHONE_BASE},
                                 {"Email", FIELD_EMAIL_BASE},
                                 {"Social network", FIELD_SOCIAL_BASE},
                                 {"Messenger", FIELD_MESSENGER_BASE}};

#define FIELD_COUNT (sizeof(fields) / sizeof(fields[0]))
#define LIST_COUNT (sizeof(lists) / sizeof(lists[0]))
#define PROPERTY_COUNT (FIELD_COUNT + LIST_COUNT * MAX_ITEM)

static int selected;

static void save(Phonebook *pb)
{
    if (storage_save(pb, DATA_FILE) != 0)
    {
        mvprintw(LINES - 1, 0, "Save error!");
        clrtoeol();
        refresh();
        getch();
    }
}

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

static int make_properties(Property *properties)
{
    int count = 0;

    for (size_t i = 0; i < FIELD_COUNT; ++i)
    {
        snprintf(properties[count].label, sizeof(properties[count].label), "%s", fields[i].label);
        properties[count].field = fields[i].field;
        properties[count++].item_index = -1;
    }

    for (size_t type = 0; type < LIST_COUNT; ++type)
    {
        for (int i = 0; i < MAX_ITEM; ++i)
        {
            snprintf(properties[count].label, sizeof(properties[count].label), "%s %d%s",
                     lists[type].label, i + 1, i ? " (add.)" : "");
            properties[count].field = (ContactField)(lists[type].base + i);
            properties[count++].item_index = i;
        }
    }

    return count;
}

static int contact_is_complete(Contact *contact)
{
    const char *last_name;
    const char *first_name;

    return cont_get_field(contact, FIELD_LAST_NAME, &last_name) == 0 &&
           cont_get_field(contact, FIELD_FIRST_NAME, &first_name) == 0 && last_name[0] &&
           first_name[0];
}

static int edit_contact(Contact *contact, int editing)
{
    Property properties[PROPERTY_COUNT];
    int count = make_properties(properties);
    int current = 0;

    while (1)
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
            mvprintw(row, 2, "%-24s %s", properties[i].label, value[0] ? value : "—");
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
            if (!editing && !contact_is_complete(contact))
            {
                move(LINES - 1, 0);
                clrtoeol();
                mvaddstr(LINES - 1, 2, "Cannot finish: Last name and First name are required!");
                refresh();
                beep();
                getch();
                break;
            }
            return 1;
        }
    }
}

static int show_contact(Contact *contact)
{
    Property properties[PROPERTY_COUNT];
    int count = make_properties(properties);
    const char *last_name = "";
    const char *first_name = "";
    const char *middle_name = "";
    int row = 3;

    clear();
    cont_get_field(contact, FIELD_LAST_NAME, &last_name);
    cont_get_field(contact, FIELD_FIRST_NAME, &first_name);
    cont_get_field(contact, FIELD_MIDDLE_NAME, &middle_name);
    mvprintw(1, 2, "%s %s %s", last_name, first_name, middle_name);

    for (int i = 3; i < count && row < LINES - 2; ++i)
    {
        const char *value = "";
        if (cont_get_field(contact, properties[i].field, &value) == 0 && value[0])
            mvprintw(row++, 2, "%s: %s", properties[i].label, value);
    }

    mvprintw(LINES - 2, 2, "e - edit | Esc - back");
    clrtoeol();
    refresh();

    if (getch() == 'e')
    {
        Contact edited = *contact;
        if (edit_contact(&edited, 1) && contact_is_complete(&edited))
        {
            *contact = edited;
            return 1;
        }
    }
    return 0;
}

static void draw_table(Phonebook *pb)
{
    clear();
    attron(A_BOLD);
    mvprintw(0, 0, "%-4s %-24s %-24s %-24s", "#", "Last name", "First name", "Phone");
    attroff(A_BOLD);

    for (int i = 0; i < pb->count && i < LINES - 4; ++i)
    {
        const char *last_name = "";
        const char *first_name = "";
        const char *phone = "";
        cont_get_field(&pb->items[i], FIELD_LAST_NAME, &last_name);
        cont_get_field(&pb->items[i], FIELD_FIRST_NAME, &first_name);
        cont_get_field(&pb->items[i], FIELD_PHONE_BASE, &phone);

        if (i == selected)
            attron(A_REVERSE);
        mvprintw(i + 1, 0, "%-4d %-24.24s %-24.24s %-24.24s", i + 1, last_name, first_name, phone);
        if (i == selected)
            attroff(A_REVERSE);
    }

    mvprintw(LINES - 2, 0, "Up/Down - select | Enter - open | a - add | d - delete | q - quit");
    clrtoeol();
    refresh();
}

int ui_run(Phonebook *pb)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    set_escdelay(25);
    curs_set(0);

    for (;;)
    {
        draw_table(pb);
        switch (getch())
        {
        case KEY_UP:
            if (selected > 0)
                --selected;
            break;
        case KEY_DOWN:
            if (selected < pb->count - 1)
                ++selected;
            break;
        case 'a':
        {
            Contact contact = {0};
            if (pb->count >= MAX_CONTACTS)
                break;

            if (edit_contact(&contact, 0) && contact_is_complete(&contact))
            {
                pb->items[pb->count++] = contact;
                selected = pb->count - 1;
                save(pb);
            }
            break;
        }
        case 'd':
            if (pb->count > 0)
            {
                mvprintw(LINES - 1, 0, "Delete contact? (y/n): ");
                clrtoeol();
                refresh();
                int confirm = getch();
                if (confirm == 'y' || confirm == 'Y')
                {
                    pb_remove(pb, selected);
                    if (selected >= pb->count && selected > 0)
                        --selected;
                    save(pb);
                }
            }
            break;
        case '\n':
        case '\r':
        case KEY_ENTER:
            if (pb->count > 0 && show_contact(&pb->items[selected]))
                save(pb);
            break;
        case 'q':
            endwin();
            return 0;
        }
    }
}