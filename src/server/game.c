#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <dlfcn.h>
#include "graph_ext.h"
#include "dir_ext.h"
#include "server.h"
#include "player.h"

#include "game.h"

#include "board.h"

/// @brief change the game's current player.
void update_player(game_t *game)
{
	game->current_player = get_other_player_id(game->current_player);
}

unsigned int get_starting_player_id()
{
	return rand() % NUM_PLAYERS;
}

unsigned int get_other_player_id(unsigned int player_id)
{
	return (player_id + 1) % NUM_PLAYERS;
}

void add_if_correct(size_t width, gsl_spmatrix_uint *mtrx, size_t x, size_t y, int dx, int dy)
{
	if (dx == 0 && dy == 0)
		return;
	if ((int)(x + dx) < 0 || x + dx >= width)
		return;
	if ((int)(y + dy) < 0 || y + dy >= width)
		return;
	int i = x + y * width;
	int j = (x + dx) + (y + dy) * width;
	gsl_spmatrix_uint_set(mtrx, i, j, twoD_offset_to_dir(dx, dy));
}

struct graph_t *init_square_graph(size_t width)
{
	struct graph_t *graph = malloc(sizeof(struct graph_t));
	graph->num_vertices = width * width;
	gsl_spmatrix_uint *tmp = gsl_spmatrix_uint_alloc(graph->num_vertices, graph->num_vertices);
	for (size_t i = 0; i < graph->num_vertices; i++)
	{
		size_t x = i % width;
		size_t y = i / width;
		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				add_if_correct(width, tmp, x, y, dx, dy);
			}
		}
	}
	graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
	gsl_spmatrix_uint_free(tmp);
	return graph;
}

void init_queens(unsigned int **queens, unsigned int num_queens, size_t width)
{
	for (int player_id = 0; player_id < NUM_PLAYERS; ++player_id)
	{
		queens[player_id] = (unsigned int *)malloc(sizeof(unsigned int) * num_queens);
	}

	int half = ((width - 1) / 2) - 1;
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
}

bool pointInRectangle(size_t lx, size_t ly, size_t rx, size_t ry, size_t x, size_t y)
{
    if (x >= lx && x <= rx && y >= ly && y <= ry)
        return true;
 
    return false;
}

struct graph_t *init_donut_graph(size_t width)
{
	struct graph_t *graph = malloc(sizeof(struct graph_t));
	graph->num_vertices = width * width;
	size_t sq_size = (width / 3) - 1;
	int offset = (int)(width / 6) - 1;
	offset = offset < 0 ? 0 : offset;
	size_t lx = (width / 2) - 1 - offset;
	size_t ly = (width / 2) - 1 - offset;
	size_t rx = lx + sq_size;
	size_t ry = ly + sq_size;
	gsl_spmatrix_uint *tmp = gsl_spmatrix_uint_alloc(graph->num_vertices, graph->num_vertices);
	for (size_t i = 0; i < graph->num_vertices; i++)
	{
		size_t x = i % width;
		size_t y = i / width;
		if(pointInRectangle(lx, ly, rx, ry, x, y))
			continue;
		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				if(pointInRectangle(lx, ly, rx, ry, x + dx, y + dy))
					continue;
				add_if_correct(width, tmp, x, y, dx, dy);
			}
		}
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
	case DONUT:
		if (width % 3 != 0)
		{
			fprintf(stderr, "For a DONUT graph, you need m mod 3 == 0\n");
			exit(1);
		}
		return init_donut_graph(width);
	default:
		exit(1);
	}

	return NULL;
}

game_t *init_game(unsigned int current_player, board_t *board)
{
	game_t *game = malloc(sizeof(game_t));
	game->board = board;
	game->current_player = current_player;

	return game;
}

/// @brief Init the game, the board, the graph, the queens and calls initialize functions of players
/// @returns A game structure representing the current game.
game_t *init_game_and_players(server_settings_t settings)
{
	// Init board
	struct graph_t *graph = init_graph(settings.game_type, settings.game_width);
	unsigned int num_queens = 4 * (settings.game_width / 10 + 1);
	board_t *board = init_board(graph, num_queens);
	init_queens(board->queens, num_queens, settings.game_width);
	place_queens_on_board(board, board->queens, num_queens);

	// Init players
	unsigned int starting_player_id = get_starting_player_id();
	unsigned int current_player_id = starting_player_id;
	do
	{
		unsigned int *queens[NUM_PLAYERS];
		init_queens(queens, num_queens, settings.game_width);
		settings.player_handles[current_player_id]
			.initialize(current_player_id, copy_graph(graph), num_queens, queens);
		current_player_id = get_other_player_id(current_player_id);
	} while (current_player_id != starting_player_id);

	return init_game(starting_player_id, board);
}

void game_free(game_t *game)
{
	board_free(game->board);
	free(game);
}