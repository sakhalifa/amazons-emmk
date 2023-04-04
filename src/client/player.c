#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "graph.h"
#include "player_custom.h"
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
    to_return->positions = (unsigned int*) malloc(sizeof(unsigned int) * );
    to_return->count = 0;
    return to_return;
}

void add_position(position_set* set, unsigned int position) {
    set->positions[set->count] = position;
    set->count++;
}
// wip
position_set* possible_moves(unsigned int queen_position) {
    size_t max_different_moves = (sqrt(global_player.board->graph->num_vertices) * 4 - 4);
    position_set* moves = init_position_set(max_different_moves);
    for (size_t i = 0; i < global_player.board->graph->num_vertices; ++i) {
        enum dir_t movement_dir = gsl_spmatrix_uint_get(global_player.board->graph->t, queen_position, i);
        if (movement_dir > 0) {
            add_position(moves, i);
        }
    }
}
//wip
struct move_t play(struct move_t previous_move) {
    if (previous_move.queen_src != UINT_MAX) {
        //update board
    }
    unsigned int random_queen_position = global_player.board->queens[rand() % global_player.board->num_queens];
    position_set* queen_possible_moves = possible_moves(random_queen_position);
    queen_possible_moves->positions[rand() % queen_possible_moves->count];
}