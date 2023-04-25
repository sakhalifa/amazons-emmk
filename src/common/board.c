#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "move_ext.h"
#include "player.h"
#include "position_set.h"
#include "colors.h"
#include "assert.h"

bool is_on_board(board_t *board, int position)
{
    return position >= 0 &&
           (unsigned int)position < (board->graph->num_vertices * board->graph->num_vertices);
}

bool is_cell_empty(board_t *board, unsigned int cell_position)
{
    return board->queens_on_board[cell_position] == EMPTY_CELL && !board->arrows[cell_position];    
}

bool has_queen(unsigned int player_id, board_t *board, unsigned int queen_position)
{
    return board->queens_on_board[queen_position] == player_id;
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
        while (next_neighbor < width) //bug? width != num_vertices
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

void add_reachable_positions_aligned(board_t *board, position_set *reachable_positions, unsigned int neighbor, enum dir_t direction_to_neighbor)
{
    bool found = true;
    while (found)
    {
        found = false;
        size_t next_neighbor = 0;
        while (next_neighbor < board->graph->num_vertices)
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

board_t *init_board(struct graph_t *graph, unsigned int num_queens)
{
    board_t *board = (board_t *)malloc(sizeof(board_t));
    board->graph = graph;
    board->num_queens = num_queens;
    board->arrows = (bool *)malloc(sizeof(bool) * graph->num_vertices);
    board->queens_on_board = (unsigned int *)malloc(sizeof(unsigned int) * graph->num_vertices);
    for (size_t i = 0; i < graph->num_vertices; ++i)
    {
        board->arrows[i] = false;
        board->queens_on_board[i] = EMPTY_CELL;
    }
    
    return board;
}

void place_queens_on_board(board_t *board, unsigned int* queens[NUM_PLAYERS], unsigned int num_queens){
    // Init queens array
    for(size_t i = 0; i < NUM_PLAYERS; i++){
        board->queens[i] = queens[i];
    }

    // Init board of queens (for direct access from a position)
    for(size_t player_id = 0; player_id < NUM_PLAYERS; ++player_id){
        for (size_t queen_index = 0; queen_index < num_queens; ++queen_index){
            board->queens_on_board[queens[player_id][queen_index]] = player_id;
        }
    }
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
    free(board->queens_on_board);
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

static bool is_cache_initialized = false;
static unsigned int *neighbor_cache[NUM_DIRS];

void initialize_neighbor_cache(size_t num_vertices)
{
    for (enum dir_t dir = FIRST_DIR; dir <= LAST_DIR; dir++)
    {
        neighbor_cache[dir - FIRST_DIR] = malloc(sizeof(unsigned int) * num_vertices);
        for (size_t i = 0; i < num_vertices; i++)
            neighbor_cache[dir - FIRST_DIR][i] = UINT_MAX - 1;
    }
}

unsigned int find_neighbor_in_cache(unsigned int position, enum dir_t direction)
{
    return neighbor_cache[direction - FIRST_DIR][position];
}

unsigned int find_neighbor_in_direction(struct graph_t *graph, unsigned int position, enum dir_t direction)
{
    if (!is_cache_initialized)
    {
        initialize_neighbor_cache(graph->num_vertices);
        is_cache_initialized = true;
    }
    unsigned int result = find_neighbor_in_cache(position, direction);
    if (result != UINT_MAX - 1)
    {
        return result;
    }
    for (size_t i = 0; i < graph->num_vertices; i++)
    {
        if (gsl_spmatrix_uint_get(graph->t, position, i) == direction)
        {
            neighbor_cache[direction - FIRST_DIR][position] = i;
            return i;
        }
    }
    neighbor_cache[direction - FIRST_DIR][position] = UINT_MAX;
    return UINT_MAX;
}

// recursive calls and not as opti as reachable_positions.
position_set *get_reachable_positions_generic(board_t *board, unsigned int position)
{
    size_t max_different_moves = board->graph->num_vertices - 1;
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

// any board shape, no recursive call
position_set *reachable_positions(board_t *board, unsigned int queen_position)
{  
    size_t max_different_moves = board->graph->num_vertices - 1;
    position_set *moves = init_position_set(max_different_moves);
    for (size_t vertex = 0; vertex < board->graph->num_vertices; ++vertex)
    {
        enum dir_t direction_to_vertex = gsl_spmatrix_uint_get(board->graph->t, (size_t)queen_position, vertex);
        if (direction_to_vertex != NO_DIR && is_cell_empty(board, vertex))
        {
            add_position(moves, vertex);
            add_reachable_positions_aligned(board, moves, vertex, direction_to_vertex);
            size_t current_vertex = vertex;
            bool found = true;
            while (found)
            {
                found = false;
                size_t next_neighbor = 0;
                while (next_neighbor < board->graph->num_vertices)
                {
                    enum dir_t direction_to_next_neighbor = gsl_spmatrix_uint_get(board->graph->t, current_vertex, next_neighbor);
                    if (direction_to_next_neighbor == direction_to_vertex && is_cell_empty(board, next_neighbor))
                    {
                        add_position(moves, next_neighbor);
                        current_vertex = next_neighbor;
                        next_neighbor = 0;
                        found = true;
                        break;
                    }
                    ++next_neighbor;
                }
            }
        }
    }
    return moves;
}

//for square board shape, not opti
//position_set *reachable_positions_deprecated(unsigned int queen_position)
//{  
//    size_t width = (size_t)sqrt(global_player.board->graph->num_vertices);
//    size_t max_different_moves = width*4 - 4;
//    position_set *moves = init_position_set(max_different_moves);
//    for (size_t i = 0; i < global_player.board->graph->num_vertices; ++i)
//    {
//        enum dir_t direction_to_i = gsl_spmatrix_uint_get(global_player.board->graph->t, (size_t)queen_position, i);
//        if (direction_to_i != NO_DIR && is_cell_empty(global_player.board, i))
//        {
//            //printf("in, q : %u, i : %u\n", queen_position, i); // for debug
//            add_reachable_positions_aligned_deprecated(global_player.board, moves, i, direction_to_i, width);
//        }
//    }
//    // printf("end\n"); // for debug
//    return moves;
//}

// for square board (cardinal and diagonal directions), opti
//position_set *reachable_positions_square_opti(unsigned int queen_position) {
//    size_t width = (size_t)sqrt(global_player.board->graph->num_vertices);
//    return NULL;
//}

position_set *get_reachable_arrows_generic(board_t *board, int player_id, unsigned int queen_src, unsigned int queen_dst)
{
    apply_queen_move(board, player_id, queen_src, queen_dst);
    position_set *set = get_reachable_positions_generic(board, queen_dst);
    cancel_queen_move(board, player_id, queen_src, queen_dst);
    return set;
}

void apply_queen_move(board_t *board, unsigned int player_id, unsigned int queen_src, unsigned int queen_dst)
{
    for (unsigned int queen_index = 0; queen_index < board->num_queens; ++queen_index)
    {
        if (board->queens[player_id][queen_index] == queen_src)
        {
            board->queens[player_id][queen_index] = queen_dst;
            break;
        }
    }
    board->queens_on_board[queen_src] = EMPTY_CELL;
    board->queens_on_board[queen_dst] = player_id;
}

void cancel_queen_move(board_t *board, unsigned int player_id, unsigned int queen_src, unsigned int queen_dst)
{
    for (unsigned int queen_index = 0; queen_index < board->num_queens; ++queen_index)
    {
        if (board->queens[player_id][queen_index] == queen_dst)
        {
            board->queens[player_id][queen_index] = queen_src;
            break;
        }
    }
    board->queens_on_board[queen_src] = player_id;
    board->queens_on_board[queen_dst] = EMPTY_CELL;
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
