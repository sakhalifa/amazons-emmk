#ifndef __TREE_H__
#define __TREE_H__
#include "util.h"
#include "array_list.h"

/**
 * A node structure.
 * Every child of a node must have the same free_func as its parent.
 */
struct node;

typedef struct node node_t;

/// @brief Creates a tree with a root value and a function to free all of its elements and returns its root
/// @param val the value
/// @param free_func the function
/// @return the root
node_t *tree_create(void *val, void (*free_func)(void *));

/// @brief Adds a child
/// @param node the node
/// @param val the value
/// @return the newly added node
node_t *node_add_child(node_t *node, void *val);

/// @brief  Removes the first occurence of a node that has a certain value from the tree and returns it.
///         If there were no child to remove, it will not remove it
/// @param node the node
/// @param val the value
/// @param cmp_func the function
/// @return the node
node_t *node_remove_child(node_t *node, void *val, int (*cmp_func)(void *, void *));

void *node_get_value(node_t *node);

node_t *node_get_parent(node_t *node);

array_list_t *node_get_children(node_t *node);

node_t *node_get_child(node_t *node, void *val, int (*cmp_func)(void *, void *));

/// @brief Gets a child with a certain value from a tree and returns it
/// @param root the root
/// @param val the value
/// @param cmp_func the function
/// @return the child or NULL if not found
node_t *tree_get_node(const node_t *root, void *val, int (*cmp_func)(void *, void *));

/// @brief Detaches a node from its containing tree, making it the root of a new tree.
/// @param node the node to detach
void detach_node(node_t *node);

/// @brief Get the root of the tree that contains the node
/// @param node the node
/// @return the root
node_t *node_get_root(const node_t *node);

/// @brief check if a node has a child with a certain value
/// @param node the node
/// @param val the value
/// @param cmp_func the function
bool node_has_child(node_t *node, void *val, int (*cmp_func)(void *, void *));

/// @brief check if a node has a parent with a certain value
/// @param node the node
/// @param val the value
/// @param cmp_func the function
bool node_has_parent(node_t *node, void *val, int (*cmp_func)(void *, void *));

/// @brief prints the tree
void tree_print(node_t *root, void (*printValue)(void *));

/// @brief Frees a whole tree
void tree_free(node_t *root);

#endif