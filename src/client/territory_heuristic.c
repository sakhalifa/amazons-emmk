#include "heuristic.h"
#include "linked_list.h"
#include <limits.h>

struct marker
{
	unsigned short D[NUM_PLAYERS];
};

bool is_marked(struct marker *marker)
{
	return marker->D[0] != USHRT_MAX || marker->D[1] != USHRT_MAX;
}

static inline unsigned int *copy_int(unsigned int i)
{
	unsigned int *m_i = malloc(sizeof(unsigned int));
	*m_i = i;
	return m_i;
}

struct marker *get_dist_table(board_t *board)
{
	struct marker *table = malloc(sizeof(struct marker) * board->graph->num_vertices);
	for (unsigned int i = 0; i < board->graph->num_vertices; i++)
	{
		for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
		{
			table[i].D[player_id] = USHRT_MAX;
		}
	}
	linked_list_t *queue = linked_list_create(free);

	for (unsigned int queen_id = 0; queen_id < board->num_queens; queen_id++)
	{
		for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
		{
			unsigned int queen_pos = board->queens[player_id][queen_id];
			table[queen_pos].D[player_id] = 0;
			linked_list_queue(queue, copy_int(queen_pos));
		}
	}

	while (linked_list_length(queue) > 0)
	{
		unsigned int queue_length = linked_list_length(queue);
		// Step 1, expand from "queens"
		for (unsigned int useless = 0; useless < queue_length; useless++)
		{
			unsigned int queen_pos = *((unsigned int *)linked_list_get_last(queue));
			position_set *reachable_pos = get_reachable_positions_generic(board, queen_pos);
			for (unsigned int pos_id = 0; pos_id < reachable_pos->count; pos_id++)
			{
				unsigned int pos = reachable_pos->positions[pos_id];
				if (is_marked(table + pos))
					continue;
				for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
				{
					table[pos].D[player_id] = table[queen_pos].D[player_id] + 1;
				}
			}
			free_position_set(reachable_pos);
			linked_list_remove_last(queue);
		}

		// Step 2, for each unmarked pos, check if it's possible to reach a marked move.
		for (unsigned int pos = 0; pos < board->graph->num_vertices; pos++)
		{
			if (is_marked(table + pos))
				continue;
			if (board->arrows[pos] || board->queens_on_board[pos] != EMPTY_CELL)
				continue;
			position_set *reachable_pos = get_reachable_positions_generic(board, pos);
			for (unsigned int pos_id = 0; pos_id < reachable_pos->count; pos_id++)
			{
				unsigned int p = reachable_pos->positions[pos_id];
				for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
				{
					if (table[p].D[player_id] < table[pos].D[player_id])
						table[pos].D[player_id] = table[p].D[player_id];
				}
			}
			if (reachable_pos->count == 0)
			{
				for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
				{
					table[pos].D[player_id] = USHRT_MAX - 1;
				}
			}
			free_position_set(reachable_pos);
			linked_list_push(queue, copy_int(pos));
		}
	}

	return table;
}

int get_score(board_t *board, int player_id)
{
	int my_score = 0;
	int other_score = 0;
	int other_player_id = (player_id + 1) % 2;
	struct marker *dist_table = get_dist_table(board);
	for (unsigned int i = 0; i < board->graph->num_vertices; i++)
	{
		struct marker mark = dist_table[i];
		if (mark.D[player_id] < mark.D[other_player_id])
			my_score += mark.D[player_id] - mark.D[other_player_id];
		if (mark.D[other_player_id] < mark.D[player_id])
			other_score += mark.D[other_player_id] - mark.D[player_id];
	}

	return my_score - other_score;
}