#include <math.h>
#include "player_ext.h"
#include <string.h>

static player_t global_player;

void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
    global_player.name = "afk";
    generic_initialize(player_id, graph, num_queens, queens);
}

char const *get_player_name()
{
    return global_player.name;
}

position_set *reachable_positions_square_opti(unsigned int queen_position)
{
    size_t max_different_moves = (global_player.board->graph->num_vertices * 4 - 4);
    position_set *moves = init_position_set(max_different_moves);
    return moves;
}

struct move_t play(struct move_t previous_move)
{
    struct move_t afk = {-1, -1, -1};
    return afk;
}

void finalize(){
    board_free(global_player.board);
}