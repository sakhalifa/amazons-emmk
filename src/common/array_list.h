#ifndef __ARRAY_LIST_H__
#define __ARRAY_LIST_H__
#include <stddef.h>
#include <stdbool.h>

typedef struct array_list array_list_t;

/// @brief Inits an array_list
/// @param len initial length
/// @param free_func the function that is called when freeing an element of the list
/// @return the initialized array_list
struct array_list *array_list_init(size_t len, void (*free_func)(void *));

/// @brief Gets the length of an array list
/// @param list the array list
/// @return the length of the array list
size_t array_list_length(const array_list_t *list);

/// @brief Pushes an element at the end of the array_list
/// @param list the list to push the element to
/// @param x the element to push
void array_list_push(array_list_t *list, void *x);


/// @brief Removes an element at an index and returns it
/// @param list the list to remove the element from
/// @param idx the index of the element to remove
/// @return the removed element
void *array_list_remove(array_list_t *list, int idx);

/// @brief Gets an element from the list
/// @param list the list to get the element from
/// @param idx the index of the element
/// @return the element at the index specified in the list
void *array_list_get(const array_list_t *list, unsigned int idx);

/// @brief Assigns a value to an index in the list
/// @param list the list to assign the element
/// @param val the value to assign
/// @param idx the index where the value will be assigned
/// @return the overwritten element
void *array_list_set(array_list_t *list, void *val, unsigned int idx);

/// @brief Check if a list contains an element
/// @param l the list
/// @param val the value to check if it's inside the list
/// @param cmp_func a comparison function. Should return 0 iif 2 elements are equal.
/// @return true iif the list contains the value \p val 
bool array_list_contains(const array_list_t *l, void *val, int (*cmp_func)(void*, void*));

/// @brief Gets the index of a value if it's in the list
/// @param l the list
/// @param val the value to get the index
/// @param cmp_func a comparison function. Should return 0 iif 2 elements are equal.
/// @return -1 if the value is not in the list, else its index.
int array_list_get_index(const array_list_t *l, void *val, int (*cmp_func)(void*, void*));

/// @brief Frees the array_list and all its elements
/// @param array_list the list to free
void array_list_free(array_list_t *array_list);





#endif