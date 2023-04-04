#ifndef _GAME_H_
#define _GAME_H_

#include "server.h"
#include "board.h"

typedef struct {
	unsigned int current_player;
    board_t *board;
} game_t;


game_t init_game_and_players(server_settings_t settings);
void update_player(game_t *game);
unsigned int get_other_player_id(unsigned int player_id);
unsigned int **init_queens(unsigned int num_queens, size_t width);

#endif