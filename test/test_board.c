#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "test_board.h"

#include "board.h"
#include "graph.h"
#include "game_impl.h"

void custom_board_free(board_t *b){
    graph_free(b->graph);
    free(b->arrows);
    free(b->queens_on_board);
    free(b);
}

void test_init_board()
{
    printf("\t%s\t", __func__);
    struct graph_t *graph = init_square_graph(10);
    board_t *b = init_board(graph, 8);
    assert(b->num_queens == 8);
    custom_board_free(b);
    printf("OK\n");
}

void test_is_on_board()
{
    printf("\t%s\t", __func__);
    struct graph_t *graph = init_square_graph(10);
    board_t *b = init_board(graph, 8);
    assert(is_on_board(b, -1) == false);
    assert(is_on_board(b, 100) == false);
    assert(is_on_board(b, 10) == true);
    assert(is_on_board(b, 0) == true);
    assert(is_on_board(b, 99) == true);
    custom_board_free(b);
    printf("OK\n");
}

void test_is_cell_empty(){
    printf("\t%s\t", __func__);
    struct graph_t *graph = init_square_graph(10);
    board_t *b = init_board(graph, 8);
    assert(is_cell_empty(b, 0) == true);
    b->queens_on_board[0] = 1;
    assert(is_cell_empty(b, 0) == false);
    custom_board_free(b);
    printf("OK\n");
}



void run_board_tests()
{
    printf("Running test file \"%s\"\n", __FILE__);
    test_init_board();
    test_is_on_board();
    test_is_cell_empty();
}