#ifndef __BOARD_DISTANCE_TABLE_H__
#define __BOARD_DISTANCE_TABLE_H__
#include "board.h"

void init_distance_lookup_table(struct graph_t *graph);
void compute_distance_lookup_table();
unsigned short get_distance(unsigned int p1, unsigned int p2);
void free_distance_lookup_table(void);

#endif