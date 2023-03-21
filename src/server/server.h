#ifndef SERVER_H
#define SERVER_H
#include <stddef.h>
#include "player_handle.h"

typedef enum {
	SQUARE,
	DONUT,
	CLOVER,
	EIGHT
} game_type_t;

typedef struct {
	size_t game_width;
	game_type_t game_type;
	player_handle_t player_handles[NUM_PLAYERS];
} server_settings_t;

#endif