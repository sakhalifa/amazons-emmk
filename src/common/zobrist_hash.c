#include <stdbool.h>
#include <stdio.h>
#include "zobrist_hash.h"
#include "mt64.h"
#define NUMBER_OF_PIECES 3
#define ARROW 0

static bool zobrist_table_initialized = false;
static unsigned long long *zobrist_table[NUMBER_OF_PIECES];

void initialize_zobrist_table(unsigned long long seed, size_t num_vertices){
	zobrist_table_initialized = true;
	init_genrand64(seed);
	for(int i = 0; i<NUMBER_OF_PIECES; i++){
		zobrist_table[i] = malloc(sizeof(unsigned long long int)*num_vertices);
		for(unsigned int j = 0; j<num_vertices; j++){
			zobrist_table[i][j] = genrand64_int64();
		}
	}
}
void free_zobrist_table(){
	zobrist_table_initialized = false;
	for(int i = 0; i<NUMBER_OF_PIECES; i++){
		free(zobrist_table[i]);
	}
}

unsigned long long compute_hash(board_t *board){
	if(!zobrist_table_initialized){
		fprintf(stderr, "Call to compute_hash without initializing zobrist table...\n");
		exit(1);
	}
	unsigned long long h = 0;
	for(unsigned int i = 0; i<board->graph->num_vertices; i++){
		if(board->arrows[i]){
			h ^= zobrist_table[ARROW][i];
		}
		else if(board->queens_on_board[i] != EMPTY_CELL){
			h ^= (board->queens_on_board[i] + 1);
		}
	}
	return h;
}