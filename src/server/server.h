#ifndef SERVER_H
#define SERVER_H
#include <stddef.h>
#include "player_handle.h"

typedef struct {
	size_t game_width;
	char game_type;
	player_handle_t player_handles[NUM_PLAYERS];
} server_settings_t;

#endif