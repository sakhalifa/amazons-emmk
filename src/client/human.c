#include "player_ext.h"
#include <stdio.h>

static player_t global_player;

void initialize(unsigned int player_id, struct graph_t *graph,
				unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
	generic_initialize(&global_player, player_id, graph, num_queens, queens, "Human");
}

void show_queens()
{
	for (size_t i = 0; i < global_player.board->num_queens; i++)
	{
		printf("- %lu. \t%u\n", i, global_player.board->queens[global_player.player_id][i]);
	}
}

void clear_input_buffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

/// @brief Scans an integer
/// @param min Minimum value for the int (included)
/// @param max Maximum value for integer (not included)
/// @param display_prompt Function to display the prompt
/// @return The scanned int
int scan_int(int min, int max, void (*display_prompt)(void))
{
	int num = max;
	while (num < min || num >= max)
	{
		display_prompt();
		// Remove warning...
		int ignored = scanf("%d", &num);
		(void)(ignored);
		clear_input_buffer();
	}
	return num;
}

void display_queen_src_prompt()
{
	show_queens();
	printf("Please choose a queen\n");
}

void display_queen_dst_prompt()
{
	print_board(global_player.board);
	printf("Please choose a valid destination for your queen\n");
}

void display_arrow_dst_prompt()
{
	print_board(global_player.board);
	printf("Please choose a valid destination for your arrow\n");
}

struct move_t play(struct move_t previous_move)
{
	if (previous_move.queen_src != FIRST_MOVE_VAL && previous_move.queen_dst != FIRST_MOVE_VAL && previous_move.arrow_dst != FIRST_MOVE_VAL)
	{
		apply_move(global_player.board, &previous_move, abs((int)global_player.player_id - 1) % NUM_PLAYERS);
	}
	print_board(global_player.board);
	int queen_src_id = scan_int(0, global_player.board->num_queens, display_queen_src_prompt);

	unsigned int queen_src = global_player.board->queens[global_player.player_id][queen_src_id];

	unsigned int queen_dst = scan_int(0, global_player.board->graph->num_vertices, display_queen_dst_prompt);

	apply_queen_move(global_player.board, global_player.player_id, queen_src, queen_dst);

	unsigned int arrow_dst = scan_int(0, global_player.board->graph->num_vertices, display_arrow_dst_prompt);

	cancel_queen_move(global_player.board, global_player.player_id, queen_src, queen_dst);

	struct move_t ret = {queen_src, queen_dst, arrow_dst};

	apply_move(global_player.board, &ret, global_player.player_id);

	return ret;
}

char const *get_player_name()
{
	return global_player.name;
}

void finalize()
{
	board_free(global_player.board);
}