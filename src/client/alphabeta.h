#ifndef __ALPHABETA_H__
#define __ALPHABETA_H__
#include "board.h"


struct move_and_score
{
	struct move_t move;
	int score;
};
struct move_and_score alphabeta(board_t *board, int my_player_id, int depth);

#endif