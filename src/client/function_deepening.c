#include "move_ext.h"
#include "player_ext.h"
#include "alphabeta.h"

#ifndef PLAYER_NAME
#define PLAYER_NAME NO_NAME
#endif

static player_t global_player;

static unsigned int turns = 0;

static double exp_coeff;

void initialize(unsigned int player_id, struct graph_t *graph,
				unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
	generic_initialize(&global_player, player_id, graph, num_queens, queens, PLAYER_NAME);
	size_t width = (size_t)sqrt(graph->num_vertices);
	exp_coeff = (1.15 / (exp(0.05 * width) * sqrt(width) * width));
	// if (player_id == 0)
}

char const *get_player_name()
{
	return global_player.name;
}

struct move_t play(struct move_t previous_move)
{
	if (previous_move.queen_src != FIRST_MOVE_VAL && previous_move.queen_dst != FIRST_MOVE_VAL && previous_move.arrow_dst != FIRST_MOVE_VAL)
	{
		apply_move(global_player.board, &previous_move, abs((int)global_player.player_id - 1) % NUM_PLAYERS);
	}
	int depth = exp(exp_coeff * turns);
	if(depth == 1){
		depth = 2;
	}
	struct move_t move = alphabeta(global_player.board, global_player.player_id, depth).move;
	if (move.queen_src != FIRST_MOVE_VAL && move.queen_dst != FIRST_MOVE_VAL && move.arrow_dst != FIRST_MOVE_VAL)
		apply_move(global_player.board, &move, global_player.player_id);
	turns += 2;
	return move;
}

void finalize()
{
	board_free(global_player.board);
	neighbors_cache_free();
	turns = 0;
}