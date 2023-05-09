#include "board_distance_table.h"
#include "linked_list.h"
#include "array_list.h"
#include <string.h>

typedef struct
{
	unsigned short *distance_lookup_table;
	size_t num_vertices;
	board_t *empty_board;
} distance_table_t;

static distance_table_t distance_table = {NULL, 0, NULL};

void init_distance_lookup_table(struct graph_t *graph)
{
	if (distance_table.distance_lookup_table != NULL)
	{
		free_distance_lookup_table();
	}
	distance_table.num_vertices = graph->num_vertices;
	distance_table.distance_lookup_table = malloc(sizeof(unsigned short) * graph->num_vertices * graph->num_vertices);
	memset(distance_table.distance_lookup_table, UCHAR_MAX, sizeof(unsigned short) * graph->num_vertices * graph->num_vertices);
	distance_table.empty_board = init_board(graph, 0);
}

void set_distance(unsigned short value, unsigned int p1, unsigned int p2)
{
	if (p1 > distance_table.num_vertices || p2 > distance_table.num_vertices)
	{
		fprintf(stderr, "Warning, OOB access at %s:%u\n", __FILE__, __LINE__);
		return;
	}
	*((distance_table.distance_lookup_table + p1 * distance_table.num_vertices) + p2) = value;
}

struct pos_and_dist
{
	unsigned int pos;
	unsigned short dist;
};

struct pos_and_dist *create_pos_and_dist(unsigned int p, unsigned short d)
{
	struct pos_and_dist *ret = malloc(sizeof(struct pos_and_dist));
	ret->pos = p;
	ret->dist = d;
	return ret;
}

void do_bfs(linked_list_t *queue, unsigned int p1)
{
	while (linked_list_length(queue) > 0)
	{
		struct pos_and_dist *p_and_d = linked_list_get_last(queue);
		if (p_and_d->dist < get_distance(p1, p_and_d->pos))
			set_distance(p_and_d->dist, p1, p_and_d->pos);
		position_set *pos_set = get_reachable_positions_generic(distance_table.empty_board, p_and_d->pos);
		for (unsigned int i = 0; i < pos_set->count; i++)
		{
			if (p_and_d->dist + 1 < get_distance(p1, pos_set->positions[i]))
			{
				linked_list_queue(queue, create_pos_and_dist(pos_set->positions[i], p_and_d->dist + 1));
			}
		}
		linked_list_remove_last(queue);
	}
}

void compute_distance_lookup_table()
{
	// For each position
	for (unsigned int p = 0; p < distance_table.num_vertices; p++)
	{
		linked_list_t *queue = linked_list_create(free);
		linked_list_queue(queue, create_pos_and_dist(p, 0));
		// Do bfs from position
		do_bfs(queue, p);
		linked_list_free(queue);
	}
}

unsigned short get_distance(unsigned int p1, unsigned int p2)
{
	return *((distance_table.distance_lookup_table + p1 * distance_table.num_vertices) + p2);
}
void free_distance_lookup_table(void)
{
	free(distance_table.distance_lookup_table);
	distance_table.distance_lookup_table = NULL;
	distance_table.num_vertices = 0;
}