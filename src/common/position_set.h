#ifndef _POSITION_SET_H_
#define _POSITION_SET_H_

#include <stdlib.h>

typedef struct
{
    unsigned int *positions;
    size_t count;
} position_set;

/// @brief initialize all the positions of the game
/// @param capacity the number of maximm positions
/// @return all the positions
position_set *init_position_set(size_t capacity);

/// @brief Add a position
/// @param set the array of positions
/// @param position the position
void add_position(position_set *set, unsigned int position);

/// @brief free all the positions
/// @param position_set the positions
void free_position_set(position_set *position_set);

/// @brief print all the positions
/// @param set the positions
void print_position_set(position_set *set);

/// @brief remove a position
/// @param set all the positions
/// @param position_index the index of the position removed
void remove_position_at_index(position_set *set, size_t position_index);

#endif // _POSITION_SET_H_