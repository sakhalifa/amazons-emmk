#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test_game.h"

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
    printf("\t%s\t", __func__);
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

void running_test_init_game_and_player_on_board(game_type_t board_type, int game_width){
    server_settings_t settings = {
        .game_type = board_type,
        .game_width = game_width,
    };
    settings.player_handles[0] = get_mock_player();
    settings.player_handles[1] = get_mock_player();

    game_t *game = init_game_and_players(settings);

    unsigned int starting_player = game->current_player;
    update_player(game);
    assert(game->current_player != starting_player);
    game_free(game);
}

void running_test_init_game_and_player_on_square_board()
{
    printf("\t%s\t", __func__);
    running_test_init_game_and_player_on_board(SQUARE, 10);
    printf("OK\n");
}

void running_test_init_game_and_player_on_clover_board()
{
    printf("\t%s\t", __func__);
    running_test_init_game_and_player_on_board(CLOVER, 15);
    printf("OK\n");
}

void running_test_init_game_and_player_on_donut_board()
{
    printf("\t%s\t", __func__);
    running_test_init_game_and_player_on_board(DONUT, 12);
    printf("OK\n");
}

void running_test_init_game_and_player_on_eight_board()
{
    printf("\t%s\t", __func__);
    running_test_init_game_and_player_on_board(EIGHT, 12);
    printf("OK\n");
}

void test_init_square()
{
    printf("\t%s\t", __func__);

    struct graph_t *graph = init_square_graph(2);
    int size = 4;
    unsigned int adj_mtrx[4][4] = {
        {NO_DIR, DIR_EAST, DIR_SOUTH, DIR_SE},
        {DIR_WEST, NO_DIR, DIR_SW, DIR_SOUTH},
        {DIR_NORTH, DIR_NE, NO_DIR, DIR_EAST},
        {DIR_NW, DIR_NORTH, DIR_WEST, NO_DIR}};

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            unsigned int gsl_val = gsl_spmatrix_uint_get(graph->t, i, j);
            if (adj_mtrx[i][j] != gsl_val)
            {
                printf("\n\tExpected %u, got %u at i=%d j=%d\n", adj_mtrx[i][j], gsl_val, i, j);
                return;
            }
        }
    }

    graph_free(graph);
    size = 9;
    graph = init_square_graph(3);
    unsigned int adj_mtrx2[9][9] = {
        {NO_DIR, DIR_EAST, NO_DIR, DIR_SOUTH, DIR_SE, NO_DIR, NO_DIR, NO_DIR, NO_DIR},
        {DIR_WEST, NO_DIR, DIR_EAST, DIR_SW, DIR_SOUTH, DIR_SE, NO_DIR, NO_DIR, NO_DIR},
        {NO_DIR, DIR_WEST, NO_DIR, NO_DIR, DIR_SW, DIR_SOUTH, NO_DIR, NO_DIR, NO_DIR},
        {DIR_NORTH, DIR_NE, NO_DIR, NO_DIR, DIR_EAST, NO_DIR, DIR_SOUTH, DIR_SE, NO_DIR},
        {DIR_NW, DIR_NORTH, DIR_NE, DIR_WEST, NO_DIR, DIR_EAST, DIR_SW, DIR_SOUTH, DIR_SE},
        {NO_DIR, DIR_NW, DIR_NORTH, NO_DIR, DIR_WEST, NO_DIR, NO_DIR, DIR_SW, DIR_SOUTH},
        {NO_DIR, NO_DIR, NO_DIR, DIR_NORTH, DIR_NE, NO_DIR, NO_DIR, DIR_EAST, NO_DIR},
        {NO_DIR, NO_DIR, NO_DIR, DIR_NW, DIR_NORTH, DIR_NE, DIR_WEST, NO_DIR, DIR_EAST},
        {NO_DIR, NO_DIR, NO_DIR, NO_DIR, DIR_NW, DIR_NORTH, NO_DIR, DIR_WEST, NO_DIR}
    };

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            unsigned int gsl_val = gsl_spmatrix_uint_get(graph->t, i, j);
            if (adj_mtrx2[i][j] != gsl_val)
            {
                printf("\n\tExpected %u, got %u at i=%d j=%d\n", adj_mtrx2[i][j], gsl_val, i, j);
                return;
            }
        }
    }
    graph_free(graph);


    printf("OK\n");
}

void run_game_tests()
{
    printf("Running test file \"%s\"\n", __FILE__);
    running_test_init_game_and_player_on_square_board();
    running_test_init_game_and_player_on_clover_board();
    running_test_init_game_and_player_on_donut_board();
    running_test_init_game_and_player_on_eight_board();
    structural_test_init_queens();
    test_init_square();
}
