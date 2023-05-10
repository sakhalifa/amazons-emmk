#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>

#include "dir.h"
#include "graph_ext.h"
#include "move_ext.h"
#include "player.h"
#include "position_set.h"
#define EMPTY_CELL (NUM_PLAYERS + 1)
typedef struct
{
  struct graph_t *graph;
  unsigned int *queens[NUM_PLAYERS];
  unsigned int num_queens;
  bool *arrows; // 1-d maping of the positions of the arrows
  /*
   * 1-d maping of the positions of the queens, to reduce time of is_cell_empty.
   * EMPTY_CELL : No Queen on position
   *  0 : Player 0 Queen on position
   *  1 : Player 1 Queen on position
   */
  unsigned int *queens_on_board;

  /// @brief It gives us the number of accessible vertices. This is useful in the case of weird boards, like clover ones.
  size_t num_accessible_vertices;
} board_t;

/// @brief Initialize the game board
/// @param graph the graph which represents the game board
/// @param num_queens the number of queens
/// @return the board
board_t *init_board(struct graph_t *graph, unsigned int num_queens);

/// @brief Place the given queens on the board
void place_queens_on_board(board_t *board, unsigned int *queens[NUM_PLAYERS], unsigned int num_queens);

/// @brief Frees the cache. THIS SHOULD NOT BE USED IF YOU STILL WANT TO USE METHODS. Or beware of the promised segfault
void neighbors_cache_free();

/// @brief return true if the given position is indexing a valid position on the
/// board
/// @param position the tested position
/// @param board the board where the against which the position is tested
/// @return true if the given position is indexing a valid position on the board
bool is_on_board(board_t *board, int position);

/// @brief check if a cell is empty
/// @param board the board
/// @param cell_position the cell position
bool is_cell_empty(board_t *board, unsigned int cell_position);

/// @brief check if a move is legal
/// @param board the game board
/// @param move the move
/// @param player_id the current player
bool is_move_legal(board_t *board, struct move_t *move, unsigned int player_id);

/// @brief move a queen
/// @param board tha game board
/// @param player_id the current player
/// @param queen_src the queen position
/// @param queen_dst the queen destination
void apply_queen_move(board_t *board, unsigned int player_id, unsigned int queen_src, unsigned int queen_dst);

/// @brief cancel a move
/// @param board tha game board
/// @param player_id the current player
/// @param queen_src the queen position
/// @param queen_dst the queen destination
void cancel_queen_move(board_t *board, unsigned int player_id, unsigned int queen_src, unsigned int queen_dst);

/// @brief Apply the given move to the given board.
/// Remove the arrow and stop the queen.
void apply_move(board_t *board, struct move_t *move, unsigned int player_id);

/// @brief Cancel the given move to the given board.
/// Adds the arrow and move the queen. It doesn't check if the move is valid
void cancel_move(board_t *board, struct move_t *move, unsigned int player_id);

/// @brief Returns the id of the winner
/// @param board the board
/// @return -1 if no winner yet, or the id of the player that won.
int get_winner(board_t *board);

/// @brief free the game board
/// @param board the board
void board_free(board_t *board);

/// @brief print the game board
/// @param board the board
void print_board(board_t *board);

/// @brief Add to the position_set "reachable_positions" all the reachable positions from initial_position in the direction direction_to_neighbor.
/// @param reachable_positions the position_set where the possible moves found by this function are added
/// @param neighbor the id of the first reachable neighbor reached in the given direction
void add_reachable_positions_aligned_deprecated(board_t *board, position_set *reachable_positions, unsigned int neighbor, enum dir_t direction_to_neighbor, size_t width);

/// @brief Add to the position_set "reachable_positions" all the reachable positions from initial_position in the direction direction_to_neighbor.
/// @param reachable_positions the position_set where the possible moves found by this function are added
/// @param neighbor the id of the first reachable neighbor reached in the given direction
void add_reachable_positions_aligned(board_t *board, position_set *reachable_positions, unsigned int neighbor, enum dir_t direction_to_neighbor);

/// @brief Gets all the reachable position from a given position. This is quite unoptimized BUT it works for any graph.
/// @param board The board
/// @param position The position we want to know all its reachable positions
/// @return The set of all the reachable positions from the given position.
position_set *get_reachable_positions_generic(board_t *board, unsigned int position);

/// @brief recursive calls and not as opti as reachable_positions
position_set *get_reachable_arrows_generic(board_t *board, int player_id, unsigned int queen_src, unsigned int queen_dst);

/// @brief return all the possibles moves of the given queen
/// @param queen_position the position of the queen whose possibles moves are returned
/// @param
/// @return all the possibles moves of the given queen
position_set *reachable_positions(board_t *board, unsigned int queen_position);

/// @brief Return true if the considered position is a neighbor of any vertex of the spmatrix, in any direction.
/// @param spmatrix the spmatrix containing the informations abour the neighbors of every vertex and their direction
/// @param position we want to know if that position is the neighbor in any direction of any other postion
/// @return true if the considered position is a neighbor of any vertex of the spmatrix, in any direction.
bool is_reachable(gsl_spmatrix_uint *spmatrix, size_t position);

/// @brief Computes the acessible vertices and sets the \p num_accessible_vertices on the board
/// @param board The board
void compute_accessible_vertices(board_t *board);

#endif