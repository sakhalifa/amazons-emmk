#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "graph.h"
#include "player.h"
#include "board.h"

typedef struct {
    unsigned int player_id;
    char const name[64];
    board_t* board;
} player_t;


typedef struct {
    struct graph_t* graph;
    unsigned int* queens[NUM_PLAYERS];
    unsigned int num_queens;
    bool* arrows;
} board_t;

static player_t global_player;

board_t* init_board(struct graph_t* graph, unsigned int num_queens, unsigned int* queens[NUM_PLAYERS]) {
    board_t* board = (board_t*) malloc(sizeof(board_t));
    board->graph = graph;
    board->num_queens = num_queens;
    board->queens = queens;
    board->arrows = (bool*) malloc(sizeof(bool) * graph->num_vertices);
    for (int i = 0; i < graph->num_vertices; ++i) {
        board->arrows[i] = false;
    }
    return board;
}

void initialize(unsigned int player_id, struct graph_t* graph,
                unsigned int num_queens, unsigned int* queens[NUM_PLAYERS]) {
    global_player.player_id = player_id;
    strcpy(global_player.name, "bot");
    global_player.board = init_board(graph, num_queens, queens);
}

char const* get_player_name() {
    return global_player.name;
}