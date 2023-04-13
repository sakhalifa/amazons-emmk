#include "move.h"
#include "board.h"
#include "player_ext.h"

struct move_and_score
{
	struct move_t move;
	int score;
};

int get_score(board_t *board, int my_player_id)
{
	int my_score = 0;
	int other_score = 0;
	int other_player_id = (my_player_id + 1) % 2;
	for (int i = 0; i < board->num_queens; i++)
	{
		position_set *pos = reachable_positions_deprecated(board->queens[my_player_id][i]);
		my_score += pos->count;
		free_position_set(pos);
		pos = reachable_positions_deprecated(board->queens[other_player_id][i]);
		other_score += pos->count;
		free_position_set(pos);
	}

	return my_score - other_score;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

struct move_and_score alphabeta_recursive(board_t *board, struct move_t cur_move, int my_player_id, int alpha, int beta, int cur_player_id, int depth)
{
	if (depth == 0)
	{
		struct move_and_score move_score = {.move = cur_move, .score = get_score(board, cur_player_id)};
		return move_score;
	}
	#pragma region maximize
	if (my_player_id == cur_player_id)
	{
		struct move_t best_move = {-1, -1, -1};
		int value = INT_MIN;
		// For each child node
		for (int i = 0; i < board->num_queens; i++)
		{
			bool break_for = false;
			int queen_src = board->queens[cur_player_id][i];
			position_set *queen_pos = reachable_positions_deprecated(queen_src);
			for (int j = 0; j < queen_pos->count; j++)
			{
				int queen_dst = queen_pos->positions[j];
				position_set *arrow_pos = reachable_positions_deprecated(queen_dst);
				for (int k = 0; k < arrow_pos->count; k++)
				{
					int arrow_dst = arrow_pos->positions[k];
					struct move_t move = {queen_src, queen_dst, arrow_dst};
					// Transform current node to child node
					apply_move(board, &move, cur_player_id);
					int score = alphabeta_recursive(board, move, my_player_id, alpha, beta, (cur_player_id + 1) % 2, depth - 1).score;
					if(score > value){
						value = score;
						best_move = move;
					}
					if(value > beta){
						break_for = true;
						break;
					}
					alpha = max(alpha, value);
					// Transform child node to current node
					cancel_move(board, &move, cur_player_id);
				}
				free_position_set(arrow_pos);
				if(break_for)
					break;
			}
			free_position_set(queen_pos);
			if(break_for)
				break;
		}
		struct move_and_score m = {best_move, value};
		return m;
	}
	#pragma endregion
	#pragma region minimize
	else
	{
		struct move_t best_move = {-1, -1, -1};
		int value = INT_MAX;
		// For each child node
		for (int i = 0; i < board->num_queens; i++)
		{
			bool break_for = false;
			int queen_src = board->queens[cur_player_id][i];
			position_set *queen_pos = reachable_positions_deprecated(queen_src);
			for (int j = 0; j < queen_pos->count; j++)
			{
				int queen_dst = queen_pos->positions[j];
				position_set *arrow_pos = reachable_positions_deprecated(queen_dst);
				for (int k = 0; k < arrow_pos->count; k++)
				{
					int arrow_dst = arrow_pos->positions[k];
					struct move_t move = {queen_src, queen_dst, arrow_dst};
					// Transform current node to child node
					apply_move(board, &move, cur_player_id);
					int score = alphabeta_recursive(board, move, my_player_id, alpha, beta, (cur_player_id + 1) % 2, depth - 1).score;
					if(score < value){
						value = score;
						best_move = move;
					}
					if(value < alpha){
						break_for = true;
						break;
					}
					beta = min(beta, value);
					// Transform child node to current node
					cancel_move(board, &move, cur_player_id);
				}
				free_position_set(arrow_pos);
				if(break_for)
					break;
			}
			free_position_set(queen_pos);
			if(break_for)
				break;
		}
		struct move_and_score m = {best_move, value};
		return m;

	}
	#pragma endregion
}

struct move_and_score alphabeta(board_t *board, int my_player_id, int depth)
{
	struct move_t cur_move = {{-1, -1, -1}};
	return alphabeta_recursive(board, cur_move, my_player_id, INT_MIN, INT_MAX, my_player_id, depth);
}