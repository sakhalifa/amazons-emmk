#include "move_ext.h"
#include "player_ext.h"
#include "heuristic.h"
#include <time.h>
#include <math.h>

#define TIMEOUT 10

struct move_and_score
{
	struct move_t move;
	int score;
};

struct move_and_score alphabeta(board_t *board, int my_player_id, int depth);

static player_t global_player;

static double max_time_per_turn;

static int depth = 1;

static int turns = 0;

void initialize(unsigned int player_id, struct graph_t *graph,
				unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
	generic_initialize(&global_player, player_id, graph, num_queens, queens, "alphachad");
	if (player_id == 1)
		turns++;
	max_time_per_turn = (double)TIMEOUT / graph->num_vertices;
}

char const *get_player_name()
{
	return global_player.name;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

struct move_and_score alphabeta_recursive(board_t *board, struct move_t cur_move, int my_player_id, int alpha, int beta, int cur_player_id, int depth);

struct move_and_score max_alphabeta(board_t *board, int my_player_id, int alpha, int beta, int cur_player_id, int depth)
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
struct move_and_score min_alphabeta(board_t *board, int my_player_id, int alpha, int beta, int cur_player_id, int depth)
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
				int score = alphabeta_recursive(board, move, my_player_id, alpha, beta, (cur_player_id + 1) % 2, depth - 1).score;
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

struct move_and_score alphabeta_recursive(board_t *board, struct move_t cur_move, int my_player_id, int alpha, int beta, int cur_player_id, int depth)
{
	if (depth == 0)
	{
		struct move_and_score move_score = {.move = cur_move, .score = get_score(board, global_player.player_id)};
		return move_score;
	}
	if (my_player_id == cur_player_id)
	{
		return max_alphabeta(board, my_player_id, alpha, beta, cur_player_id, depth);
	}
	else
	{
		return min_alphabeta(board, my_player_id, alpha, beta, cur_player_id, depth);
	}
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
	clock_t start_time = clock();
	struct move_t move = alphabeta(global_player.board, global_player.player_id, depth).move;
	double time_taken = (double)(clock() - start_time) / CLOCKS_PER_SEC;
	if(time_taken > max_time_per_turn)
		depth--;
	size_t num_vertices = global_player.board->num_accessible_vertices - turns;
	if (((double)num_vertices/sqrt(num_vertices)*num_vertices)*time_taken < num_vertices*max_time_per_turn)
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