#include "board.h"
#include "move.h"
#include "player.h"

bool is_on_board(board_t board, unsigned int position) {
    return position >= 0 && position < (board.graph->num_vertices * board.graph->num_vertices);
}

bool is_cell_empty(board_t board, unsigned int queen, unsigned int arrow) {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < board.num_queens; j++) {
            if (board.queens[i][j] == queen)
                return false;
        }
    }

    if (board.arrows[arrow] == true)
        return false;

    return true;
}

bool is_move_legal(board_t board, struct move_t move) {
    return is_on_board(board, move.queen_src) && is_on_board(board, move.queen_dst) 
            && is_on_board(board, move.arrow_dst) && is_cell_empty(board, move.queen_dst, move.arrow_dst);
            
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