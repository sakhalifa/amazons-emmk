#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <dlfcn.h>
#include "graph_ext.h"
#include "dir.h"
#include "server.h"
#include "player.h"

#include "server_init.h"

#include "board.h"

/// @brief change the game's current player.
void update_player(game_t *game){
	game->current_player = get_other_player_id(game->current_player);
}

unsigned int get_starting_player_id()
{
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
    unsigned int **queens = (unsigned int **)malloc(NUM_PLAYERS * num_queens * sizeof(unsigned int *));
    for (int player_id = 0; player_id < NUM_PLAYERS; ++player_id){
        queens[player_id] = (unsigned int *) malloc(sizeof(unsigned int) * num_queens);
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

/// @brief Init the game, the board, the graph, the queens and calls initialize functions of players
/// @returns A game structure representing the current game.
game_t init_game_and_players(server_settings_t settings)
{

	game_t game;

	// Init board
	struct graph_t *graph = init_graph(settings.game_type, settings.game_width);
	unsigned int num_queens = 4 * (settings.game_width / 10 + 1);
	unsigned int **queens = init_queens(num_queens, settings.game_width);

	game.board = init_board(graph, num_queens, queens);

	// Init players
	unsigned int starting_player_id = get_starting_player_id();
	settings.player_handles[starting_player_id]
		.initialize(starting_player_id, graph, num_queens, queens);
	
	unsigned int other_player_id = get_other_player_id(starting_player_id);
	settings.player_handles[starting_player_id]
		.initialize(other_player_id, graph, num_queens, queens);

	game.current_player = starting_player_id;

	return game;

}