#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test_game.h"
#include "test_board.h"
#include "test_tree.h"
#include "test_array_list.h"

int main(){
    run_game_tests();
    run_board_tests();
    test_trees();
    test_array();
    return EXIT_SUCCESS;
}