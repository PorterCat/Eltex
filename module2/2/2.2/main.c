#include "operations.h"

#include <stdio.h>

int main(void)
{
    int choice;
    double left, right, result;
    int status;

    while (1)
    {
        printf("\n0. Exit\n"
               "1. Add\n"
               "2. Subtract\n"
               "3. Multiply\n"
               "4. Divide\n"
               "Choose operation: ");

        if (scanf("%d", &choice) != 1)
            return 1;

        if (choice == 0)
            break;

        if (choice < 1 || choice > 4)
        {
            printf("Invalid operation.\n");
            continue;
        }

        printf("Enter two numbers: ");

        if (scanf("%lf %lf", &left, &right) != 2)
            return 1;

        switch (choice)
        {
        case 1:
            status = calc_add(left, right, &result);
            break;
        case 2:
            status = calc_subtract(left, right, &result);
            break;
        case 3:
            status = calc_multiply(left, right, &result);
            break;
        case 4:
            status = calc_divide(left, right, &result);
            break;
        }

        if (status != 0)
        {
            printf("Calculation error.\n");
            continue;
        }

        printf("Result: %g\n", result);
    }

    return 0;
}