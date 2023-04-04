#include <stdio.h>
#include <math.h>
#include "board.h"
#include "move.h"
#include "player.h"
#include "position_set.h"

bool is_on_board(board_t *board, int position)
{
    return position >= 0 &&
           (unsigned int)position < (board->graph->num_vertices * board->graph->num_vertices);
}

bool is_cell_empty(board_t *board, unsigned int cell_position)
{
    for (size_t i = 0; i < NUM_PLAYERS; i++)
    {
        for (size_t j = 0; j < board->num_queens; j++)
        {
            if (board->queens[i][j] == cell_position)
            {
                return false;
            }
        }
    }
  return !board->arrows[cell_position];
}

bool has_queen (unsigned int player_id, board_t *board, unsigned int queen_position) {
    for (size_t i = 0; i < board->num_queens; ++i) {
        if (board->queens[player_id][i] == queen_position) {
            return true;
        }
    }
    return false;
}

void add_possible_moves_aligned(board_t* board, position_set* possible_moves, unsigned int initial_position, unsigned int neighbor, enum dir_t direction_to_neighbor) {
    add_position(possible_moves, neighbor);
    bool found = true;
    while (found) {
        found = false;
        size_t next_neighbor = 0;
        while (next_neighbor < board->graph->num_vertices) {
            enum dir_t direction_to_next_neighbor = gsl_spmatrix_uint_get(board->graph->t, neighbor, next_neighbor);
            if (direction_to_next_neighbor == direction_to_neighbor) {
                // add_position(next_neighbor, neighbor);
                add_position(possible_moves, next_neighbor);
                neighbor = next_neighbor;
                found = true;
                break;
            }
            ++next_neighbor;
        }
    }
}

bool is_move_legal(board_t *board, struct move_t *move, unsigned int player_id) {
  return is_on_board(board, move->queen_src) &&
         is_on_board(board, move->queen_dst) &&
         is_on_board(board, move->arrow_dst) &&
         is_cell_empty(board, move->queen_dst) &&
         is_cell_empty(board, move->arrow_dst) &&
         has_queen(player_id, board, move->queen_src);
}

board_t *init_board(struct graph_t *graph, unsigned int num_queens)
{
    board_t *board = (board_t *)malloc(sizeof(board_t));
    board->graph = graph;
    board->num_queens = num_queens;
    board->arrows = (bool *)malloc(sizeof(bool) * graph->num_vertices);
    for (size_t i = 0; i < graph->num_vertices; ++i)
    {
        board->arrows[i] = false;
    }
    return board;
}

/// @brief Apply the given move to the given board.
/// Adds the arrow and move the queen. It doesn't check if the move is valid
void apply_move(board_t *board, struct move_t *move, unsigned int player_id)
{
    board->arrows[move->arrow_dst] = true;
    // Find queens src
    for (unsigned int queen_id = 0; queen_id < board->num_queens; ++queen_id)
    {
        if (board->queens[player_id][queen_id] == move->queen_src)
        {
            board->queens[player_id][queen_id] = move->queen_dst;
            break;
        }
    }
}

void board_free(board_t *board)
{
    graph_free(board->graph);
    for (int i = 0; i < NUM_PLAYERS; i++)
        free(board->queens[i]);
    free(board->arrows);
    free(board);
}

void print_board(board_t *board)
{
    unsigned int width = sqrt(board->graph->num_vertices);
    for (unsigned int i = 0; i < width; ++i)
    {
        for (unsigned int j = 0; j < width; ++j)
        {
            if (board->arrows[i * width + j])
            {
                printf("a");
            }
            else if (!is_cell_empty(board, i * width + j))
            {
                printf("q");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}
