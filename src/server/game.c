#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <dlfcn.h>
#include "graph_ext.h"
#include "dir.h"
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

void set_square_matrix_cardinal_directions(gsl_spmatrix_uint* direction_matrix, size_t width, size_t num_vertices) {
	for (size_t vertex = width; vertex < num_vertices; ++vertex) {
		gsl_spmatrix_uint_set(direction_matrix, vertex, vertex - width, DIR_NORTH);
	}
	for (size_t vertex = 0; vertex < num_vertices - width; ++vertex) {
		gsl_spmatrix_uint_set(direction_matrix, vertex, vertex + width, DIR_SOUTH);
	}
	for (size_t row = 0; row < width; ++row) {
		for (size_t col = 0; col < width - 1; ++col) {
			size_t vertex = col + row * width;
			gsl_spmatrix_uint_set(direction_matrix, vertex, vertex + 1, DIR_EAST);
		}
		for (size_t col = 1; col < width; ++col) {
			size_t vertex = col + row * width;
			gsl_spmatrix_uint_set(direction_matrix, vertex, vertex - 1, DIR_WEST);
		}
	}
}

void set_square_matrix_diagonal_directions(gsl_spmatrix_uint* direction_matrix, size_t width) {
	for (size_t row = 0; row < width - 1; ++row) {
		for (size_t col = 0; col < width - 1; ++col) {
			size_t vertex = col + row * width;
			gsl_spmatrix_uint_set(direction_matrix, vertex, vertex + width + 1, DIR_SE);
		}
	}
	for (size_t row = 1; row < width; ++row) {
		for (size_t col = 0; col < width - 1; ++col) {
			size_t vertex = col + row * width;
			gsl_spmatrix_uint_set(direction_matrix, vertex, vertex - width + 1, DIR_NE);
		}
	}
	for (size_t row = 0; row < width - 1; ++row) {
		for (size_t col = 1; col < width; ++col) {
			size_t vertex = col + row * width;
			gsl_spmatrix_uint_set(direction_matrix, vertex, vertex + width - 1, DIR_SW);
		}
	}
	for (size_t row = 1; row < width; ++row) {
		for (size_t col = 1; col < width; ++col) {
			size_t vertex = col + row * width;
			gsl_spmatrix_uint_set(direction_matrix, vertex, vertex - width - 1, DIR_NW);
		}
	}
}

gsl_spmatrix_uint* allocate_COO_square_direction_matrix(size_t width, size_t num_vertices) {
	gsl_spmatrix_uint* direction_matrix = gsl_spmatrix_uint_alloc(num_vertices, num_vertices);
	set_square_matrix_cardinal_directions(direction_matrix, width, num_vertices);
	set_square_matrix_diagonal_directions(direction_matrix, width);
	return direction_matrix;
}

struct graph_t *init_square_graph(size_t width)
{
	struct graph_t *graph = malloc(sizeof(struct graph_t));
	graph->num_vertices = width * width;
	gsl_spmatrix_uint* tmp = allocate_COO_square_direction_matrix(width, graph->num_vertices);
	graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
	gsl_spmatrix_uint_free(tmp);
	return graph;
}

bool pointInRectangle(size_t lx, size_t ly, size_t rx, size_t ry, size_t x, size_t y)
{
    if (x >= lx && x <= rx && y >= ly && y <= ry) {
		return true;
	}
    return false;
}

void remove_all_dir_neighbors_in_square_grid(gsl_spmatrix_uint* neighbor_matrix, size_t removed_vertex, size_t width) {
	for (int horizontal_offset = -1; horizontal_offset <= 1; ++horizontal_offset) {
		for (int vertical_offset = -1; vertical_offset <= 1; ++vertical_offset) {
			int total_offset = horizontal_offset + width * vertical_offset;
			gsl_spmatrix_uint_set(neighbor_matrix, removed_vertex + total_offset, removed_vertex, NO_DIR);
			gsl_spmatrix_uint_set(neighbor_matrix, removed_vertex, removed_vertex + total_offset, NO_DIR);
		}
	}
}

struct graph_t *init_donut_graph(size_t width)
{
	struct graph_t *graph = malloc(sizeof(struct graph_t));
	graph->num_vertices = width * width;
	gsl_spmatrix_uint* tmp = allocate_COO_square_direction_matrix(width, graph->num_vertices);
	for (size_t row = width / 3; row < 2 * width / 3; ++row) {
		for (size_t col = width / 3; col < 2 * width / 3; ++col) {
			size_t vertex = col + row * width;
			remove_all_dir_neighbors_in_square_grid(tmp, vertex, width);
		}
	}
	graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
	gsl_spmatrix_uint_free(tmp);
	return graph;
}

struct graph_t *init_clover_graph(size_t width)
{
	struct graph_t *graph = malloc(sizeof(struct graph_t));
	graph->num_vertices = width * width;
	gsl_spmatrix_uint* tmp = allocate_COO_square_direction_matrix(width, graph->num_vertices);
	for (size_t row_zone = 0; row_zone <= 1; ++row_zone) {
		for (size_t row = (row_zone * 2 + 1) * (width / 5); row < (row_zone * 2 + 2) * (width / 5); ++row) {
			for (size_t col_zone = 0; col_zone <= 1; ++col_zone) {
				for (size_t col = (col_zone * 2 + 1) * (width / 5); col < (col_zone * 2 + 2) * (width / 5); ++col) {
					size_t vertex = col + row * width;
					remove_all_dir_neighbors_in_square_grid(tmp, vertex, width);
				}
			}
		}
	}
	graph->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
	gsl_spmatrix_uint_free(tmp);
	return graph;
}

struct graph_t *init_eight_graph(size_t width)
{
	struct graph_t *graph = malloc(sizeof(struct graph_t));
	graph->num_vertices = width * width;
	gsl_spmatrix_uint* tmp = allocate_COO_square_direction_matrix(width, graph->num_vertices);
	for (size_t row_zone = 1, col_zone = 2; row_zone <= 2; ++row_zone, --col_zone) {
		for (size_t row = row_zone * width / 4; row < (row_zone + 1) * width / 4; ++row) {
			for (size_t col = col_zone * width / 4; col < (col_zone + 1) * width / 4; ++col) {
				size_t vertex = col + row * width;
				remove_all_dir_neighbors_in_square_grid(tmp, vertex, width);
			}
		}
	}
	gsl_spmatrix_uint_set(tmp, width / 2, width / 2, DIR_NW);
	gsl_spmatrix_uint_set(tmp, width / 2 - 1, width / 2 - 1, DIR_SE);
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
			exit(EXIT_FAILURE);
		}
		return init_donut_graph(width);
	case CLOVER:
		if (width % 5 != 0)
		{
			fprintf(stderr, "For a CLOVER graph, you need m mod 5 == 0\n");
			exit(EXIT_FAILURE);
		}
		return init_clover_graph(width);
	case EIGHT:
		if (width % 4 != 0)
		{
			fprintf(stderr, "For an EIGHT graph, you need m mod 4 == 0\n");
			exit(EXIT_FAILURE);
		}
		return init_eight_graph(width);
	default:
		fprintf(stderr, "The game type %d isn't handled.\n", game_type);
		exit(EXIT_FAILURE);
	}

	return NULL;
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