#include "binary_tree.h"
#include "phonebook_wrapper.h"
#include "../../2/2.1/storage.h"
#include "../../2/2.1/ui/ui.h"

#include <stdio.h>

#include "binary_tree.h"

#include <stdio.h>

int compare_int(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;

    return (a > b) - (a < b);
}

void print_tree(const TreeNode *node, int depth, char branch)
{
    if (node == NULL)
        return;

    for (int i = 0; i < depth; i++)
        printf("\t");

    if (depth > 0)
        printf("%c ", branch);

    printf("%d\n", *(const int *)node->data);

    print_tree(node->left, depth + 1, '/');
    print_tree(node->right, depth + 1, '\\');
}

int main(void)
{
    BinaryTree tree;

    int values[] = {1, 2, 3, 4, 5, 6};
    int count = sizeof(values) / sizeof(values[0]);

    if (tree_init(&tree, sizeof(int), compare_int) != 0)
        return -1;

    for (int i = 0; i < count; i++)
    {
        if (tree_insert(&tree, &values[i]) == NULL)
        {
            tree_destroy(&tree);
            return -1;
        }
    }

    printf("Binary Search Tree:\n\n");
    print_tree(tree.root, 0, '\0');

    tree_destroy(&tree);

    return 0;
}

// int compare_contacts(const void *left, const void *right)
// {
//     const Contact *a = left;
//     const Contact *b = right;

//     int result = strcmp(a->last_name, b->last_name);

//     if (result != 0)
//         return result;

//     return strcmp(a->first_name, b->first_name);
// }

// int main()
// {
//     BinaryTree contacts = {0};
//     PhonebookAPI api;

//     if (tree_init(&contacts, sizeof(Contact), compare_contacts) != 0)
//         return 1;

//     if (pb_bind(&api, &contacts) != 0)
//     {
//         tree_destroy(&contacts);
//         return 1;
//     }

//     if (storage_load(&api, "phonebook.dat") != 0)
//     {
//         fprintf(stderr, "Cannot load a phonebook\n");
//         tree_destroy(&contacts);
//         return 1;
//     }

//     int result = ui_run(&api);

//     tree_destroy(&contacts);

//     return result;
// }