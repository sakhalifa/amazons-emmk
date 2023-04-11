#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test_server.h"

int main(){
    printf("Starting Server Tests\n");
    structural_test_init_queens();
    running_test_init_game_and_player();
    return EXIT_SUCCESS;
}