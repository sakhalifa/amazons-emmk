#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <dlfcn.h>
#include "graph_ext.h"
#include "dir.h"

#include "server.h"

#define USAGE_STRING "Usage: %s [-t |] [-m |] <player1.so> <player2.so>\n"

void print_usage(char *binary_name)
{
	printf(USAGE_STRING, binary_name);
}

server_settings_t get_args(int argc, char *const *argv)
{
	server_settings_t settings = {
		.game_type = SQUARE,
		.game_width = 10,
	};
	char opt;
	int width;
	while ((opt = getopt(argc, argv, "t:m:")) != -1)
	{
		switch (opt)
		{
		case 't':
			switch (optarg[0])
			{
			case 'c':
				settings.game_type = SQUARE;
				break;
			case 'd':
				settings.game_type = DONUT;
				break;
			case 't':
				settings.game_type = CLOVER;
				break;
			case '8':
				settings.game_type = EIGHT;
				break;
			}
			break;
		case 'm':
			width = atoi(optarg);
			if (width < 5)
			{
				fprintf(stderr, "Width should be >=5");
				exit(1);
			}
			settings.game_width = width;
			break;
		}
	}
	if (argv[optind] == NULL || argv[optind + 1] == NULL)
	{
		print_usage(argv[0]);
		exit(1);
	}

	settings.player_handles[0] = create_player_handle(argv[optind]);
	settings.player_handles[1] = create_player_handle(argv[optind + 1]);
	return settings;
}

unsigned int get_starting_player_id()
{
	srand(time(NULL));
	return rand() % NUM_PLAYERS;
}

unsigned int get_other_player_id(unsigned int player_id)
{
	return player_id == 1 ? 0 : 1;
}

struct graph_t *init_square_graph(size_t width)
{
	struct graph_t *graph = malloc(sizeof(struct graph_t));
	graph->num_vertices = width * width;
	gsl_spmatrix_uint *tmp = gsl_spmatrix_uint_alloc(graph->num_vertices, graph->num_vertices);
	for (size_t pos = 0; pos < graph->num_vertices; ++pos)
	{
		enum dir_t cur_dir = DIR_NW;
		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				int new_pos = pos + dx + dy * width;
				if ((size_t)new_pos == pos || new_pos < 0 || (size_t)new_pos >= graph->num_vertices)
					continue;
				gsl_spmatrix_uint_set(tmp, pos, (size_t)new_pos, cur_dir);
				cur_dir = (cur_dir + 1) % NUM_DIRS;
			}
		}
	}
	graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
	gsl_spmatrix_uint_free(tmp);
	return graph;
}

unsigned int **init_queens(unsigned int num_queens, size_t width)
{
	unsigned int **queens = (unsigned int **)malloc(NUM_PLAYERS * sizeof(unsigned int *));

	for (int i = 0; i < NUM_PLAYERS; ++i)
	{
		queens[i] = (unsigned int *)malloc(sizeof(unsigned int) * num_queens);
	}

	int half = (width / 2) - (width % 2);
	int cur = half;
	int end = width * width - 1;
	int row = 1; // Start placing at row 1, to avoid contact with top row
	for (unsigned int i = 0; i < num_queens; i += 2)
	{
		if (cur >= 1)
		{ // Place on top (or bottom) row
			queens[0][i] = cur;
			queens[0][i + 1] = width - (cur + 1);
			queens[1][i] = end - cur;
			queens[1][i + 1] = end - (width - (cur + 1));
			cur -= 2;
		}
		else
		{ // Place on columns
			queens[0][i] = row * width;
			queens[0][i + 1] = (row + 1) * width - 1;
			queens[1][i] = end - row * width;
			queens[1][i + 1] = end - ((row + 1) * width - 1);
			row += 2;
		}
	}
	return queens;
}

struct graph_t *init_graph(game_type_t game_type, size_t width)
{
	switch (game_type)
	{
	case SQUARE:
		return init_square_graph(width);
		break;
	default:
		exit(1);
	}

	return NULL;
}

// TODO : define him better
void init_game_and_players(server_settings_t settings)
{

	struct graph_t *graph = init_graph(settings.game_type, settings.game_width);
	unsigned int num_queens = 4 * (settings.game_width / 10 + 1);
	unsigned int **queens = init_queens(num_queens, settings.game_width);

	unsigned int starting_player_id = get_starting_player_id();
	settings.player_handles[starting_player_id]
		.initialize(starting_player_id, graph, num_queens, queens);
	unsigned int other_player_id = get_other_player_id(starting_player_id);
	settings.player_handles[starting_player_id]
		.initialize(other_player_id, graph, num_queens, queens);
}

int main(int argc, char *const *argv)
{

	unsigned int **queens = init_queens(4, 5);
	for (int pid = 0; pid < NUM_PLAYERS; ++pid)
	{
		for (int i = 0; i < 4; ++i)
		{
			printf("%d ", queens[pid][i]);
		}
		printf("\n");
	}

	server_settings_t args = get_args(argc, argv);

	for (int i = 0; i < NUM_PLAYERS; i++)
		dlclose(args.player_handles[i].dl_handle);
	return 0;
}
