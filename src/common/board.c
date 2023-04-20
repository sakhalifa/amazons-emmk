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

bool has_queen(unsigned int player_id, board_t *board, unsigned int queen_position)
{
    for (size_t i = 0; i < board->num_queens; ++i)
    {
        if (board->queens[player_id][i] == queen_position)
        {
            return true;
        }
    }
    return false;
}
// old, deprecated
void add_reachable_positions_aligned_deprecated(board_t *board, position_set *reachable_positions, unsigned int neighbor, enum dir_t direction_to_neighbor, size_t width)
{
    add_position(reachable_positions, neighbor);
    bool found = true;
    while (found)
    {
        found = false;
        size_t next_neighbor = 0;
        while (next_neighbor < width)
        {
            enum dir_t direction_to_next_neighbor = gsl_spmatrix_uint_get(board->graph->t, neighbor, next_neighbor);
            if (direction_to_next_neighbor == direction_to_neighbor && is_cell_empty(board, next_neighbor))
            {
                add_position(reachable_positions, next_neighbor);
                neighbor = next_neighbor;
                next_neighbor = 0;
                found = true;
                break;
            }
            ++next_neighbor;
        }
    }
}

// Only works for square grids
enum dir_t get_move_direction(unsigned int src, unsigned int dst, unsigned int width)
{
    if (dst > src)
    {
        if (dst < src + width - src % width)
        {
            return DIR_EAST;
        }
        else if (dst % width == src % width)
        {
            return DIR_SOUTH;
        }
        else if (dst % (width + 1) == src % (width + 1))
        {
            return DIR_SE;
        }
        else if (dst % (width - 1) == src % (width - 1))
        {
            return DIR_SW;
        }
    }
    else if (dst < src)
    {
        if (dst >= src - src % width)
        {
            return DIR_WEST;
        }
        else if (dst % width == src % width)
        {
            return DIR_NORTH;
        }
        else if (dst % (width + 1) == src % (width + 1))
        {
            return DIR_NW;
        }
        else if (dst % (width - 1) == src % (width - 1))
        {
            return DIR_NE;
        }
    }
    return NO_DIR;
}

// precond : src and dst are valid positions of the board. Board is a square grid.
bool is_reachable_aligned_position(board_t *board, unsigned int src, unsigned int dst)
{
    unsigned int width = sqrt(board->graph->num_vertices);
    int increment;
    size_t iterations;
    enum dir_t move_direction = get_move_direction(src, dst, width);
    switch (move_direction)
    {
    case DIR_EAST:
        increment = 1;
        iterations = width - src % width - 1;
        break;
    case DIR_NE:
        increment = 1 - width;
        iterations = width - src % width - 1;
        break;
    case DIR_SE:
        increment = width + 1;
        iterations = width - src % width - 1;
        break;
    case DIR_WEST:
        increment = -1;
        iterations = src % width;
        break;
    case DIR_NW:
        increment = -width - 1;
        iterations = src % width;
        break;
    case DIR_SW:
        increment = width - 1;
        iterations = src % width;
        break;
    case DIR_NORTH:
        increment = -width;
        iterations = (size_t)src / width;
        break;
    case DIR_SOUTH:
        increment = width;
        iterations = (size_t)width - 1 - src / width;
        break;
    default:
        return false;
    }
    unsigned int next_vertex = src;
    for (size_t i = 0; i < iterations; ++i)
    {
        next_vertex = next_vertex + increment;
        if (gsl_spmatrix_uint_get(board->graph->t, src, next_vertex) != move_direction)
        {
            return false;
        }
        else if (next_vertex == dst)
        {
            return true;
        }
    }
    return false;
}

bool is_reachable_position(board_t *board, unsigned int src, unsigned int dst)
{
    position_set *set = get_reachable_positions_generic(board, src);
    for (size_t i = 0; i < set->count; i++)
    {
        if (set->positions[i] == dst)
        {
            free_position_set(set);
            return true;
        }
    }
    free_position_set(set);
    return false;
}

bool is_reachable_arrow(board_t *board, struct move_t *move, unsigned int player_id)
{
    apply_queen_move(board, player_id, move->queen_src, move->queen_dst);
    bool reachable = is_reachable_position(board, move->queen_dst, move->arrow_dst);
    cancel_queen_move(board, player_id, move->queen_src, move->queen_dst);
    return reachable;
}

bool is_move_legal(board_t *board, struct move_t *move, unsigned int player_id)
{
    return is_on_board(board, move->queen_src) &&
           is_on_board(board, move->queen_dst) &&
           is_on_board(board, move->arrow_dst) &&
           has_queen(player_id, board, move->queen_src) &&
           is_reachable_position(board, move->queen_src, move->queen_dst) &&
           is_reachable_arrow(board, move, player_id);
}

board_t *init_board(struct graph_t *graph, unsigned int num_queens, size_t width)
{
    board_t *board = (board_t *)malloc(sizeof(board_t));
    board->graph = graph;
    board->num_queens = num_queens;
    board->arrows = (bool *)malloc(sizeof(bool) * graph->num_vertices);
    board->width = width;
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
    apply_queen_move(board, player_id, move->queen_src, move->queen_dst);
}

void cancel_move(board_t *board, struct move_t *move, unsigned int player_id)
{
    board->arrows[move->arrow_dst] = false;
    // Find queens dst
    cancel_queen_move(board, player_id, move->queen_src, move->queen_dst);
}

void board_free(board_t *board)
{
    graph_free(board->graph);
    for (int i = 0; i < NUM_PLAYERS; i++)
        free(board->queens[i]);
    free(board->arrows);
    free(board);
}

void print_letter_line(unsigned int size)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        if (i > 0)
            printf(" ");
        printf("%c", 'A' + i);
    }
}

void print_vertical_line(unsigned int size)
{
    for (unsigned int i = 0; i < size; ++i)
    {
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
    printf("-\\\n");
    for (unsigned int i = 0; i < width; ++i)
    {
        printf("%2d |", i + 1);
        for (unsigned int j = 0; j < width; ++j)
        {
            if (j > 0)
                printf(" ");
            unsigned int position = i * width + j;
            bool isReachable = false;
            for (unsigned int i = 0; i < board->graph->t->size1; i++)
            {
                for (unsigned int k = board->graph->t->p[i]; k < (unsigned int)board->graph->t->p[i + 1]; k++)
                {
                    unsigned int j = board->graph->t->i[k];
                    if (i == position || j == position)
                    {
                        isReachable = true;
                        break;
                    }
                }
                if (isReachable)
                    break;
            }
            if (!isReachable)
            {
                printf(" ");
                continue;
            }
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
        printf(" |\n");
    }
    printf("   \\");
    print_vertical_line(width * 2 - 1);
    printf("-/\n");
}

unsigned int find_neighbor_in_direction(struct graph_t *graph, unsigned int position, enum dir_t direction)
{
    for (size_t i = 0; i < graph->num_vertices; i++)
    {
        if (gsl_spmatrix_uint_get(graph->t, position, i) == direction)
        {
            return i;
        }
    }

    return UINT_MAX;
}

position_set *get_reachable_positions_generic(board_t *board, unsigned int position)
{
    size_t max_different_moves = board->width * 4 - 4;
    position_set *set = init_position_set(max_different_moves);
    for (enum dir_t dir = FIRST_DIR; dir <= LAST_DIR; dir++)
    {
        unsigned int neighbor_pos = find_neighbor_in_direction(board->graph, position, dir);
        while (neighbor_pos != UINT_MAX && is_on_board(board, neighbor_pos) && is_cell_empty(board, neighbor_pos))
        {
            add_position(set, neighbor_pos);
            neighbor_pos = find_neighbor_in_direction(board->graph, neighbor_pos, dir);
        }
    }
    return set;
}

position_set *get_reachable_arrows_generic(board_t *board, int player_id, unsigned int queen_src, unsigned int queen_dst)
{
    apply_queen_move(board, player_id, queen_src, queen_dst);
    position_set *set = get_reachable_positions_generic(board, queen_dst);
    cancel_queen_move(board, player_id, queen_src, queen_dst);
    return set;
}

void apply_queen_move(board_t *board, unsigned int player_id, unsigned int queen_src, unsigned int queen_dst)
{
    for (unsigned int queen_id = 0; queen_id < board->num_queens; ++queen_id)
    {
        if (board->queens[player_id][queen_id] == queen_src)
        {
            board->queens[player_id][queen_id] = queen_dst;
            break;
        }
    }
}

void cancel_queen_move(board_t *board, unsigned int player_id, unsigned int queen_src, unsigned int queen_dst)
{
    for (unsigned int queen_id = 0; queen_id < board->num_queens; ++queen_id)
    {
        if (board->queens[player_id][queen_id] == queen_dst)
        {
            board->queens[player_id][queen_id] = queen_src;
            break;
        }
    }
}

int get_winner(board_t *board)
{
    for (size_t i = 0; i < NUM_PLAYERS; i++)
    {
        bool lose = true;
        for (size_t j = 0; j < board->num_queens; j++)
        {
            position_set *set = get_reachable_positions_generic(board, board->queens[i][j]);
            size_t count = set->count;
            free_position_set(set);
            if (count > 0)
            {
                lose = false;
                break;
            }
        }
        if (lose)
            return (i + 1) % NUM_PLAYERS;
    }

    return -1;
}
