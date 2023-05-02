#include "test_board.h"
#include "board.h"
#include "graph.h"
#include "game_impl.h"
#include <stdio.h>
#include <assert.h>

void free_board_without_queens(board_t *board)
{
	graph_free(board->graph);
	free(board->arrows);
	free(board->queens_on_board);
	free(board);
}

void test_init_board()
{
	printf("\t%s\t", __func__);

	const size_t WIDTH = 5;
	struct graph_t *graph = init_square_graph(WIDTH);
	board_t *board = init_board(graph, 4);
	assert(board->graph == graph);
	assert(board->arrows != NULL);
	assert(board->num_queens == 4);
	assert(board->queens != NULL);
	assert(board->queens_on_board != NULL);
	free_board_without_queens(board);
	printf("OK\n");
}

void test_place_queens_on_board()
{
	printf("\t%s\t", __func__);
	const size_t WIDTH = 5;
	struct graph_t *graph = init_square_graph(WIDTH);
	board_t *board = init_board(graph, 4);
	unsigned int *queens[NUM_PLAYERS];
	unsigned int queens0[4] = {0, 1, 2, 3};
	unsigned int queens1[4] = {4, 5, 6, 7};
	queens[0] = queens0;
	queens[1] = queens1;
	place_queens_on_board(board, queens, 4);
	for (size_t i = 0; i < graph->num_vertices; i++)
	{
		if (i < 4)
		{
			assert(board->queens_on_board[i] == 0);
		}
		else if (i < 8)
		{
			assert(board->queens_on_board[i] == 1);
		}
		else
		{
			assert(board->queens_on_board[i] == EMPTY_CELL);
		}
	}
	free_board_without_queens(board);
	printf("OK\n");
}

void test_is_on_board()
{
	printf("\t%s\t", __func__);

	const size_t WIDTH = 5;
	struct graph_t *graph = init_square_graph(WIDTH);
	board_t *board = init_board(graph, 4);
	assert(!is_on_board(board, -1));
	assert(!is_on_board(board, WIDTH * WIDTH));
	for (size_t pos = 0; pos < graph->num_vertices; pos++)
	{
		assert(is_on_board(board, pos));
	}

	free_board_without_queens(board);
	printf("OK\n");
}

void test_all_cell_empty_empty_board()
{
	board_t *board = init_board(init_square_graph(10), 0);
	for (size_t pos = 0; pos < board->graph->num_vertices; pos++)
	{
		assert(is_cell_empty(board, pos));
	}
	free_board_without_queens(board);
}

void test_all_cell_not_empty_full_board()
{
	board_t *board = init_board(init_square_graph(10), 0);
	for (size_t pos = 0; pos < board->graph->num_vertices; pos++)
	{
		board->arrows[pos] = true;
	}

	for (size_t pos = 0; pos < board->graph->num_vertices; pos++)
	{
		assert(!is_cell_empty(board, pos));
	}
	free_board_without_queens(board);
}

void test_queen_cells_not_empty()
{
	board_t *board = init_board(init_square_graph(10), 0);
	unsigned int *queens[NUM_PLAYERS];
	unsigned int queens0[4] = {0, 1, 2, 3};
	unsigned int queens1[4] = {4, 5, 6, 7};
	queens[0] = queens0;
	queens[1] = queens1;
	place_queens_on_board(board, queens, 4);
	for (size_t pos = 0; pos < board->graph->num_vertices; pos++)
	{
		assert(is_cell_empty(board, pos) == !(pos < 8));
	}
	free_board_without_queens(board);
}

void test_is_cell_empty()
{
	printf("\t%s\t", __func__);
	test_all_cell_empty_empty_board();
	test_all_cell_not_empty_full_board();
	test_queen_cells_not_empty();
	printf("OK\n");
}

void test_move_oob()
{
	board_t *board = init_board(init_square_graph(5), 4);
	unsigned int *queens[NUM_PLAYERS];
	unsigned int queens0[4] = {5, 1, 3, 9};
	unsigned int queens1[4] = {15, 21, 23, 19};
	queens[0] = queens0;
	queens[1] = queens1;
	for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
	{
		struct move_t move = {queens[player_id][0], -1, 0};
		assert(!is_move_legal(board, &move, player_id));
	}
	free_board_without_queens(board);
}

void test_arrow_oob()
{
	board_t *board = init_board(init_square_graph(5), 4);
	unsigned int *queens[NUM_PLAYERS];
	unsigned int queens0[4] = {5, 1, 3, 9};
	unsigned int queens1[4] = {15, 21, 23, 19};
	queens[0] = queens0;
	queens[1] = queens1;
	for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
	{
		struct move_t move = {queens[player_id][0], 11, -1};
		assert(!is_move_legal(board, &move, player_id));
	}
	free_board_without_queens(board);
}

void test_move_non_empty(){
	board_t *board = init_board(init_square_graph(5), 4);
	unsigned int *queens[NUM_PLAYERS];
	unsigned int queens0[4] = {1, 3, 5, 9};
	unsigned int queens1[4] = {15, 21, 23, 19};
	queens[0] = queens0;
	queens[1] = queens1;
	board->arrows[10] = true;
	//Move to queen
	for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
	{
		struct move_t move = {queens[player_id][0], queens[player_id][1], 11};
		assert(!is_move_legal(board, &move, player_id));
	}
	//Move to arrow
	for (unsigned int player_id = 0; player_id < NUM_PLAYERS; player_id++)
	{
		struct move_t move = {queens[player_id][0], queens[player_id][1], 11};
		assert(!is_move_legal(board, &move, player_id));
	}
	free_board_without_queens(board);
}

void test_is_move_legal()
{
	printf("\t%s\t", __func__);
	test_move_oob();
	test_arrow_oob();
	test_move_non_empty();
	printf("OK\n");
}

void run_board_tests()
{
	printf("Running test file \"%s\"\n", __FILE__);
	test_is_on_board();
	test_init_board();
	test_place_queens_on_board();
	test_is_cell_empty();
	test_is_move_legal();
}
