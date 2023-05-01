#include "heuristic.h"
#ifndef AGG
#define AGG 0
#endif

#ifndef DEF
#define DEF 0
#endif

int get_score(board_t *board, int player_id)
{
	int my_score = 0;
	int other_score = 0;
	int other_player_id = (player_id + 1) % 2;
	for (size_t i = 0; i < board->num_queens; i++)
	{
		position_set *pos = get_reachable_positions_generic(board, board->queens[player_id][i]);
		my_score += pos->count;
		free_position_set(pos);
		pos = get_reachable_positions_generic(board, board->queens[other_player_id][i]);
		other_score += pos->count;
		free_position_set(pos);
	}

	return my_score - other_score;
}