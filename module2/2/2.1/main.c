#include "phonebook.h"
#include "storage.h"
#include "ui.h"

#include <locale.h>
#include <stdio.h>

int main(void)
{
    setlocale(LC_ALL, "");

    Phonebook pb = {0};

    if (storage_load(&pb, "phonebook.dat") != 0)
    {
        fprintf(stderr, "Cannot load a phonebook\n");
        return 1;
    }

    return ui_run(&pb);
}