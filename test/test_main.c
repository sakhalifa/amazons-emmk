#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test_game.h"
#include "test_position_set.h"
#include "test_distance.h"

int main(){
    run_game_tests();
    run_position_set_test();
    run_distance_tests();
    return EXIT_SUCCESS;
}