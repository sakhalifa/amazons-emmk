#ifndef __TREE_H__
#define __TREE_H__
#include "util.h"
#include "array_list.h"

// TODO : Hide implementation!!!!
/**
 * A node structure.
 * Every child of a node must have the same free_func as its parent.
*/
typedef struct node{
    void *value;
    void (*free_func)(void*);
    struct node* parent;
    array_list_t *children;
} node_t;

/**
 * Creates a tree with a root value and a function to free all of its elements and returns its root.
*/
node_t *tree_create(void *val, void (*free_func)(void*));

/**
 * Adds a child with the value val to a node and returns the newly added node.
*/
node_t *node_add_child(node_t* node, void *val);

/**
 * Removes the first occurence of a node that has a certain value from the tree and returns it.
 * If there were no child to remove, it will not remove it
*/
node_t *node_remove_child(node_t *node, void* val, int (*cmp_func)(void*, void*));

/**
 * Gets a child with a certain value from a tree and returns it. If it is not found,
 * returns NULL.
*/
node_t *tree_get_node(const node_t *root, void* val, int (*cmp_func)(void*, void*));

/**
 * Returns the root of the tree that contains the node
*/
node_t *node_get_root(const node_t* node);

/**
 * If a node has a child with a certain value
*/
bool node_has_child(node_t *node, void* val, int (*cmp_func)(void*, void*));
/**
 * If a node has a parent with a certain value
*/
bool node_has_parent(node_t *node, void* val, int (*cmp_func)(void*, void*));

/**
 * Prints the tree
*/
void tree_print(node_t* root, void (*printValue)(void*));

/**
 * Frees a whole tree.
*/
void node_free(node_t* root);

#endif