#include "priority_queue.h"

#include <stddef.h>
#include <stdio.h>

static int add_contact_message(LinkedList *queue, unsigned int priority,
                               const Contact *contact, const char *message)
{
    if (priority > PRIORITY_MAX)
        return -1;

    ContactMessagePriority item = {0};
    item.priority = (uint8_t)priority;
    item.contact = contact;
    snprintf(item.message, sizeof(item.message), "%s", message);
    return pq_push(queue, &item);
}

static void print_contact_message(const char *operation,
                                  const ContactMessagePriority *item)
{
    const char *first_name;
    const char *last_name;

    cont_get_field(item->contact, FIELD_FIRST_NAME, &first_name);
    cont_get_field(item->contact, FIELD_LAST_NAME, &last_name);

    printf("%s (priority %u): %s %s — %s\n", operation, item->priority,
           first_name, last_name, item->message);
}

int main(void)
{
    LinkedList queue;
    ContactMessagePriority item;
    Contact anna = {0};
    Contact ivan = {0};
    Contact oleg = {0};

    list_init(&queue, sizeof(ContactMessagePriority));

    cont_init(&anna, "Смирнова", "Анна", FIELD_PHONE(0), "12312412");
    cont_init(&ivan, "Петров", "Иван", FIELD_PHONE(0), "8877989");
    cont_init(&oleg, "Иванов", "Олег", FIELD_EMAIL(0), "oleg@example.com");

    add_contact_message(&queue, 30, &ivan, "добавлен новый контакт");
    add_contact_message(&queue, 70, &anna, "добавлен новый контакт");

    cont_edit(&anna, FIELD_PHONE(0), "99999999");
    add_contact_message(&queue, 220, &anna, "срочно изменен номер телефона");
    add_contact_message(&queue, 100, &oleg, "обновлен адрес электронной почты");

    cont_edit(&ivan, FIELD_MIDDLE_NAME, "Алексеевич");
    add_contact_message(&queue, 5, &ivan, "добавлено отчество");

    cont_edit(&anna, FIELD_LAST_NAME, "Соколова");
    add_contact_message(&queue, 220, &anna, "исправлена фамилия");

    printf("В очереди: %zu сообщений\n\n", (size_t)queue.count);

    printf("Извлечение первого сообщения (наивысший приоритет):\n");
    pq_pop_first(&queue, &item);
    print_contact_message("Извлечено", &item);

    printf("\nИзвлечение сообщения с точным приоритетом 100:\n");
    pq_pop_priority(&queue, 100, &item);
    print_contact_message("Извлечено", &item);

    printf("\nИзвлечение первого сообщения с приоритетом не ниже 200:\n");
    pq_pop_at_least(&queue, 200, &item);
    print_contact_message("Извлечено", &item);

    printf("\nОставшиеся сообщения:\n");
    while (queue.count > 0)
    {
        pq_pop_first(&queue, &item);
        print_contact_message("Извлечено", &item);
    }

    list_destroy(&queue);
    return 0;
}
