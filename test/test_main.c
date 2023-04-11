#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test_server.h"

int main(int argc, char **argv){
    printf("Starting Server Tests\n");
    structural_test_init_queens();
    return EXIT_SUCCESS;
}