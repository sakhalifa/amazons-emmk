#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>

#include "graph.h"
#include "player.h"
#include "move.h"
#include "dir.h"

typedef struct {
    struct graph_t graph;
    unsigned int *queens[NUM_PLAYERS];
    unsigned int num_queens;
} board_t;


bool is_move_legal(struct move_t move);

/// @returns a struct containing the neighbors list and the amount of neighbors
struct neighbors_t neighbors(unsigned int cell);

/// @returns the neighbor cell in the given direction, MAX_INT if it doesn't exist
unsigned int neighbor(unsigned int cell, enum dir_t direction);


#endif