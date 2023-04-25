#include "player_ext.h"
#include "tree.h"
#include <math.h>
#include <float.h>
#define SQRT_2 (1.41421356237)
#define EXPLORATION_CONSTANT (SQRT_2)

static node_t *monte_carlo_tree;

static player_t global_player;

struct transition
{
	struct move_t move;
	unsigned int player_id;
};

struct node_data
{
	int wins;
	int visits;
	struct transition transition;
};

struct node_and_player
{
	node_t *node;
	int player_id;
};

struct node_data *create_node_data(struct move_t move, unsigned int player_id)
{
	struct node_data *data = malloc(sizeof(struct node_data));
	data->wins = 0;
	data->visits = 0;
	data->transition.move = move;
	data->transition.player_id = player_id;

	return data;
}

double UCT1(node_t *parent, node_t *successor)
{
	struct node_data *parent_data = (struct node_data *)node_get_value(parent);
	struct node_data *successor_data = (struct node_data *)node_get_value(successor);

	double exploitation_rate = successor_data->wins / successor_data->visits;

	double exploration_rate = EXPLORATION_CONSTANT * sqrt(log(parent_data->visits) / (double)successor_data->visits);

	return exploitation_rate + exploration_rate;
}

node_t *get_max_child(node_t *node)
{
	double max = -DBL_MAX;
	node_t *max_child = NULL;
	array_list_t *children = node_get_children(node);
	for (size_t i = 0; i < array_list_length(children); i++)
	{
		node_t *child = (node_t *)array_list_get(children, i);
		double uct_res = UCT1(node, child);
		if (uct_res >= max)
		{
			max = uct_res;
			max_child = child;
		}
	}

	return max_child;
}

struct node_and_player selection(node_t *root)
{
	int player_id = global_player.player_id;

	for (array_list_t *children = node_get_children(root); get_winner(global_player.board) == -1 && array_list_length(children) > 0; children = node_get_children(root))
	{
		root = get_max_child(root);
		struct move_t *move = &((struct node_data *)node_get_value(root))->transition.move;
		apply_move(global_player.board, move, player_id);
		player_id = (player_id + 1) % NUM_PLAYERS;
	}
	struct node_and_player ret = {.node = root, .player_id = player_id};
	return ret;
}

void expansion(struct node_and_player selection)
{
	node_t *selected_node = selection.node;
	for (size_t i = 0; i < global_player.board->num_queens; i++)
	{
		unsigned int queen_src = global_player.board->queens[selection.player_id][i];
		position_set *queen_pos = get_reachable_positions_generic(global_player.board, queen_src);
		for (size_t j = 0; j < queen_pos->count; j++)
		{
			unsigned int queen_dst = queen_pos->positions[j];
			position_set *arrow_pos = get_reachable_arrows_generic(global_player.board, selection.player_id, queen_src, queen_dst);
			for (size_t k = 0; k < arrow_pos->count; k++)
			{
				int arrow_dst = arrow_pos->positions[k];
				struct move_t move = {queen_src, queen_dst, arrow_dst};
				node_add_child(selected_node, create_node_data(move, selection.player_id));
				i++;
			}
			free_position_set(arrow_pos);
		}
		free_position_set(queen_pos);
	}
}

/// @brief Monte Carlo Rollout
/// @param to_simulate
/// @return the id of the player that won the rollout.
unsigned int rollout(struct node_and_player to_simulate)
{
	int winner;
	int player_id = to_simulate.player_id;
	array_list_t *move_stack = array_list_init(0, free);
	while ((winner = get_winner(global_player.board)) == -1)
	{
		int queen_src_id = rand() % global_player.board->num_queens;
		unsigned int queen_src = global_player.board->queens[player_id][queen_src_id];
		position_set *set = get_reachable_positions_generic(global_player.board, queen_src);
		while (set->count == 0)
		{
			free_position_set(set);
			queen_src_id = (queen_src_id + 1) % global_player.board->num_queens;
			queen_src = global_player.board->queens[player_id][queen_src_id];
			set = get_reachable_positions_generic(global_player.board, queen_src);
		}
		int queen_dst = set->positions[rand() % set->count];
		free_position_set(set);
		set = get_reachable_arrows_generic(global_player.board, player_id, queen_src, queen_dst);
		int arrow_dst = set->positions[rand() % set->count];
		free_position_set(set);
		struct move_t *move = malloc(sizeof(struct move_t));
		*move = (struct move_t){queen_src, queen_dst, arrow_dst};
		array_list_push(move_stack, move);
		apply_move(global_player.board, move, player_id);
		player_id = (player_id + 1) % NUM_PLAYERS;
	}
	// return board to state before rollout
	for (int i = array_list_length(move_stack) - 1; i >= 0; i--)
	{
		player_id = (player_id + 1) % NUM_PLAYERS;
		cancel_move(global_player.board, (struct move_t *)array_list_get(move_stack, i), player_id);
	}
	array_list_free(move_stack);
	return (unsigned int)winner;
}

void backtrack(struct node_and_player simulated, unsigned int winner_id)
{
	node_t *cur = simulated.node;
	int result = winner_id == global_player.player_id ? 1 : -1;
	while (cur != NULL)
	{
		struct node_data *data = (struct node_data *)node_get_value(cur);
		data->visits++;
		data->wins += result;
		cur = node_get_parent(cur);
	}
}

void initialize(unsigned int player_id, struct graph_t *graph,
				unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
	global_player.player_id = player_id;
	global_player.name = "MCTS";
	global_player.board = init_board(graph, num_queens);
	for (size_t i = 0; i < NUM_PLAYERS; i++)
	{
		global_player.board->queens[i] = queens[i];
	}

	monte_carlo_tree = tree_create(create_node_data((struct move_t){-1, -1, -1}, -1), free);
}

int cmp_node_data_move(struct move_t *move, struct node_data *data)
{
	struct move_t data_mv = data->transition.move;
	if (data_mv.queen_src == move->queen_src && data_mv.queen_dst == move->queen_dst && data_mv.arrow_dst == move->arrow_dst)
		return 0;
	return 1;
}

void rollback_selection(node_t *selected_node){
	while(selected_node != NULL){
		struct node_data *data = (struct node_data*)node_get_value(selected_node);
		if(node_get_parent(selected_node) != NULL)
			cancel_move(global_player.board, &data->transition.move, data->transition.player_id);
		selected_node = node_get_parent(selected_node);
	}
}

void do_one_mcts_iteration()
{
	struct node_and_player selected_node = selection(monte_carlo_tree);
	expansion(selected_node);
	array_list_t *children = node_get_children(selected_node.node);
	for (size_t i = 0; i < array_list_length(children); i++)
	{
		node_t *child = array_list_get(children, i);
		struct node_data *data = (struct node_data*)node_get_value(child);
		apply_move(global_player.board, &data->transition.move, data->transition.player_id);
		struct node_and_player to_simulate = {.node = child, .player_id = (selected_node.player_id + 1) % NUM_PLAYERS};
		unsigned int winner = rollout(to_simulate);
		cancel_move(global_player.board, &data->transition.move, data->transition.player_id);
		backtrack(to_simulate, winner);
	}

	rollback_selection(selected_node.node);
}

struct move_t play(struct move_t previous_move)
{
	if (previous_move.queen_src != UINT_MAX && previous_move.queen_dst != UINT_MAX && previous_move.arrow_dst != UINT_MAX)
	{
		apply_move(global_player.board, &previous_move, abs((int)global_player.player_id - 1) % NUM_PLAYERS);
		node_t *corresponding_node = node_get_child(monte_carlo_tree, &previous_move, (void *)cmp_node_data_move);
		if (corresponding_node != NULL)
		{
			detach_node(corresponding_node);
		}
		else
		{
			corresponding_node = tree_create(create_node_data(previous_move, (global_player.player_id + 1) % NUM_PLAYERS), free);
		}
		tree_free(monte_carlo_tree);
		monte_carlo_tree = corresponding_node;
	}
	for(int i = 0; i<100; i++)
		do_one_mcts_iteration();
	node_t *selected = get_max_child(monte_carlo_tree);
	if (selected == NULL)
	{
		struct move_t ret = {-1, -1, -1};
		return ret;
	}
	detach_node(selected);
	tree_free(monte_carlo_tree);
	monte_carlo_tree = selected;
	struct move_t ret = ((struct node_data *)node_get_value(selected))->transition.move;
	apply_move(global_player.board, &ret, global_player.player_id);
	return ret;
}

void finalize()
{
	board_free(global_player.board);
	tree_free(monte_carlo_tree);
}

char const *get_player_name()
{
	return global_player.name;
}