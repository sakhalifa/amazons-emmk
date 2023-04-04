#include "board.h"

bool is_on_board(board_t board, unsigned int position) {
    return position >= 0 && position < (board.graph->num_vertices * board.graph->num_vertices);
}

board_t* init_board(struct graph_t* graph, unsigned int num_queens, unsigned int* queens[NUM_PLAYERS]) {
    board_t* board = (board_t*) malloc(sizeof(board_t));
    unsigned int** qu = queens;
    board->graph = graph;
    board->num_queens = num_queens;
    board->queens = queens;
    board->arrows = (bool*) malloc(sizeof(bool) * graph->num_vertices);
    for (int i = 0; i < graph->num_vertices; ++i) {
        board->arrows[i] = false;
    }
    return board;
}