#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include "graph.h"

#include "server.h"


#define USAGE_STRING "Usage: %s [-t |] [-m |] <player1.so> <player2.so>\n"

void print_usage(char *binary_name){
	printf(USAGE_STRING, binary_name);
}

server_settings_t get_args(int argc, char *const *argv){
	
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
	unsigned int* queens[NUM_PLAYERS];

	unsigned int starting_player_id = get_starting_player_id();
	settings.player_handles[starting_player_id]
		.initialize(starting_player_id, graph, num_queens, queens);
	unsigned int other_player_id = get_other_player_id()
	settings.player_handles[starting_player_id]
		.initialize(other_player_id, graph, num_queens, queens);

}

int main(int argc, char *const *argv)
{
	
	return 0;
}
