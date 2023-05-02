#include "array_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
//TODO Refactor

void free_nothing(void *a){
    (void)(a);
}

void test_no_memory_leak(void){
    printf("\t%s\t", __func__);
    array_list_t *zero_array = array_list_init(0, free);
    for (int i = 0; i < 20; i++)
    {
        int *number = malloc(sizeof(int));
        array_list_push(zero_array, number);
    }

    array_list_free(zero_array);
    printf("OK\n");
}

void test_array(void)
{
	printf("Running test file \"%s\"\n", __FILE__);
    test_no_memory_leak();
}
