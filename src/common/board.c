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
}