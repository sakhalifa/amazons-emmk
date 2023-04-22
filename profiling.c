#include "player.h"
#include "graph.h"
#include "game.h"
#include "board.h"

void init_queens(unsigned int **queens, unsigned int num_queens, size_t width);

struct graph_t *init_square_graph(size_t width);

int main(void){
	size_t width = 10;
	struct graph_t *graph = init_square_graph(width);
	unsigned int num_queens = 4 * (width / 10 + 1);
	board_t *board = init_board(graph, num_queens, width);
	init_queens(board->queens, num_queens, width);

	initialize(0, graph, num_queens, board->queens);
	struct move_t prev_move = {-1, -1, -1};
	play(prev_move);
	return 0;
}