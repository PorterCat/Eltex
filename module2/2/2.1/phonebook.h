#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#define MAX_CONTACTS 100
#define MAX_STR 128
#define MAX_ITEM 3

#define cont_init(ptr_c, l_name, f_name, ...)                                                      \
    do                                                                                             \
    {                                                                                              \
        strncpy((ptr_c)->last_name, (l_name), MAX_STR - 1);                                        \
        strncpy((ptr_c)->first_name, (f_name), MAX_STR - 1);                                       \
        cont_set_fields((ptr_c), __VA_ARGS__, LAST_ARG);                                           \
    } while (0)

#define cont_edit(ptr_c, ...) cont_set_fields((ptr_c), __VA_ARGS__, LAST_ARG)

#define FIELD_PHONE(idx) (FIELD_PHONE_BASE + (idx))
#define FIELD_EMAIL(idx) (FIELD_EMAIL_BASE + (idx))
#define FIELD_SOCIAL(idx) (FIELD_SOCIAL_BASE + (idx))
#define FIELD_MESSENGER(idx) (FIELD_MESSENGER_BASE + (idx))

typedef enum
{
    LAST_ARG = 0,

    FIELD_FIRST_NAME,
    FIELD_LAST_NAME,

    FIELD_MIDDLE_NAME,
    FIELD_WORKPLACE,
    FIELD_POSITION,

    FIELD_PHONE_BASE = 6,
    FIELD_EMAIL_BASE = FIELD_PHONE_BASE + MAX_ITEM,
    FIELD_SOCIAL_BASE = FIELD_EMAIL_BASE + MAX_ITEM,
    FIELD_MESSENGER_BASE = FIELD_SOCIAL_BASE + MAX_ITEM,
} ContactField;

typedef struct
{
    char last_name[MAX_STR];
    char first_name[MAX_STR];

    char middle_name[MAX_STR];
    char workplace[MAX_STR];
    char position[MAX_STR];

    char phones[MAX_ITEM][MAX_STR];
    char emails[MAX_ITEM][MAX_STR];
    char socials[MAX_ITEM][MAX_STR];
    char messengers[MAX_ITEM][MAX_STR];

    int phone_count;
    int email_count;
    int social_count;
    int messenger_count;
} Contact;

typedef struct
{
    Contact items[MAX_CONTACTS];
    int count;
} Phonebook;

int pb_add(Phonebook *pb, const Contact *con);
int pb_remove(Phonebook *pb, int index);
int pb_get(const Phonebook *pb, int index, Contact *cont);

int cont_set_fields(Contact *cont, ...);
int cont_get_field(const Contact *cont, ContactField field, const char *value[]);

#endif // PHONEBOOK_H