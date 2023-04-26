#ifndef _GAME_H_
#define _GAME_H_

#include "server.h"
#include "board.h"

typedef struct {
	unsigned int current_player;
    board_t *board;
} game_t;


/// @brief Updates the adjacency matrix neighbor_matrix so that removed_vertex has no neighbors and no neighbors has removed_vertex as a neighbor.
///		   Neighbors of all cardinal and diagonal directions are considered, so removed_vertex musn't be on the edge of the matrix.
/// @param neighbor_matrix the adjacency matrix containing the directions in which neighbors can be accessed
/// @param removed_vertex the vertex that the function disconnects from all other neighbor
/// @param width the width of the square grid represented by the adjacency matrix
void remove_all_dir_neighbors_in_square_grid(gsl_spmatrix_uint* neighbor_matrix, size_t removed_vertex, size_t width);

game_t *init_game(unsigned int current_player, board_t *board);
game_t *init_game_and_players(server_settings_t settings);
void update_player(game_t *game);
unsigned int get_other_player_id(unsigned int player_id);

void game_free(game_t *game);

#endif