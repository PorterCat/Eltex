int calc_add(double left, double right, double *result)
{
    *result = left + right;
    return 0;
}

int calc_subtract(double left, double right, double *result)
{
    *result = left - right;
    return 0;
}

int calc_multiply(double left, double right, double *result)
{
    *result = left * right;
    return 0;
}

int calc_divide(double left, double right, double *result)
{
    if (right == 0)
        return -1;

    *result = left / right;
    return 0;
}