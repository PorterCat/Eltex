#include "calculator.h"
#include "operations.h"

#include <stdio.h>

int main(void)
{
    Calculator calc = {0};

    calc_register(&calc, "Add", calc_add);
    calc_register(&calc, "Subtract", calc_subtract);
    calc_register(&calc, "Multiply", calc_multiply);
    calc_register(&calc, "Divide", calc_divide);

    int choice;
    double left, right, result;

    while (1)
    {
        printf("\n0. Exit\n");

        for (int i = 0; i < calc.count; i++)
            printf("%d. %s\n", i + 1, calc_name(&calc, i));

        printf("Choose operation: ");

        if (scanf("%d", &choice) != 1)
            break;

        if (choice == 0)
            break;

        if (choice < 1 || choice > calc.count)
        {
            printf("Invalid operation.\n");
            continue;
        }

        printf("Enter two numbers: ");

        if (scanf("%lf %lf", &left, &right) != 2)
            break;

        if (calc_execute(&calc, choice - 1, left, right, &result) != 0)
        {
            printf("Calculation error.\n");
            continue;
        }

        printf("Result: %g\n", result);
    }

    calc_clear(&calc);

    return 0;
}