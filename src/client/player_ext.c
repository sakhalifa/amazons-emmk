#include "player_ext.h"
#include "player.h"
#include "graph.h"

void generic_initialize(player_t *player, unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS], char *name){
    player->name = name;
    player->player_id = player_id;
    player->board = init_board(graph, num_queens);
    place_queens_on_board(player->board, queens, num_queens);
}