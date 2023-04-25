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
#include "colors.h"

#define USAGE_STRING "Usage: %s [-t |] [-m |] <player1.so> <player2.so>\n"

void print_usage(char *binary_name)
{
	fprintf(stdout, USAGE_STRING, binary_name);
}

server_settings_t get_args(int argc, char *const *argv)
{
	server_settings_t settings = {
		.game_type = SQUARE,
		.game_width = 5,
		.seed = time(NULL)
	};
	char opt;
	int width;
	while ((opt = getopt(argc, argv, "t:m:s:")) != -1)
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
			default:
				fprintf(stderr, "'%c' isn't a valid board shape (example: %s" GREEN " -t c " RESET "<player1.so> <player2.so>)\n", optarg[0], argv[0]);
				exit(1);
			}
			break;
		case 'm':
			width = atoi(optarg);
			if (width < 5)
			{
				fprintf(stderr, "Width should be >=5 (example: %s" GREEN " -m 10 " RESET "<player1.so> <player2.so>)\n", argv[0]);
				exit(1);
			}
			settings.game_width = width;
			break;
		case 's':
			settings.seed = atoi(optarg);
			break;
		default:
			fprintf(stderr, "'%c' isn't a valid parameter.\n", opt);
			print_usage(argv[0]);
			exit(1);
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
	server_settings_t args = get_args(argc, argv);
	srand(args.seed);
	game_t *game = init_game_and_players(args);

	struct move_t current_move = {-1, -1, -1};
	bool game_not_over = true;
	size_t turns = 0;
	while (game_not_over)
	{

		print_board(game->board);
		
		current_move = args.player_handles[game->current_player].play(current_move);
		if (!is_move_legal(game->board, &current_move, game->current_player))
		{
			game_not_over = false;
			update_player(game);
			break;
		}

		// Play the move on the board
		apply_move(game->board, &current_move, game->current_player);
		update_player(game);
		turns++;
	}

	unsigned int winner = game->current_player;
	printf("Player %u ('%s') won in %lu turns!\n", winner, args.player_handles[winner].get_player_name(), turns);
	game_free(game);
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		args.player_handles[i].finalize();
		dlclose(args.player_handles[i].dl_handle);
	}
	neighbors_cache_free();
	return 0;
}
