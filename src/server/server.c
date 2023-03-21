#include <stdio.h>
#include <getopt.h>
#include "server.h"

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
	while ((opt = getopt(argc, argv, "t:m:")) != -1)
	{
		switch(opt){
			case 't':
				switch(optarg[0]){
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
				int width = atoi(optarg);
				if(width < 5){
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
	settings.player_handles[1] = create_player_handle(argv[optind+1]);
	return settings;
}

int main(int argc, char *const *argv)
{
	server_settings_t args = get_args(argc, argv);
	return 0;
}
