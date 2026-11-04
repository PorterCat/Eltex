#include "contact_card.h"
#include "contact_properties.h"

#include <ncurses.h>

void contact_card_show(const Contact *contact)
{
    ContactProperty properties[CONTACT_PROPERTY_COUNT];
    int count = contact_properties_init(properties);

    const char *last_name = "";
    const char *first_name = "";
    const char *middle_name = "";
    int row = 3;

    if (contact == NULL)
        return;

    cont_get_field(contact, FIELD_LAST_NAME, &last_name);
    cont_get_field(contact, FIELD_FIRST_NAME, &first_name);
    cont_get_field(contact, FIELD_MIDDLE_NAME, &middle_name);

    clear();

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
}