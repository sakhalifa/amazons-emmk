#include <dlfcn.h>
#include "player_handle.h"

player_handle_t create_player_handle(char *filepath){
	void *player_handle = dlopen(filepath, RTLD_LAZY);
	if (player_handle == NULL)
	{
		fprintf(stderr, "Invalid library path '%s'!\n", filepath);
		exit(1);
	}
	player_handle_t p = {
		.dl_handle = player_handle,
		.play = dlsym(player_handle, "play"),
		.finalize = dlsym(player_handle, "finalize"),
		.get_player_name = dlsym(player_handle, "get_player_name"),
		.initialize = dlsym(player_handle, "initialize")};
	return p;
}