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


#endif