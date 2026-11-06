#include "priority_queue.h"

int pq_push(LinkedList *queue, const ContactMessagePriority *item)
{
    ListNode *position = queue->head;
    while (position != NULL)
    {
        const ContactMessagePriority *current = position->data;
        if (current->priority < item->priority)
            break;
        position = position->next;
    }

    return list_insert(queue, position, item) != NULL ? 0 : -1;
}

static int remove_item(LinkedList *queue, ListNode *node, ContactMessagePriority *item)
{
    *item = *(ContactMessagePriority *)node->data;
    return list_remove(queue, node);
}

int pq_pop_first(LinkedList *queue, ContactMessagePriority *item)
{
    return remove_item(queue, queue->head, item);
}

int pq_pop_priority(LinkedList *queue, unsigned int priority, ContactMessagePriority *item)
{
    for (ListNode *node = queue->head; node != NULL; node = node->next)
    {
        const ContactMessagePriority *current = node->data;
        if (current->priority < priority)
            break;
        if (current->priority == priority)
            return remove_item(queue, node, item);
    }

    return -1;
}

int pq_pop_at_least(LinkedList *queue, unsigned int min_priority,
                    ContactMessagePriority *item)
{
    ListNode *node = queue->head;
    if (node == NULL || ((ContactMessagePriority *)node->data)->priority < min_priority)
        return -1;

    return remove_item(queue, node, item);
}
