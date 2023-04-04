#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>

#include "dir.h"
#include "graph.h"
#include "move.h"
#include "player.h"

typedef struct {
  struct graph_t *graph;
  unsigned int **queens;
  unsigned int num_queens;
  bool *arrows; // 1-d maping of the positions of the arrows
} board_t;

board_t *init_board(struct graph_t *graph, unsigned int num_queens,
                    unsigned int *queens[NUM_PLAYERS]);

/// @brief return true if the given position is indexing a valid position on the
/// board
/// @param position the tested position
/// @param board the board where the against which the position is tested
/// @return true if the given position is indexing a valid position on the board
bool is_on_board(board_t *board, unsigned int position);

bool is_cell_empty(board_t* board, unsigned int queen, unsigned int arrow);

bool is_move_legal(board_t *board, struct move_t *move);

/// @returns a struct containing the neighbors list and the amount of neighbors
struct neighbors_t neighbors(unsigned int cell);

/// @returns the neighbor cell in the given direction, MAX_INT if it doesn't
/// exist
unsigned int neighbor(unsigned int cell, enum dir_t direction);

#endif