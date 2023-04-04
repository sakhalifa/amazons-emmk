#include "board.h"

bool is_on_board(board_t board, unsigned int position) {
    return position >= 0 && position < (board.graph->num_vertices * board.graph->num_vertices);
}