#ifndef _SERVER_INIT_H_
#define _SERVER_INIT_H_

#include "server.h"

typedef struct {
	unsigned int current_player;
    board_t board;
} game_t;


game_t init_game_and_players(server_settings_t settings);
unsigned int update_player(game_t *game)

#endif