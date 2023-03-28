#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include "graph.h"

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


unsigned int** init_queens(unsigned int num_queens, size_t width){
	unsigned int** queens = (unsigned int **) malloc(num_queens * NUM_PLAYERS * sizeof(unsigned int));
	
	int half = (width / 2) - (1 - width % 2);
	int cur = half;
	int end = width * width - 1;
	int row = 0;
	for (unsigned int i = 0; i < num_queens / 2; ++i){
		if (cur >= 0){ // Place on top (or bottom) row
			queens[0][i] = cur;
			queens[0][i] = width - cur;
			queens[1][i] = end - cur;
			queens[1][i] = end - (width - cur);
			cur -= 2;
		} else { // Place on columns
			queens[0][i] = row * width;
			queens[0][i] = (row + 1) * width - 1;
			queens[1][i] = end - row * width;
			queens[1][i] = end - ((row + 1) * width - 1);
			row += 2;
		}
	}
	return queens;
}


unsigned int get_starting_player_id(){
	srand(time(NULL));
	return rand() % NUM_PLAYERS;
}

unsigned int get_other_player_id(unsigned int player_id){
	return player_id == 1 ? 0 : 1;
}

// TODO : define him better
void init_game_and_players(server_settings_t settings){

	struct graph_t graph;
	unsigned int num_queens = 4 * (settings.game_width / 10 + 1);
	unsigned int** queens = init_queens(num_queens, settings.game_width);

	unsigned int starting_player_id = get_starting_player_id();
	settings.player_handles[starting_player_id]
		.initialize(starting_player_id, &graph, num_queens, queens);
	unsigned int other_player_id = get_other_player_id(starting_player_id);
	settings.player_handles[starting_player_id]
		.initialize(other_player_id, &graph, num_queens, queens);

}

int main(int argc, char *const *argv)
{
	server_settings_t args = get_args(argc, argv);

	for(int i =0; i<NUM_PLAYERS; i++)
		dlclose(args.player_handles[i].dl_handle);
	return 0;
}
