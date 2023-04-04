#include <stdio.h>
#include "move.h"
#include "player_ext.h"

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name(){
	return "p1";
}

/* Player initialization
 * PARAM:
 * - player_id:    the id of the player, between 0 and NUM_PLAYERS-1
 * - num_vertices: the number of vertices of the graph
 * - graph:        a matrix of size (num_vertices*num_vertices) in CSR
 *                 format representing the edges of the graph, such that
 *                 graph[row*num_vertices+col] is the edge from row to col
 *                 (NO_DIR, DIR_NORTH, DIR_SOUTH, ...)
 * - num_queens:   the number of queens per player
 * - queens:       an array containing, for each player, an array of length
 *                 `num_queens`, giving the ids of the positions of the queens
 * PRECOND:
 * - `id` is either 0 or 1
 * - `graph` is heap-allocated and it is up to you (the player) to
 *    free it before the end of the program
 * - each array in `queens` is heap-allocated and it is up to you (the
 *   player) to free them all before the end of the progra
 */
void initialize(unsigned int player_id, struct graph_t* graph,
                unsigned int num_queens, unsigned int* queens[NUM_PLAYERS]){

				}

/* Computes next move
 * PARAM:
 * - previous_move: the move from the previous player. If this is the
 *                  first move of the game, `previous_move` is equal to
 *                  { UINT_MAX, UINT_MAX, UINT_MAX }
 * RETURNS: - the next move for the player.
 */
struct move_t play(struct move_t previous_move){
	struct move_t move = {-1, -1, -1};
	return move;
}

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize(){
	printf("coucou player1\n");
}