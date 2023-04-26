#include "move_ext.h"
#include "player_ext.h"
#include <math.h>

static player_t global_player;

static unsigned int divider = 0;

static unsigned int turns = 0;

void initialize(unsigned int player_id, struct graph_t *graph,
				unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
	generic_initialize(&global_player, player_id, graph, num_queens, queens, "alphabeta");
	size_t width = (size_t)sqrt(graph->num_vertices);
	divider = width + 0.7 * width;
}

char const *get_player_name()
{
	return global_player.name;
}

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
	for (size_t i = 0; i < board->num_queens; i++)
	{
		position_set *pos = get_reachable_positions_generic(board, board->queens[my_player_id][i]);
		my_score += pos->count;
		free_position_set(pos);
		pos = get_reachable_positions_generic(board, board->queens[other_player_id][i]);
		other_score += pos->count;
		free_position_set(pos);
	}

	return other_score - my_score;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
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
		for (size_t i = 0; i < board->num_queens; i++)
		{
			bool break_for = false;
			int queen_src = board->queens[cur_player_id][i];
			position_set *queen_pos = get_reachable_positions_generic(board, queen_src);
			for (size_t j = 0; j < queen_pos->count; j++)
			{
				int queen_dst = queen_pos->positions[j];
				position_set *arrow_pos = get_reachable_arrows_generic(board, cur_player_id, queen_src, queen_dst);
				for (size_t k = 0; k < arrow_pos->count; k++)
				{
					int arrow_dst = arrow_pos->positions[k];
					struct move_t move = {queen_src, queen_dst, arrow_dst};
					// Transform current node to child node
					apply_move(board, &move, cur_player_id);
					int score = alphabeta_recursive(board, move, my_player_id, alpha, beta, (cur_player_id + 1) % 2, depth - 1).score;
					if (score > value)
					{
						value = score;
						best_move = move;
					}
					if (value > beta)
					{
						break_for = true;
						cancel_move(board, &move, cur_player_id);
						break;
					}
					alpha = max(alpha, value);
					// Transform child node to current node
					cancel_move(board, &move, cur_player_id);
				}
				free_position_set(arrow_pos);
				if (break_for)
					break;
			}
			free_position_set(queen_pos);
			if (break_for)
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
		for (size_t i = 0; i < board->num_queens; i++)
		{
			bool break_for = false;
			int queen_src = board->queens[cur_player_id][i];
			position_set *queen_pos = get_reachable_positions_generic(board, queen_src);
			for (size_t j = 0; j < queen_pos->count; j++)
			{
				int queen_dst = queen_pos->positions[j];
				position_set *arrow_pos = get_reachable_arrows_generic(board, cur_player_id, queen_src, queen_dst);
				for (size_t k = 0; k < arrow_pos->count; k++)
				{
					int arrow_dst = arrow_pos->positions[k];
					struct move_t move = {queen_src, queen_dst, arrow_dst};
					// Transform current node to child node
					apply_move(board, &move, cur_player_id);
					int score = -alphabeta_recursive(board, move, my_player_id, alpha, beta, (cur_player_id + 1) % 2, depth - 1).score;
					if (score < value)
					{
						value = score;
						best_move = move;
					}
					if (value < alpha)
					{
						break_for = true;
						cancel_move(board, &move, cur_player_id);
						break;
					}
					beta = min(beta, value);
					// Transform child node to current node
					cancel_move(board, &move, cur_player_id);
				}
				free_position_set(arrow_pos);
				if (break_for)
					break;
			}
			free_position_set(queen_pos);
			if (break_for)
				break;
		}
		struct move_and_score m = {best_move, value};
		return m;
	}
#pragma endregion
}

struct move_and_score alphabeta(board_t *board, int my_player_id, int depth)
{
	struct move_t cur_move = {-1, -1, -1};
	return alphabeta_recursive(board, cur_move, my_player_id, INT_MIN, INT_MAX, my_player_id, depth);
}

struct move_t play(struct move_t previous_move)
{
	if (previous_move.queen_src != FIRST_MOVE_VAL && previous_move.queen_dst != FIRST_MOVE_VAL && previous_move.arrow_dst != FIRST_MOVE_VAL)
	{
		apply_move(global_player.board, &previous_move, abs((int)global_player.player_id - 1) % NUM_PLAYERS);
	}
	struct move_t move = alphabeta(global_player.board, global_player.player_id, (turns / divider) + 1).move;
	if (move.queen_src != FIRST_MOVE_VAL && move.queen_dst != FIRST_MOVE_VAL && move.arrow_dst != FIRST_MOVE_VAL)
		apply_move(global_player.board, &move, global_player.player_id);
	turns += 1;
	return move;
}

void finalize()
{
	board_free(global_player.board);
	neighbors_cache_free();
	turns = 0;
	divider = 0;
}