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
		if ((int)pos - 1 >= 0)
			gsl_spmatrix_uint_set(tmp, pos, pos - 1, DIR_WEST);
		if (pos + 1 < graph->num_vertices)
			gsl_spmatrix_uint_set(tmp, pos, pos + 1, DIR_EAST);
		if ((int)(pos - width) >= 0)
			gsl_spmatrix_uint_set(tmp, pos, pos - width, DIR_NORTH);
		if (pos + width < graph->num_vertices)
			gsl_spmatrix_uint_set(tmp, pos, pos + width, DIR_SOUTH);
	}
	graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
	gsl_spmatrix_uint_free(tmp);
	return graph;
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
	unsigned int** queens = init_queens(num_queens, settings.game_width);

	unsigned int starting_player_id = get_starting_player_id();
	settings.player_handles[starting_player_id]
		.initialize(starting_player_id, &graph, num_queens, queens);
	unsigned int other_player_id = get_other_player_id(starting_player_id);
	settings.player_handles[starting_player_id]
		.initialize(other_player_id, &graph, num_queens, queens);

}

int main(int argc, char *const *argv)
{
	server_settings_t args = get_args(argc, argv);

	for (int i = 0; i < NUM_PLAYERS; i++)
		dlclose(args.player_handles[i].dl_handle);
	return 0;
}

