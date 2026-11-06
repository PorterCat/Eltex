#include "binary_tree.h"

#include <stdlib.h>
#include <string.h>

void tree_replace_node(BinaryTree *tree, TreeNode *old_node, TreeNode *new_node)
{
    if (old_node->parent == NULL)
        tree->root = new_node;
    else if (old_node == old_node->parent->left)
        old_node->parent->left = new_node;
    else
        old_node->parent->right = new_node;

    if (new_node != NULL)
        new_node->parent = old_node->parent;
}

TreeNode *tree_detach_node(BinaryTree *tree, TreeNode *node)
{
    TreeNode *balance = NULL;

    if (node->left == NULL)
    {
        balance = node->parent;
        tree_replace_node(tree, node, node->right);
    }
    else if (node->right == NULL)
    {
        balance = node->parent;
        tree_replace_node(tree, node, node->left);
    }
    else
    {
        TreeNode *successor = node->right;

        while (successor->left != NULL)
            successor = successor->left;

        if (successor->parent != node)
        {
            balance = successor->parent;

            tree_replace_node(tree, successor, successor->right);

            successor->right = node->right;
            successor->right->parent = successor;
        }
        else
        {
            balance = successor;
        }

        tree_replace_node(tree, node, successor);

        successor->left = node->left;
        successor->left->parent = successor;
    }

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return balance;
}

void tree_link_node(BinaryTree *tree, TreeNode *node)
{
    TreeNode *parent = NULL;
    TreeNode *current = tree->root;

    while (current != NULL)
    {
        parent = current;

        if (tree->compare(node->data, current->data) < 0)
            current = current->left;
        else
            current = current->right;
    }

    node->parent = parent;
    node->height = 1;

    if (parent == NULL)
        tree->root = node;
    else if (tree->compare(node->data, parent->data) < 0)
        parent->left = node;
    else
        parent->right = node;
}

int tree_contains_node(const BinaryTree *tree, const TreeNode *node)
{
    if (tree == NULL || node == NULL)
        return 0;

    while (node->parent != NULL)
        node = node->parent;

    return node == tree->root;
}

int tree_init(BinaryTree *tree, size_t data_size, CompareFunc compare)
{
    if (tree == NULL || data_size == 0 || compare == NULL)
        return -1;

    tree->root = NULL;
    tree->count = 0;
    tree->data_size = data_size;
    tree->compare = compare;

    return 0;
}

TreeNode *tree_insert(BinaryTree *tree, const void *data)
{
    if (tree == NULL || data == NULL || tree->data_size == 0)
        return NULL;

    TreeNode *node = malloc(sizeof(*node));

    if (node == NULL)
        return NULL;

    node->data = malloc(tree->data_size);

    if (node->data == NULL)
    {
        free(node);
        return NULL;
    }

    memcpy(node->data, data, tree->data_size);

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    tree_link_node(tree, node);
    tree->count++;

    tree_rebalance(tree, node);

    return node;
}

int tree_reposition(BinaryTree *tree, TreeNode *node)
{
    if (!tree_contains_node(tree, node))
        return -1;

    TreeNode *balance = tree_detach_node(tree, node);

    tree_rebalance(tree, balance);

    tree_link_node(tree, node);
    tree_rebalance(tree, node);

    return 0;
}

int tree_remove(BinaryTree *tree, TreeNode *node)
{
    if (!tree_contains_node(tree, node))
        return -1;

    TreeNode *balance = tree_detach_node(tree, node);

    free(node->data);
    free(node);

    tree->count--;

    tree_rebalance(tree, balance);

    return 0;
}

void tree_clear(BinaryTree *tree)
{
    if (tree == NULL)
        return;

    TreeNode *node = tree->root;

    while (node != NULL)
    {
        if (node->left != NULL)
            node = node->left;
        else if (node->right != NULL)
            node = node->right;
        else
        {
            TreeNode *parent = node->parent;

            if (parent == NULL)
                tree->root = NULL;
            else if (parent->left == node)
                parent->left = NULL;
            else
                parent->right = NULL;

            free(node->data);
            free(node);

            node = parent;
        }
    }

    tree->count = 0;
}

void tree_destroy(BinaryTree *tree)
{
    if (tree == NULL)
        return;

    tree_clear(tree);

    tree->data_size = 0;
    tree->compare = NULL;
}

void tree_update_height(TreeNode *node)
{
    if (node == NULL)
        return;

    int left = node->left != NULL ? node->left->height : 0;
    int right = node->right != NULL ? node->right->height : 0;

    node->height = (left > right ? left : right) + 1;
}

int tree_balance_factor(const TreeNode *node)
{
    if (node == NULL)
        return 0;

    int left = node->left != NULL ? node->left->height : 0;
    int right = node->right != NULL ? node->right->height : 0;

    return right - left;
}

TreeNode *tree_rotate_left(BinaryTree *tree, TreeNode *node)
{
    TreeNode *right = node->right;

    node->right = right->left;

    if (node->right != NULL)
        node->right->parent = node;

    tree_replace_node(tree, node, right);

    right->left = node;
    node->parent = right;

    tree_update_height(node);
    tree_update_height(right);

    return right;
}

TreeNode *tree_rotate_right(BinaryTree *tree, TreeNode *node)
{
    TreeNode *left = node->left;

    node->left = left->right;

    if (node->left != NULL)
        node->left->parent = node;

    tree_replace_node(tree, node, left);

    left->right = node;
    node->parent = left;

    tree_update_height(node);
    tree_update_height(left);

    return left;
}

void tree_rebalance(BinaryTree *tree, TreeNode *node)
{
    while (node != NULL)
    {
        tree_update_height(node);

        int balance = tree_balance_factor(node);

        if (balance > 1)
        {
            if (tree_balance_factor(node->right) < 0)
                tree_rotate_right(tree, node->right);

            node = tree_rotate_left(tree, node);
        }
        else if (balance < -1)
        {
            if (tree_balance_factor(node->left) > 0)
                tree_rotate_left(tree, node->left);

            node = tree_rotate_right(tree, node);
        }

        node = node->parent;
    }
}