#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "player_ext.h"
#include "position_set.h"
#include "board.h"

static player_t global_player;

void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
    generic_initialize(&global_player, player_id, graph, num_queens, queens, "bot");
}

char const *get_player_name()
{
    return global_player.name;
}

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
    position_set* queen_possible_moves = reachable_positions(global_player.board, available_queens->positions[chosen_queen_index]);
    while (queen_possible_moves->count == 0 && available_queens->count > 1) {
        remove_position_at_index(available_queens, chosen_queen_index);
        free_position_set(queen_possible_moves);
        chosen_queen_index = rand() % available_queens->count;
        queen_possible_moves = reachable_positions(global_player.board, available_queens->positions[chosen_queen_index]);
    }
    if (queen_possible_moves->count == 0) {
        free_position_set(available_queens);
        free_position_set(queen_possible_moves);
        return default_move;
    }
    played_move.queen_src = available_queens->positions[chosen_queen_index];
    free_position_set(available_queens);
    
    // printf("%u\n", played_move.queen_src);
    // print_position_set(queen_possible_moves);
    played_move.queen_dst = queen_possible_moves->positions[rand() % queen_possible_moves->count];
    apply_queen_move(global_player.board, global_player.player_id, played_move.queen_src, played_move.queen_dst);
    free_position_set(queen_possible_moves);
    position_set *arrow_possible_moves = reachable_positions(global_player.board, played_move.queen_dst);
    played_move.arrow_dst = arrow_possible_moves->positions[rand() % arrow_possible_moves->count];
    global_player.board->arrows[played_move.arrow_dst] = true;
    free_position_set(arrow_possible_moves);
    return played_move;
}

void finalize(){
    board_free(global_player.board);
}