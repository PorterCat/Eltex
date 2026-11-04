#include "phonebook_wrapper.h"
#include "../../2/2.1/storage.h"
#include "../../2/2.1/ui.h"

#include <stdio.h>

int main()
{
    LinkedList contacts = {0};
    PhonebookAPI api;

    if (list_init(&contacts, sizeof(Contact)) != 0)
        return 1;

    if (pb_bind(&api, &contacts) != 0)
    {
        list_destroy(&contacts);
        return 1;
    }

    if (storage_load(&api, "phonebook.dat") != 0)
    {
        fprintf(stderr, "Cannot load a phonebook\n");
        list_destroy(&contacts);
        return 1;
    }

    int result = ui_run(&api);

    list_destroy(&contacts);

    return result;
}