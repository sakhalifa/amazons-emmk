#include "alphabeta.h"
#include "move_ext.h"
#include "player_ext.h"
#include <time.h>

#define TIMEOUT 10

#ifndef PLAYER_NAME
#define PLAYER_NAME NO_NAME
#endif

static player_t global_player;

static double max_time_per_turn;

static int depth = 2;

static int turns = 0;

void initialize(unsigned int player_id, struct graph_t *graph,
				unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
	generic_initialize(&global_player, player_id, graph, num_queens, queens, PLAYER_NAME);
	compute_accessible_vertices(global_player.board);
	if (player_id == 1)
		turns++;
	max_time_per_turn = (double)TIMEOUT / graph->num_vertices;
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
	clock_t start_time = clock();
	struct move_t move = alphabeta(global_player.board, global_player.player_id, depth).move;
	double time_taken = (double)(clock() - start_time) / CLOCKS_PER_SEC;
	if (depth > 2 && time_taken > max_time_per_turn)
		depth--;
	size_t num_vertices = global_player.board->num_accessible_vertices - turns;
	if (((double)1 / (sqrt(num_vertices))) * time_taken < max_time_per_turn/num_vertices)
	{
		depth++;
	}
	if (move.queen_src != FIRST_MOVE_VAL && move.queen_dst != FIRST_MOVE_VAL && move.arrow_dst != FIRST_MOVE_VAL)
		apply_move(global_player.board, &move, global_player.player_id);
	turns += 2;
	return move;
}

void finalize()
{
	board_free(global_player.board);
	neighbors_cache_free();
	depth = 1;
}