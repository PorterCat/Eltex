#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <arpa/inet.h>

uint32_t random_ip(void)
{
    return ((uint32_t)(rand() & 255) << 24) | ((uint32_t)(rand() & 255) << 16) |
           ((uint32_t)(rand() & 255) << 8) | (uint32_t)(rand() & 255);
}

int is_local(uint32_t ip, uint32_t gateway, uint32_t mask)
{
    return (ip & mask) == (gateway & mask);
}

void print_packet(int number, uint32_t ip, int local)
{
    printf("%d. %u.%u.%u.%u -> %s\n", number, (ip >> 24) & 255, (ip >> 16) & 255, (ip >> 8) & 255,
           ip & 255, local ? "Own subnet" : "Via gateway");
}

void print_stats(int own, int other, int total)
{
    printf("\nStatistics:\n");
    printf("Own subnet: %d (%.2f%%)\n", own, own * 100.0 / total);
    printf("Other networks: %d (%.2f%%)\n", other, other * 100.0 / total);
}

int parse_mask(const char *text, uint32_t *mask)
{
    if (text[0] == '/')
    {
        char *end;
        long prefix = strtol(text + 1, &end, 10);

        if (text[1] < '0' || text[1] > '9' || *end != '\0' || prefix > 32)
            return -1;

        *mask = prefix == 0 ? 0 : UINT32_MAX << (32 - prefix);
        return 0;
    }

    struct in_addr address;

    if (inet_pton(AF_INET, text, &address) != 1)
        return -1;

    *mask = ntohl(address.s_addr);

    uint32_t inverse = ~*mask;

    if (inverse & (inverse + 1))
        return -1;

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <gateway> </prefix> <N>\n", argv[0]);
        return -1;
    }

    struct in_addr address;

    if (inet_pton(AF_INET, argv[1], &address) != 1)
    {
        printf("Invalid gateway address\n");
        return -1;
    }

    uint32_t mask;

    if (parse_mask(argv[2], &mask) == -1)
    {
        printf("Invalid subnet mask\n");
        return -1;
    }

    int n = atoi(argv[3]);

    if (n <= 0)
    {
        printf("Invalid packet count\n");
        return -1;
    }

    uint32_t gateway = ntohl(address.s_addr);

    int own = 0;
    int other = 0;

    srand(time(NULL));

    for (int i = 0; i < n; i++)
    {
        uint32_t ip = random_ip();
        int local = is_local(ip, gateway, mask);

        if (local)
            own++;
        else
            other++;

        print_packet(i + 1, ip, local);
    }

    print_stats(own, other, n);

    return 0;
}