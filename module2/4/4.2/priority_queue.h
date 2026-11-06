#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "../4.1/linked_list.h"
#include "../../2/2.1/contact.h"

#include <stdint.h>

#define PRIORITY_MIN 0
#define PRIORITY_MAX 255

typedef struct
{
    uint8_t priority;
    const Contact *contact;
    char message[MAX_STR];
} ContactMessagePriority;

int pq_push(LinkedList *queue, const ContactMessagePriority *item);
int pq_pop_first(LinkedList *queue, ContactMessagePriority *item);
int pq_pop_priority(LinkedList *queue, unsigned int priority, ContactMessagePriority *item);
int pq_pop_at_least(LinkedList *queue, unsigned int min_priority,
                    ContactMessagePriority *item);

#endif
