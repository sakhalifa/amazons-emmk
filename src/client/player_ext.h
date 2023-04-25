#ifndef _PLAYER_CUSTOM_H_
#define _PLAYER_CUSTOM_H_

#include "player.h"
#include "board.h"

typedef struct
{
    unsigned int player_id;
    char *name;
    board_t *board;
} player_t;

void generic_initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS]);

#endif // _PLAYER_CUSTOM_H_