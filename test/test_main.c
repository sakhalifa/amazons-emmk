#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test_game.h"
#include "test_board.h"

int main(){
    run_game_tests();
    run_board_tests();
    return EXIT_SUCCESS;
}