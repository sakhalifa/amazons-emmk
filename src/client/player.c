#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "graph.h"
#include "player_ext.h"
#include "board.h"

typedef struct {
    unsigned int player_id;
    char const name[64];
    board_t* board;
} player_t;

static player_t global_player;

typedef struct {
    unsigned int* positions;
    size_t count;
} position_set;

void initialize(unsigned int player_id, struct graph_t* graph,
                unsigned int num_queens, unsigned int* queens[NUM_PLAYERS]) {
    global_player.player_id = player_id;
    strcpy(global_player.name, "bot");
    global_player.board = init_board(graph, num_queens, queens);
}

char const* get_player_name() {
    return global_player.name;
}

position_set* init_position_set(size_t capacity) {
    position_set* to_return = (position_set*) malloc(sizeof(position_set));
    to_return->positions = (unsigned int*) malloc(sizeof(unsigned int) * capacity);
    to_return->count = 0;
    return to_return;
}

void add_position(position_set* set, unsigned int position) {
    set->positions[set->count] = position;
    set->count++;
}

void add_possible_moves_aligned(position_set* possible_moves, unsigned int initial_position, unsigned int neighbor, enum dir_t direction_to_neighbor) {
    add_position(possible_moves, neighbor);
    bool found = true;
    while (found) {
        found = false;
        size_t next_neighbor = 0;
        while (next_neighbor < global_player.board->graph->num_vertices) {
            enum dir_t direction_to_next_neighbor = gsl_spmatrix_uint_get(global_player.board->graph->t, neighbor, next_neighbor);
            if (direction_to_next_neighbor == direction_to_neighbor) {
                add_position(next_neighbor, neighbor);
                neighbor = next_neighbor;
                found = true;
                break;
            }
            ++next_neighbor;
        }
    }
}

position_set* possible_moves(unsigned int queen_position) {
    size_t max_different_moves = (sqrt(global_player.board->graph->num_vertices) * 4 - 4);
    position_set* moves = init_position_set(max_different_moves);
    for (size_t i = 0; i < global_player.board->graph->num_vertices; ++i) {
        enum dir_t direction_to_i = gsl_spmatrix_uint_get(global_player.board->graph->t, queen_position, i);
        if (direction_to_i > 0) {
            add_possible_moves_aligned(moves, queen_position, i, direction_to_i);
        }
    return moves;
    }
}
struct move_t play(struct move_t previous_move) {
    if (previous_move.queen_src != UINT_MAX) {
        apply_move(global_player.board, &previous_move, global_player.player_id);
    }
    struct move_t played_move;
    played_move.queen_src = global_player.board->queens[rand() % global_player.board->num_queens];
    position_set* queen_possible_moves = possible_moves(played_move.queen_src);
    played_move.queen_dst = queen_possible_moves->positions[rand() % queen_possible_moves->count];
    position_set* arrow_possible_moves = possible_moves(played_move.queen_dst);
    played_move.arrow_dst = arrow_possible_moves->positions[rand() % arrow_possible_moves->count];
    return played_move;
}