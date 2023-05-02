#include <dlfcn.h>
#include "player_handle.h"

player_handle_t create_player_handle(char *filepath)
{
	void *player_handle = dlopen(filepath, RTLD_LAZY);
	if (player_handle == NULL)
	{
		fprintf(stderr, "Invalid library path '%s'!\n", filepath);
		exit(EXIT_FAILURE);
	}
	printf("%s\n", filepath);
	printf("\t%p\n", player_handle);
	printf("\t%p\n", dlsym(player_handle, "get_player_name"));
	
	player_handle_t p = {
		.dl_handle = player_handle,
		.play = dlsym(player_handle, "play"),
		.finalize = dlsym(player_handle, "finalize"),
		.get_player_name = dlsym(player_handle, "get_player_name"),
		.initialize = dlsym(player_handle, "initialize"),
		.path = filepath};
	return p;
}