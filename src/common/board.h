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
  unsigned int *queens[NUM_PLAYERS];
  unsigned int num_queens;
  bool *arrows; // 1-d maping of the positions of the arrows
} board_t;

board_t *init_board(struct graph_t *graph, unsigned int num_queens);

/// @brief return true if the given position is indexing a valid position on the
/// board
/// @param position the tested position
/// @param board the board where the against which the position is tested
/// @return true if the given position is indexing a valid position on the board
bool is_on_board(board_t *board, int position);

bool is_cell_empty(board_t* board, unsigned int cell_position);

bool is_move_legal(board_t *board, struct move_t *move, unsigned int player_id);

void apply_queen_move(board_t *board, unsigned int player_id, unsigned int queen_src, unsigned int queen_dst);

void cancel_queen_move(board_t *board, unsigned int player_id, unsigned int queen_src, unsigned int queen_dst);

void apply_move(board_t *board, struct move_t *move, unsigned int player_id);
void cancel_move(board_t *board, struct move_t *move, unsigned int player_id);

void board_free(board_t *board);

void print_board(board_t *board);

/// @brief Add to the position_set "reachable_positions" all the reachable positions from initial_position in the direction direction_to_neighbor.
/// @param reachable_positions the position_set where the possible moves found by this function are added
/// @param neighbor the id of the first reachable neighbor reached in the given direction
void add_reachable_positions_aligned_deprecated(board_t* board, position_set* reachable_positions, unsigned int neighbor, enum dir_t direction_to_neighbor, size_t width);

void add_reachable_positions_aligned(board_t *board, position_set *reachable_positions, unsigned int neighbor, enum dir_t direction_to_neighbor);

/// @brief Gets all the reachable position from a given position. This is quite unoptimized BUT it works for any graph.
/// @param board The board 
/// @param position The position we want to know all its reachable positions
/// @return The set of all the reachable positions from the given position.
position_set *get_reachable_positions_generic(board_t *board, unsigned int position);

position_set *get_reachable_arrows_generic(board_t *board, int player_id, unsigned int queen_src, unsigned int queen_dst);

#endif