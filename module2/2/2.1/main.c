#include "phonebook.h"
#include "storage.h"
#include "ui/ui.h"

#include <stdio.h>

int main()
{
    PhonebookArray contacts = {0};
    PhonebookAPI api;

    if (pb_bind(&api, &contacts) != 0)
    {
        return 1;
    }

    if (storage_load(&api, "phonebook.dat") != 0)
    {
        fprintf(stderr, "Cannot load a phonebook\n");
        return 1;
    }

    return ui_run(&api);
}