#ifndef SERVER_H
#define SERVER_H
#include <stddef.h>
#include "player.h"

typedef struct {
	size_t game_width;
	char game_type;
	void *player_handles[NUM_PLAYERS];
} server_settings_t;

#endif