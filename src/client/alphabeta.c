#include "heuristic.h"
#include "alphabeta.h"
#include <math.h>

static inline int max(int a, int b)
{
	return a > b ? a : b;
}

static inline int min(int a, int b)
{
	return a < b ? a : b;
}

struct move_and_score alphabeta_recursive(board_t *board, struct move_t cur_move, int my_player_id, int alpha, int beta, int cur_player_id, int depth);

struct move_and_score max_alphabeta(board_t *board, int my_player_id, int alpha, int beta, int cur_player_id, int depth)
{
	struct move_and_score best_move = {.move = FIRST_MOVE, .score = INT_MIN};
	for (unsigned int queen_id = 0; queen_id < board->num_queens; queen_id++)
	{
		unsigned int queen_src = board->queens[cur_player_id][queen_id];

		position_set *queen_destinations = get_reachable_positions_generic(board, queen_src);
		for (unsigned int queen_dst_id = 0; queen_dst_id < queen_destinations->count; queen_dst_id++)
		{
			unsigned int queen_dst = queen_destinations->positions[queen_dst_id];

			position_set *arrow_destinations = get_reachable_arrows_generic(board, cur_player_id, queen_src, queen_dst);
			for (unsigned int arrow_dst_id = 0; arrow_dst_id < arrow_destinations->count; arrow_dst_id++)
			{
				unsigned int arrow_dst = arrow_destinations->positions[arrow_dst_id];

				struct move_t cur_move = {queen_src, queen_dst, arrow_dst};

				apply_move(board, &cur_move, cur_player_id);
				struct move_and_score node_best_move = alphabeta_recursive(board, cur_move, my_player_id, alpha, beta, (cur_player_id + 1) % NUM_PLAYERS, depth - 1);
				cancel_move(board, &cur_move, cur_player_id);

				if (node_best_move.score > best_move.score)
				{
					best_move.move = cur_move;
					best_move.score = node_best_move.score;
				}

				if (best_move.score >= beta)
				{
					free_position_set(arrow_destinations);
					free_position_set(queen_destinations);
					return best_move;
				}

				alpha = max(alpha, best_move.score);
			}
			free_position_set(arrow_destinations);
		}
		free_position_set(queen_destinations);
	}

	return best_move;
}

struct move_and_score min_alphabeta(board_t *board, int my_player_id, int alpha, int beta, int cur_player_id, int depth)
{
	struct move_and_score best_move = {.move = FIRST_MOVE, .score = INT_MAX};
	for (unsigned int queen_id = 0; queen_id < board->num_queens; queen_id++)
	{
		unsigned int queen_src = board->queens[cur_player_id][queen_id];

		position_set *queen_destinations = get_reachable_positions_generic(board, queen_src);
		for (unsigned int queen_dst_id = 0; queen_dst_id < queen_destinations->count; queen_dst_id++)
		{
			unsigned int queen_dst = queen_destinations->positions[queen_dst_id];

			position_set *arrow_destinations = get_reachable_arrows_generic(board, cur_player_id, queen_src, queen_dst);
			for (unsigned int arrow_dst_id = 0; arrow_dst_id < arrow_destinations->count; arrow_dst_id++)
			{
				unsigned int arrow_dst = arrow_destinations->positions[arrow_dst_id];

				struct move_t cur_move = {queen_src, queen_dst, arrow_dst};

				apply_move(board, &cur_move, cur_player_id);
				struct move_and_score node_best_move = alphabeta_recursive(board, cur_move, my_player_id, alpha, beta, (cur_player_id + 1) % NUM_PLAYERS, depth - 1);
				cancel_move(board, &cur_move, cur_player_id);

				if (node_best_move.score < best_move.score)
				{
					best_move.move = cur_move;
					best_move.score = node_best_move.score;
				}

				if (alpha >= best_move.score)
				{
					free_position_set(arrow_destinations);
					free_position_set(queen_destinations);
					return best_move;
				}
				beta = min(beta, best_move.score);
			}
			free_position_set(arrow_destinations);
		}
		free_position_set(queen_destinations);
	}
	return best_move;
}

struct move_and_score alphabeta_recursive(board_t *board, struct move_t cur_move, int my_player_id, int alpha, int beta, int cur_player_id, int depth)
{
	if (depth == 0 || get_winner(board) != -1)
	{
		struct move_and_score move_score = {.move = cur_move, .score = get_score(board, my_player_id)};
		return move_score;
	}
	if (cur_player_id == my_player_id)
	{
		return max_alphabeta(board, my_player_id, alpha, beta, cur_player_id, depth);
	}
	else
	{
		return min_alphabeta(board, my_player_id, alpha, beta, cur_player_id, depth);
	}
}

struct move_t get_random_move(board_t *board, int player_id)
{
	struct move_t random_move = FIRST_MOVE;
	for (unsigned int queen_id = 0; queen_id < board->num_queens; queen_id++)
	{
		unsigned int queen_src = board->queens[player_id][queen_id];

		position_set *queen_destinations = get_reachable_positions_generic(board, queen_src);
		for (unsigned int queen_dst_id = 0; queen_dst_id < queen_destinations->count; queen_dst_id++)
		{
			unsigned int queen_dst = queen_destinations->positions[queen_dst_id];

			position_set *arrow_destinations = get_reachable_arrows_generic(board, player_id, queen_src, queen_dst);
			for (unsigned int arrow_dst_id = 0; arrow_dst_id < arrow_destinations->count; arrow_dst_id++)
			{
				unsigned int arrow_dst = arrow_destinations->positions[arrow_dst_id];

				random_move.queen_src = queen_src;
				random_move.queen_dst = queen_dst;
				random_move.arrow_dst = arrow_dst;
				free_position_set(arrow_destinations);
				free_position_set(queen_destinations);

				return random_move;
			}
			free_position_set(arrow_destinations);
		}
		free_position_set(queen_destinations);
	}
	return random_move;
}

struct move_and_score alphabeta(board_t *board, int my_player_id, int depth)
{
	struct move_t cur_move = get_random_move(board, my_player_id);
	return alphabeta_recursive(board, cur_move, my_player_id, INT_MIN, INT_MAX, my_player_id, depth);
}
