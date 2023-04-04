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

#endif // _PLAYER_CUSTOM_H_