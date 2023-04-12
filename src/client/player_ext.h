#ifndef _PLAYER_CUSTOM_H_
#define _PLAYER_CUSTOM_H_

#include "player.h"
#include "position_set.h"
#include "board.h"

typedef struct
{
    unsigned int player_id;
    char *name;
    board_t *board;
} player_t;

/// @brief return all the possibles moves of the given queen
/// @param queen_position the position of the queen whose possibles moves are returned
/// @return all the possibles moves of the given queen
position_set* reachable_positions_deprecated(unsigned int queen_position);

#endif // _PLAYER_CUSTOM_H_