#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "game.h"

void structural_test_init_queens(){
    printf("%s\t", __func__);

    unsigned int **queens = init_queens(4, 6);
    assert(queens[0][0] == 1);
    assert(queens[0][1] == 4);
    assert(queens[0][2] == 6);
    assert(queens[0][3] == 11);
    assert(queens[1][0] == 34);
    assert(queens[1][1] == 31);
    assert(queens[1][2] == 29);
    assert(queens[1][3] == 24);

    queens = init_queens(4, 5);
    assert(queens[0][0] == 1);
    assert(queens[0][1] == 3);
    assert(queens[0][2] == 5);
    assert(queens[0][3] == 9);
    assert(queens[1][0] == 23);
    assert(queens[1][1] == 21);
    assert(queens[1][2] == 19);
    assert(queens[1][3] == 15);
    printf("OK\n");
}

int main(int argc, char **argv){
    printf("Starting Server Tests\n");
    structural_test_init_queens();
    exit(EXIT_SUCCESS); //use this to prevent obscure segfault
}

