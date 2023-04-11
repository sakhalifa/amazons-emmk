#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "board.h"
#include "move.h"
#include "player.h"
#include "position_set.h"
#include "colors.h"

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

void add_reachable_positions_aligned(board_t* board, position_set* reachable_positions, unsigned int neighbor, enum dir_t direction_to_neighbor, size_t width) {
    add_position(reachable_positions, neighbor);
    bool found = true;
    while (found) {
        found = false;
        size_t next_neighbor = 0;
        while (next_neighbor < width) {
            enum dir_t direction_to_next_neighbor = gsl_spmatrix_uint_get(board->graph->t, neighbor, next_neighbor);
            if (direction_to_next_neighbor == direction_to_neighbor) {
                add_position(reachable_positions, next_neighbor);
                neighbor = next_neighbor;
                found = true;
                break;
            }
            ++next_neighbor;
        }
    }
}

enum dir_t get_move_direction(board_t *board, unsigned int src, unsigned int dst) {
    unsigned int width = sqrt(board->graph->num_vertices);
    if (dst > src) {
        if (dst % width == src % width) {
            return DIR_SOUTH;
        } else if (dst % (width + 1) == src % (width + 1)) {
            return DIR_SE;
        } else if (dst % (width - 1) == src % (width - 1)) {
            if (src % width == 0) {
                return DIR_EAST;
            } else {
                return DIR_SW;
            }
        }
    } else if (dst < src) {
        if (dst % width == src % width) {
            return DIR_NORTH;
        } else if (dst % (width + 1) == src % (width + 1)) {
            return DIR_NW;
        } else if (dst % (width - 1) == src % (width - 1)) {
            if (src % width == 0) {
                return DIR_WEST;
            } else {
                return DIR_NE;
            }
        }
    }
    return NO_DIR;
}

bool is_reachable_aligned_move() {
    return true;
}

bool is_move_legal(board_t *board, struct move_t *move, unsigned int player_id) {
    size_t width = (size_t)sqrt(board->graph->num_vertices);
    size_t max_reachable_positions_in_queen_dir = width - 1;
    position_set *reachable_positions_in_queen_dir = init_position_set(max_reachable_positions_in_queen_dir);
    enum dir_t move_direction = get_move_direction(board, move->queen_src, move->queen_dst); // TODO ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
    for (size_t i = 0; i < width; ++i)
    {
        enum dir_t direction_to_i = gsl_spmatrix_uint_get(board->graph->t, move->queen_src, i);
        if (move_direction == direction_to_i)
        {
            add_reachable_positions_aligned(board, reachable_positions_in_queen_dir, i, direction_to_i, width);
        }
    }
    return is_on_board(board, move->queen_src) &&
            is_on_board(board, move->queen_dst) &&
            is_on_board(board, move->arrow_dst) &&
            is_cell_empty(board, move->queen_dst) &&
            is_cell_empty(board, move->arrow_dst) &&
            has_queen(player_id, board, move->queen_src) &&
            is_reachable_aligned_move(board, move->queen_src, move->queen_dst);
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

void print_letter_line(unsigned int size){
    for (unsigned int i = 0; i < size; ++i){
        if (i > 0) printf(" ");
        printf("%c", 'A' + i);
    }
}

void print_vertical_line(unsigned int size){
    for (unsigned int i = 0; i < size; ++i){
        printf("-");
    }
}

void print_board(board_t *board)
{
    unsigned int width = sqrt(board->graph->num_vertices);
    printf("    ");
    print_letter_line(width);
    printf("\n");
    printf("   /");
    print_vertical_line(width * 2 - 1);
    printf("\\\n");
    for (unsigned int i = 0; i < width; ++i)
    {
        printf("%2d |", i + 1);
        for (unsigned int j = 0; j < width; ++j)
        {
            if (j > 0) printf(" ");
            unsigned int position = i * width + j;
            if (board->arrows[position])
            {
                printf("➴");
            }
            else if (has_queen(0, board, position))
            {
                printf(RED "♛" RESET);
            }
            else if (has_queen(1, board, position))
            {   
                printf(CYAN "♛" RESET);
            }
            else
            {
                printf("▢");
            }
        }
        printf("|\n");
    }
    printf("   \\");
    print_vertical_line(width * 2 - 1);
    printf("/\n");
}
