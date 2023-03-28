#include <stdio.h>
#include <stdlib.h>

#include "server_ext.h"



void structural_test_init_queens(){
    printf("%s\t", __func__);
    unsigned int **queens = init_queens(4, 6);

    printf("OK\n");
}

int test(int argc, char **argv){
    printf("Starting Server Tests\n");
    structural_test_init_queens();
    exit(EXIT_SUCCESS); //use this to prevent obscure segfault
}

