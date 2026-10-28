#ifndef CALCULATOR_H
#define CALCULATOR_H

typedef int (*Operation)(double left, double right, double *result);

typedef struct
{
    struct
    {
        char *name;
        Operation function;
    } *commands;

    int count;
    int capacity;
} Calculator;

int calc_register(Calculator *calc, const char *name, Operation operation);
const char *calc_name(const Calculator *calc, int index);
int calc_execute(const Calculator *calc, int index, double left, double right, double *result);
void calc_clear(Calculator *calc);

#endif