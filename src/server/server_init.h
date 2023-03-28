#ifndef _SERVER_INIT_H_
#define _SERVER_INIT_H_

#include "server.h"

unsigned int get_starting_player_id();
unsigned int get_other_player_id(unsigned int player_id);
struct graph_t *init_square_graph(size_t width);
unsigned int **init_queens(unsigned int num_queens, size_t width);
struct graph_t *init_graph(game_type_t game_type, size_t width);
void init_game_and_players(server_settings_t settings);

#endif