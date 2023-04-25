#include "player_ext.h"
#include "player.h"
#include "graph.h"

void generic_initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS]){

    global_player.player_id = player_id;
    global_player.board = init_board(graph, num_queens);
    place_queens_on_board(global_player.board, queens, num_queens);
}