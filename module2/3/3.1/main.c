#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int parse_numeric(const char *str, mode_t *mode)
{
    unsigned value = 0;

    for (int i = 0; i < 3; i++)
    {
        if (!str[i] || str[i] < '0' || str[i] > '7')
            return -1;

        value = (value << 3) | (str[i] - '0');
    }

    *mode = value;
    return 0;
}

int parse_symbolic(const char *str, mode_t *mode)
{
    const char *chars = "rwxrwxrwx";
    *mode = 0;

    for (int i = 0; i < 9; i++)
    {
        if (!str[i] || (str[i] != '-' && str[i] != chars[i]))
            return -1;

        *mode = (*mode << 1) | (str[i] == chars[i]);
    }

    return 0;
}

int apply_change(mode_t *mode, const char *command)
{
    int user = 0, group = 0, others = 0;
    int read = 0, write = 0, execute = 0;

    while (*command == 'u' || *command == 'g' || *command == 'o' || *command == 'a')
    {
        switch (*command++)
        {
        case 'u':
            user = 1;
            break;
        case 'g':
            group = 1;
            break;
        case 'o':
            others = 1;
            break;
        case 'a':
            user = group = others = 1;
            break;
        }
    }

    char op = *command;

    if ((!user && !group && !others) || (op != '+' && op != '-' && op != '='))
        return -1;

    command++;

    while (*command)
    {
        switch (*command++)
        {
        case 'r':
            read = 1;
            break;
        case 'w':
            write = 1;
            break;
        case 'x':
            execute = 1;
            break;
        default:
            return -1;
        }
    }

    if (!read && !write && !execute && op != '=')
        return -1;

    mode_t bits = (read << 2) | (write << 1) | execute;
    mode_t mask = 0, permissions = 0;

    if (user)
    {
        mask |= 0700;
        permissions |= bits << 6;
    }

    if (group)
    {
        mask |= 0070;
        permissions |= bits << 3;
    }

    if (others)
    {
        mask |= 0007;
        permissions |= bits;
    }

    if (op == '+')
        *mode |= permissions;
    else if (op == '-')
        *mode &= ~permissions;
    else
        *mode = (*mode & ~mask) | permissions;

    return 0;
}

void format_mode(mode_t mode, char *symbolic, char *bits)
{
    int bit_position = 0;
    mode_t bit = 0400;

    for (int i = 0; i < 9; i++)
    {
        int enabled = (mode & bit) != 0;

        symbolic[i] = enabled ? "rwx"[i % 3] : '-';
        bits[bit_position++] = enabled ? '1' : '0';

        if ((i + 1) % 3 == 0 && i != 8)
            bits[bit_position++] = ' ';

        bit >>= 1;
    }

    symbolic[9] = '\0';
    bits[bit_position] = '\0';
}

void print_mode(mode_t mode)
{
    char symbolic[10], bits[12];

    format_mode(mode, symbolic, bits);

    printf("Symbolic: %s\n", symbolic);
    printf("Numeric:  %03o\n", (unsigned)mode);
    printf("Bits:     %s\n", bits);
}

void print_change(mode_t before, mode_t after)
{
    char old_symbolic[10], new_symbolic[10];
    char old_bits[12], new_bits[12];

    format_mode(before, old_symbolic, old_bits);
    format_mode(after, new_symbolic, new_bits);

    printf("Symbolic: %s -> %s\n", old_symbolic, new_symbolic);
    printf("Numeric:  %03o -> %03o\n", (unsigned)before, (unsigned)after);
    printf("Bits:     %s -> %s\n", old_bits, new_bits);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <mode> [changes...]\n", argv[0]);
        printf("       %s -f <file> [changes...]\n", argv[0]);
        return 1;
    }

    mode_t before;
    int first_change;

    if (strcmp(argv[1], "-f") == 0)
    {
        struct stat info;

        if (argc < 3 || stat(argv[2], &info) == -1)
        {
            printf("Cannot access file\n");
            return 1;
        }

        before = info.st_mode & 0777;
        first_change = 3;
    }
    else
    {
        if (argv[1][0] >= '0' && argv[1][0] <= '9')
        {
            if (parse_numeric(argv[1], &before) == -1)
            {
                printf("Invalid permissions\n");
                return 1;
            }
        }
        else
        {
            if (parse_symbolic(argv[1], &before) == -1)
            {
                printf("Invalid permissions\n");
                return 1;
            }
        }
        first_change = 2;
    }

    mode_t after = before;

    for (int i = first_change; i < argc; i++)
    {
        if (apply_change(&after, argv[i]) == -1)
        {
            printf("Invalid modification: %s\n", argv[i]);
            return 1;
        }
    }

    if (first_change == argc)
        print_mode(before);
    else
        print_change(before, after);

    return 0;
}
