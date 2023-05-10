#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__
#include <stdlib.h>
#include <stdbool.h>
typedef struct linked_list linked_list_t;
typedef struct linked_list_cell linked_list_cell_t;

/// @brief Allocates an empty linked_list
/// @param free_func the function used to free all of its elements
/// @return An empty linked list
linked_list_t *linked_list_create(void (*free_func)(void *));

/// @brief Frees the linked_list and all its elements
/// @param l the list to free
void linked_list_free(linked_list_t *l);

/// @brief Gets the first element of the linked list
/// @param l the linked list
/// @return the first element of the linked list
void *linked_list_get_first(linked_list_t *l);

/// @brief Gets the last element of the linked list
/// @param l the linked list
/// @return the last element of the linked list
void *linked_list_get_last(linked_list_t *l);

/// @brief Gets the length of the linked list
/// @param l the linked list
/// @return the length of the linked list
size_t linked_list_length(linked_list_t *l);

/// @brief Removes the last element of the linked list and frees it
/// @param l the linked list
/// @return true iif the last element was removed
bool linked_list_remove_last(linked_list_t *l);

/// @brief Removes the first element of the linked list and frees it
/// @param l the linked list
/// @return true iif the first element was removed
bool linked_list_remove_first(linked_list_t *l);

/// @brief Appends an element to the end of the list
/// @param l the list
/// @param val the value to push
void linked_list_push(linked_list_t *l, void *val);

/// @brief Adds an element at the start of the list
/// @param l the list
/// @param val the element to enqueue
void linked_list_queue(linked_list_t *l, void *val);

void *linked_list_cell_get_data(linked_list_cell_t* c);

linked_list_cell_t *linked_list_first_cell(linked_list_t *l);

linked_list_cell_t *linked_list_last_cell(linked_list_t *l);

linked_list_cell_t *linked_list_cell_next(linked_list_cell_t *c);

linked_list_cell_t *linked_list_cell_prev(linked_list_cell_t *c);

#endif