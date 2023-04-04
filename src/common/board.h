#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>

#include "dir.h"
#include "graph_ext.h"
#include "move.h"
#include "player.h"
#include "position_set.h"

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
bool is_on_board(board_t *board, int position);

bool is_cell_empty(board_t* board, unsigned int cell_position);

bool is_move_legal(board_t *board, struct move_t *move, unsigned int player_id);

/// @returns a struct containing the neighbors list and the amount of neighbors
struct neighbors_t neighbors(unsigned int cell);

/// @returns the neighbor cell in the given direction, MAX_INT if it doesn't
/// exist
unsigned int neighbor(unsigned int cell, enum dir_t direction);

void apply_move(board_t *board, struct move_t *move, unsigned int player_id);

void board_free(board_t *board);

void print_board(board_t *board);

/// @brief Add to the position_set "possible_moves" all the reachable positions from initial_position in the direction direction_to_neighbor.
/// @param possible_moves the position_set where the possible moves found by this function are added
/// @param initial_position the id of the position from where we try to find reachable positions in the given direction
/// @param neighbor the id of the first reachable neighbor reached in the given direction
void add_possible_moves_aligned(board_t* board, position_set* possible_moves, unsigned int initial_position, unsigned int neighbor, enum dir_t direction_to_neighbor);

#endif