#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stddef.h>

typedef int (*CompareFunc)(const void *left, const void *right);

typedef struct TreeNode
{
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
    int height;
    void *data;
} TreeNode;

typedef struct
{
    TreeNode *root;
    int count;
    size_t data_size;
    CompareFunc compare;
} BinaryTree;

int tree_init(BinaryTree *tree, size_t data_size, CompareFunc compare);

TreeNode *tree_insert(BinaryTree *tree, const void *data);
int tree_reposition(BinaryTree *tree, TreeNode *node);
int tree_remove(BinaryTree *tree, TreeNode *node);

void tree_clear(BinaryTree *tree);
void tree_destroy(BinaryTree *tree);

// AVL
int tree_balance_factor(const TreeNode *node);
void tree_update_height(TreeNode *node);
TreeNode *tree_rotate_left(BinaryTree *tree, TreeNode *node);
TreeNode *tree_rotate_right(BinaryTree *tree, TreeNode *node);
void tree_rebalance(BinaryTree *tree, TreeNode *node);

#endif