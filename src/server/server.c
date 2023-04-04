#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <dlfcn.h>
#include "graph_ext.h"
#include "dir.h"
#include "move.h"

#include "server.h"
#include "game.h"

#define USAGE_STRING "Usage: %s [-t |] [-m |] <player1.so> <player2.so>\n"

void print_usage(char *binary_name)
{
	printf(USAGE_STRING, binary_name);
}

server_settings_t get_args(int argc, char *const *argv)
{
	server_settings_t settings = {
		.game_type = SQUARE,
		.game_width = 10,
	};
	char opt;
	int width;
	while ((opt = getopt(argc, argv, "t:m:")) != -1)
	{
		switch (opt)
		{
		case 't':
			switch (optarg[0])
			{
			case 'c':
				settings.game_type = SQUARE;
				break;
			case 'd':
				settings.game_type = DONUT;
				break;
			case 't':
				settings.game_type = CLOVER;
				break;
			case '8':
				settings.game_type = EIGHT;
				break;
			}
			break;
		case 'm':
			width = atoi(optarg);
			if (width < 5)
			{
				fprintf(stderr, "Width should be >=5");
				exit(1);
			}
			settings.game_width = width;
			break;
		}
	}
	if (argv[optind] == NULL || argv[optind + 1] == NULL)
	{
		print_usage(argv[0]);
		exit(1);
	}

	settings.player_handles[0] = create_player_handle(argv[optind]);
	settings.player_handles[1] = create_player_handle(argv[optind + 1]);
	return settings;
}

int main(int argc, char *const *argv)
{
	srand(time(NULL));
	server_settings_t args = get_args(argc, argv);
	game_t *game = init_game_and_players(args);

	struct move_t current_move = { -1, -1, -1 };
	bool game_not_over = true;
	while (game_not_over){
		
		current_move = args.player_handles[game->current_player].play(current_move);
		
		if (! is_move_legal(game->board, &current_move)){
			game_not_over = false;
			update_player(game);
			break;
		}

		// Play the move on the board
		apply_move(game->board, &current_move, game->current_player);

		update_player(game);
	}

	unsigned int winner = game->current_player;
	printf("%s won!\n", args.player_handles[winner].get_player_name());
	game_free(game);
	for (int i = 0; i < NUM_PLAYERS; i++)
		dlclose(args.player_handles[i].dl_handle);

	return 0;
}

