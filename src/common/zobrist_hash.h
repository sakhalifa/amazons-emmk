#ifndef __ZOBRIST_HASH_H__
#define __ZOBRIST_HASH_H__
#include "board.h"

void initialize_zobrist_table(size_t num_vertices);

void free_zobrist_table();

unsigned long long int compute_hash(board_t *board);

#endif