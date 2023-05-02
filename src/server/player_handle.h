#ifndef PLAYER_HANDLE_H
#define PLAYER_HANDLE_H
#include "player.h"

typedef struct {
	void *dl_handle;
	char const *path;
	char const *(*get_player_name)(void);
	void (*initialize)(unsigned int, struct graph_t*,
                unsigned int, unsigned int*[NUM_PLAYERS]);
	struct move_t (*play)(struct move_t);
	void (*finalize)(void);
} player_handle_t;

/// @brief Create a player handle object
/// @param filepath the filepath
/// @return player_handle_t 
player_handle_t create_player_handle(char *filepath);

#endif