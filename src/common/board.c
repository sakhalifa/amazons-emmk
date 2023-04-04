#include <stdio.h>
#include <math.h>
#include "board.h"
#include "move.h"
#include "player.h"

bool is_on_board(board_t *board, unsigned int position) {
  return position >= 0 &&
         position < (board->graph->num_vertices * board->graph->num_vertices);
}

bool is_cell_empty(board_t *board, unsigned int cell_position) {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    for (int j = 0; j < board->num_queens; j++) {
      if (board->queens[i][j] == cell_position) {
        return false;
      }
    }
    if (board->arrows[cell_position] == true) {
      return false;
    }
  }
  return true;
}

bool is_move_legal(board_t *board, struct move_t *move, unsigned int player_id) {
  return is_on_board(board, move->queen_src) &&
         is_on_board(board, move->queen_dst) &&
         is_on_board(board, move->arrow_dst) &&
         is_cell_empty(board, move->queen_dst) &&
         is_cell_empty(board, move->arrow_dst) &&
         !board->arrows[move->arrow_dst];
}

board_t *init_board(struct graph_t *graph, unsigned int num_queens,
                    unsigned int *queens[NUM_PLAYERS]) {
  board_t *board = (board_t *)malloc(sizeof(board_t));
  board->graph = graph;
  board->num_queens = num_queens;
  board->queens = queens;
  board->arrows = (bool *)malloc(sizeof(bool) * graph->num_vertices);
  for (int i = 0; i < graph->num_vertices; ++i) {
    board->arrows[i] = false;
  }
  return board;
}

unsigned int neighbor(unsigned int cell, enum dir_t direction);

struct neighbors_t neighbors(unsigned int cell);

/// @brief Apply the given move to the given board.
/// Adds the arrow and move the queen. It doesn't check if the move is valid
void apply_move(board_t *board, struct move_t *move, unsigned int player_id) {
  board->arrows[move->arrow_dst] = true;
  // Find queens src
  for (unsigned int queen_id = 0; queen_id < board->num_queens; ++queen_id) {
    if (board->queens[player_id][queen_id] == move->queen_src) {
      board->queens[player_id][queen_id] = move->queen_dst;
      break;
    }
  }
}

void board_free(board_t *board) {
  graph_free(board->graph);
  for (int i = 0; i < NUM_PLAYERS; i++)
    free(board->queens[i]);
  free(board->arrows);
  free(board->queens);
  free(board);
}


void print_board(board_t *board){
    unsigned int width = sqrt(board->graph->num_vertices);
    for (unsigned int i = 0; i < width; ++i){
        if (board->arrows[i]){
            printf("a");
        } else if ()

    }

}
