#ifndef _PLAYER_CUSTOM_H_
#define _PLAYER_CUSTOM_H_

#include "player.h"

struct position_set_s;

typedef struct position_set_s position_set;

/// @brief return an empty position_set (position_count set on 0)
/// @return a position_set with position_count set on 0
position_set* init_position_set();

/// @brief add the given position to the given set
/// @param set the set in which the position is added
/// @param position the position that is added to the set
void add_position(position_set* set, unsigned int position);

/// @brief return all the possibles moves of the given queen
/// @param queen_position the position of the queen whose possibles moves are returned
/// @return all the possibles moves of the given queen
position_set* possible_moves(unsigned int queen_position);

/// @brief Add to the position_set "possible_moves" all the reachable positions from initial_position in the direction direction_to_neighbor.
/// @param possible_moves the position_set where the possible moves found by this function are added
/// @param initial_position the id of the position from where we try to find reachable positions in the given direction
/// @param neighbor the id of the first reachable neighbor reached in the given direction
void add_possible_moves_aligned(position_set* possible_moves, unsigned int initial_position, unsigned int neighbor, enum dir_t direction_to_neighbor);

#endif // _PLAYER_CUSTOM_H_