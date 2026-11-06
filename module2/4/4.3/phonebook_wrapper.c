#include "phonebook_wrapper.h"

TreeNode *inorder_next(TreeNode *node)
{
    if (node->right != NULL)
    {
        node = node->right;

        while (node->left != NULL)
            node = node->left;

        return node;
    }

    TreeNode *parent = node->parent;

    while (parent != NULL && node == parent->right)
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

TreeNode *inorder_prev(TreeNode *node)
{
    if (node->left != NULL)
    {
        node = node->left;

        while (node->right != NULL)
            node = node->right;

        return node;
    }

    TreeNode *parent = node->parent;

    while (parent != NULL && node == parent->left)
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

int api_count(const void *context)
{
    const BinaryTree *tree = context;

    return tree != NULL ? tree->count : -1;
}

PhonebookIterator api_begin(const void *context)
{
    const BinaryTree *tree = context;
    TreeNode *node;

    if (tree == NULL)
        return NULL;

    node = tree->root;

    while (node != NULL && node->left != NULL)
        node = node->left;

    return node;
}

PhonebookIterator api_next(const void *context, PhonebookIterator iterator)
{
    (void)context;

    return iterator != NULL ? inorder_next((TreeNode *)iterator) : NULL;
}

PhonebookIterator api_prev(const void *context, PhonebookIterator iterator)
{
    (void)context;

    return iterator != NULL ? inorder_prev((TreeNode *)iterator) : NULL;
}

Contact *api_get(void *context, PhonebookIterator iterator)
{
    (void)context;

    TreeNode *node = (TreeNode *)iterator;

    return node != NULL ? node->data : NULL;
}

int api_add(
    void *context,
    const Contact *contact,
    PhonebookIterator *inserted
)
{
    BinaryTree *tree = context;

    if (inserted != NULL)
        *inserted = NULL;

    if (tree == NULL || contact == NULL)
        return -1;

    TreeNode *node = tree_insert(tree, contact);

    if (node == NULL)
        return -1;

    if (inserted != NULL)
        *inserted = node;

    return 0;
}

int api_remove(
    void *context,
    PhonebookIterator iterator,
    PhonebookIterator *next
)
{
    BinaryTree *tree = context;
    TreeNode *node = (TreeNode *)iterator;

    if (next != NULL)
        *next = NULL;

    if (tree == NULL || node == NULL)
        return -1;

    TreeNode *position = inorder_next(node);

    if (position == NULL)
        position = inorder_prev(node);

    if (tree_remove(tree, node) != 0)
        return -1;

    if (next != NULL)
        *next = position;

    return 0;
}

int api_update(
    void *context,
    PhonebookIterator iterator,
    PhonebookIterator *updated
)
{
    BinaryTree *tree = context;
    TreeNode *node = (TreeNode *)iterator;

    if (updated != NULL)
        *updated = NULL;

    if (tree == NULL || node == NULL)
        return -1;

    if (tree_reposition(tree, node) != 0)
        return -1;

    if (updated != NULL)
        *updated = node;

    return 0;
}

void api_clear(void *context)
{
    tree_clear(context);
}

int pb_bind(PhonebookAPI *api, BinaryTree *tree)
{
    if (api == NULL || tree == NULL ||
        tree->data_size != sizeof(Contact) ||
        tree->compare == NULL)
        return -1;

    *api = (PhonebookAPI){
        .context = tree,
        .count = api_count,
        .begin = api_begin,
        .next = api_next,
        .prev = api_prev,
        .get = api_get,
        .add = api_add,
        .remove = api_remove,
        .update = api_update,
        .clear = api_clear
    };

    return 0;
}