#include "tree.h"
#include <stdio.h>
#include <assert.h>
#include "util.h"
struct node
{
    void *value;
    void (*free_func)(void *);
    struct node *parent;
    array_list_t *children;
};

void vs_node_free(void *vn)
{
    node_free((node_t *)vn);
}

node_t *tree_create(void *val, void (*free_func)(void *))
{
    node_t *n = malloc(sizeof(node_t));
    CHECK_MALLOC(n);
    n->children = array_list_init(0, vs_node_free);
    n->parent = NULL;
    n->value = val;
    n->free_func = free_func;

    return n;
}

node_t *node_add_child(node_t *node, void *val)
{
    node_t *n = tree_create(val, node->free_func);
    n->parent = node;
    array_list_push(node->children, n);
    return n;
}

bool node_has_child(node_t *node, void *val, int (*cmp_func)(void *, void *))
{
    for (uint i = 0; i < array_list_length(node->children); i++)
    {
        node_t *child = array_list_get(node->children, i);
        if (!cmp_func(val, child->value))
        {
            return true;
        }
    }
    return false;
}

node_t *node_remove_child(node_t *node, void *val, int (*cmp_func)(void *, void *))
{
    for (uint i = 0; i < array_list_length(node->children); i++)
    {
        node_t *child = array_list_get(node->children, i);
        if (!cmp_func(val, child->value))
        {
            array_list_remove(node->children, i);
            return child;
        }
    }
    return NULL;
}

node_t *node_get_root(const node_t *node)
{
    while (node->parent != NULL)
    {
        node = node->parent;
    }
    return (node_t *)node;
}

node_t *tree_get_node(const node_t *root, void *val, int (*cmp_func)(void *, void *))
{
    if (root == NULL)
    {
        return NULL;
    }
    if (!cmp_func(val, root->value))
    {
        return (node_t *)root;
    }
    for (uint i = 0; i < array_list_length(root->children); i++)
    {
        node_t *n = tree_get_node(array_list_get(root->children, i), val, cmp_func);
        if (n != NULL)
        {
            return n;
        }
    }
    return NULL;
}

bool node_has_parent(node_t *node, void *val, int (*cmp_func)(void *, void *))
{
    if (node == NULL)
    {
        return false;
    }
    if (!cmp_func(val, node->value))
    {
        return true;
    }
    return node_has_parent(node->parent, val, cmp_func);
}

void node_free(node_t *root)
{
    if (root == NULL)
    {
        return;
    }
    array_list_free(root->children);
    root->free_func(root->value);
    free(root);
}

void node_print(node_t *node, unsigned int indents, void (*print_value)(void *))
{
    for (int i = 0; i < (int)indents; i++)
    {
        printf("    ");
    }
    print_value(node->value);
    printf("\n");
    for (uint i = 0; i < array_list_length(node->children); i++)
    {
        node_print(array_list_get(node->children, i), indents + 1, print_value);
    }
}

void tree_print(node_t *root, void (*print_value)(void *))
{
    node_print(root, 0, print_value);
}