#include "player.h"
#include "move_ext.h"
#include <unistd.h>

char const *get_player_name()
{
	return "We do a little trolling";
}

void initialize(unsigned int player_id, struct graph_t *graph,
				unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
	(void)(player_id);
	(void)(graph);
	(void)(num_queens);
	(void)(queens);
	sleep(20);
}

struct move_t play(struct move_t previous_move)
{
	(void)(previous_move);
	return FIRST_MOVE;
}

void finalize() {}
