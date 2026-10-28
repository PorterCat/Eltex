#ifndef STORAGE_H
#define STORAGE_H

#include "phonebook.h"

int storage_load(Phonebook *pb, const char *path);
int storage_save(const Phonebook *pb, const char *path);

#endif // STORAGE_H