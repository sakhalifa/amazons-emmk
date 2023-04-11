#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "game_impl.h"
#include "game.h"

#include "player_handle.h"
#include "mock_player.h"

void queen_free(unsigned int **queens)
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        free(queens[i]);
    }
}

void structural_test_init_queens()
{
    printf("%s\t", __func__);
    unsigned int *queens[NUM_PLAYERS];
    init_queens(queens, 4, 5);
    assert(queens[0][0] == 1);
    assert(queens[0][1] == 3);
    assert(queens[0][2] == 5);
    assert(queens[0][3] == 9);
    assert(queens[1][0] == 23);
    assert(queens[1][1] == 21);
    assert(queens[1][2] == 19);
    assert(queens[1][3] == 15);
    queen_free(queens);

    init_queens(queens, 4, 6);
    assert(queens[0][0] == 1);
    assert(queens[0][1] == 4);
    assert(queens[0][2] == 6);
    assert(queens[0][3] == 11);
    assert(queens[1][0] == 34);
    assert(queens[1][1] == 31);
    assert(queens[1][2] == 29);
    assert(queens[1][3] == 24);
    queen_free(queens);

    printf("OK\n");
}

void running_test_init_game_and_player()
{
    server_settings_t settings = {
        .game_type = SQUARE,
        .game_width = 2,
    };
    settings.player_handles[0] = get_mock_player();
    settings.player_handles[1] = get_mock_player();

    game_t *game = init_game_and_players(settings);

    unsigned int adj_mtrx[4][4] = {
        {NO_DIR, DIR_EAST, DIR_SOUTH, DIR_SE},
        {DIR_WEST, NO_DIR, DIR_SW, DIR_SOUTH},
        {DIR_NORTH, DIR_NE, NO_DIR, DIR_EAST},
        {DIR_NW, DIR_NORTH, DIR_WEST, NO_DIR}};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            unsigned int gsl_val = gsl_spmatrix_uint_get(game->board->graph->t, i, j);
            if (adj_mtrx[i][j] != gsl_val)
            {
                printf("Expected %u, got %u at i=%d j=%d\n", adj_mtrx[i][j], gsl_val, i, j);
            }
        }
    }

    game_free(game);
}
