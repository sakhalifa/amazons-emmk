#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "graph.h"
#include "player_ext.h"
#include "board.h"
#include "position_set.h"

static player_t global_player;

void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
    global_player.player_id = player_id;
    global_player.name = "bot";
    global_player.board = init_board(graph, num_queens);
    for(size_t i = 0; i < NUM_PLAYERS; i++){
        global_player.board->queens[i] = queens[i];
    }
}

char const *get_player_name()
{
    return global_player.name;
}

//for square board shape, not opti
position_set *reachable_positions_deprecated(unsigned int queen_position)
{  
    size_t width = (size_t)sqrt(global_player.board->graph->num_vertices);
    size_t max_different_moves = width*4 - 4;
    position_set *moves = init_position_set(max_different_moves);
    for (size_t i = 0; i < global_player.board->graph->num_vertices; ++i)
    {
        enum dir_t direction_to_i = gsl_spmatrix_uint_get(global_player.board->graph->t, (size_t)queen_position, i);
        if (direction_to_i != NO_DIR && is_cell_empty(global_player.board, i))
        {
            //printf("in, q : %u, i : %u\n", queen_position, i); // for debug
            add_reachable_positions_aligned_deprecated(global_player.board, moves, i, direction_to_i, width);
        }
    }
    // printf("end\n"); // for debug
    return moves;
}

//for square board shape, not opti
position_set *reachable_positions(unsigned int queen_position)
{  
    size_t max_different_moves = global_player.board->graph->num_vertices - 1;
    position_set *moves = init_position_set(max_different_moves);
    for (size_t i = 0; i < global_player.board->graph->num_vertices; ++i)
    {
        enum dir_t direction_to_i = gsl_spmatrix_uint_get(global_player.board->graph->t, (size_t)queen_position, i);
        if (direction_to_i != NO_DIR && is_cell_empty(global_player.board, i))
        {
            //printf("in, q : %u, i : %u\n", queen_position, i); // for debug
            add_reachable_positions_aligned(global_player.board, moves, i, direction_to_i);
        }
    }
    // printf("end\n"); // for debug
    return moves;
}

// for square board (cardinal and diagonal directions)
//position_set *reachable_positions_square_opti(unsigned int queen_position) {
//    size_t width = (size_t)sqrt(global_player.board->graph->num_vertices);
//    return NULL;
//}

struct move_t play(struct move_t previous_move)
{
    if (previous_move.queen_src != UINT_MAX)
    {
        apply_move(global_player.board, &previous_move, (global_player.player_id + 1) % 2);
    }
    struct move_t default_move = {UINT_MAX - 1, UINT_MAX - 1, UINT_MAX - 1};
    struct move_t played_move;

    position_set* available_queens = init_position_set(global_player.board->num_queens);
    for (size_t i = 0; i < global_player.board->num_queens; ++i) {
        add_position(available_queens, global_player.board->queens[global_player.player_id][i]);
    }

    size_t chosen_queen_index = rand() % available_queens->count;
    position_set* queen_possible_moves = reachable_positions(available_queens->positions[chosen_queen_index]);
    while (queen_possible_moves->count == 0 && available_queens->count > 1) {
        remove_position_at_index(available_queens, chosen_queen_index);
        free_position_set(queen_possible_moves);
        chosen_queen_index = rand() % available_queens->count;
        queen_possible_moves = reachable_positions(available_queens->positions[chosen_queen_index]);
    }
    if (queen_possible_moves->count == 0) {
        free_position_set(available_queens);
        free_position_set(queen_possible_moves);
        return default_move;
    }
    
    played_move.queen_src = available_queens->positions[chosen_queen_index];
    free_position_set(available_queens);
    
    printf("%u\n", played_move.queen_src);
    print_position_set(queen_possible_moves);
    played_move.queen_dst = queen_possible_moves->positions[rand() % queen_possible_moves->count];
    apply_queen_move(global_player.board, global_player.player_id, played_move.queen_src, played_move.queen_dst);
    free_position_set(queen_possible_moves);
    position_set *arrow_possible_moves = reachable_positions(played_move.queen_dst);
    played_move.arrow_dst = arrow_possible_moves->positions[rand() % arrow_possible_moves->count];
    global_player.board->arrows[played_move.arrow_dst] = true;
    free_position_set(arrow_possible_moves);
    return played_move;
}

void finalize(){
    board_free(global_player.board);
}