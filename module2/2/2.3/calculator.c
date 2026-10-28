#include "calculator.h"

#include <stdlib.h>
#include <string.h>

int calc_register(Calculator *calc, const char *name, Operation operation)
{
    if (!calc || !name || !operation)
        return -1;

    if (calc->count == calc->capacity)
    {
        int capacity = calc->capacity ? calc->capacity * 2 : 4;
        void *commands = realloc(calc->commands, capacity * sizeof(*calc->commands));

        if (!commands)
            return -1;

        calc->commands = commands;
        calc->capacity = capacity;
    }

    char *copy = malloc(strlen(name) + 1);

    if (!copy)
        return -1;

    strcpy(copy, name);
    calc->commands[calc->count].name = copy;
    calc->commands[calc->count].function = operation;
    calc->count++;

    return 0;
}

const char *calc_name(const Calculator *calc, int index)
{
    if (!calc || index < 0 || index >= calc->count)
        return NULL;

    return calc->commands[index].name;
}

int calc_execute(const Calculator *calc, int index, double left, double right, double *result)
{
    if (!calc || index < 0 || index >= calc->count || !result)
        return -1;

    return calc->commands[index].function(left, right, result);
}

void calc_clear(Calculator *calc)
{
    if (!calc)
        return;

    for (int i = 0; i < calc->count; i++)
        free(calc->commands[i].name);

    free(calc->commands);
    *calc = (Calculator){0};
}