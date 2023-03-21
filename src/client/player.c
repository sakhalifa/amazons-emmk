#include <stdio.h>
#include "player.h"

typedef struct {
    unsigned int player_id;
    char name[64];
    board_t board;
} player_t;

static player_t player;

void initialize(unsigned int player_id, struct graph_t* graph,
                unsigned int num_queens, unsigned int* queens[NUM_PLAYERS]) {
    player.player_id = player_id;
    player. 
}