#include "player_ext.h"
#include "tree.h"
#include <math.h>
#include <float.h>
#define SQRT_2 (1.41421356237)
#define EXPLORATION_CONSTANT (SQRT_2)

struct node_data
{
	int wins;
	int visits;
	// Figure out what to store for state...
};

double UCT1(node_t *parent, node_t *successor)
{
	struct node_data *parent_data = (struct node_data *)node_get_data(parent);
	struct node_data *successor_data = (struct node_data *)node_get_data(successor);

	double exploitation_rate = successor_data->wins / successor_data->visits;

	double exploration_rate = EXPLORATION_CONSTANT * sqrt(log(parent_data->visits) / (double)successor_data->visits);

	return exploitation_rate + exploration_rate;
}

node_t *selection(node_t *root)
{
	for (array_list_t *children = node_get_children(root); array_list_length(children) > 0; children = node_get_children(root))
	{
		double max = DBL_MIN;
		node_t *max_child = NULL;
		for (size_t i = 0; i < array_list_length(children); i++)
		{
			node_t *child = (node_t *)array_list_get(children, i);
			double uct_res = UCT1(root, child);
			if (uct_res > max)
			{
				max = uct_res;
				max_child = child;
			}
		}
		root = max_child;
	}
	return root;
}
