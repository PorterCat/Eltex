#ifndef STORAGE_H
#define STORAGE_H

#include "phonebook_api.h"

int storage_load(PhonebookAPI *pb, const char *path);
int storage_save(const PhonebookAPI *pb, const char *path);

#endif