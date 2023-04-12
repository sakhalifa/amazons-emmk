#ifndef _GAME_IMPL_H_
#define _GAME_IMPL_H_

#include "server.h"
#include "board.h"

void init_queens(unsigned int **queens, unsigned int num_queens, size_t width);
struct graph_t *init_square_graph(size_t width);

#endif