#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "test_position_set.h"

#include "position_set.h"


void test_init_position_set(){
    printf("\t%s\t", __func__);
    position_set* set = init_position_set(50);
    assert(set->count == 0);
    free_position_set(set);
    printf("OK\n");
}

void test_free_position_set(){
    printf("\t%s\t", __func__);
    position_set * set = init_position_set(50);
    free_position_set(set);
    printf("OK\n");
}

void test_add_position(){
    printf("\t%s\t", __func__);
    position_set * set = init_position_set(50);
    assert(set->count == 0);
    add_position(set, 1);
    assert(set->positions[0] == 1);
    assert(set->count == 1);
    add_position(set, 10);
    assert(set->positions[1] == 10);
    assert(set->count == 2);
    add_position(set, 1);
    assert(set->positions[2] == 1);
    assert(set->count == 3);
    add_position(set, 100);
    assert(set->positions[3] == 100);
    assert(set->count == 4);
    add_position(set, 0);
    assert(set->positions[4] == 0);
    assert(set->count == 5);
    free_position_set(set);
    printf("OK\n");
}

void test_remove_position_at_index(){
    printf("\t%s\t", __func__);
    position_set * set = init_position_set(50);
    add_position(set, 1);
    add_position(set, 2);
    add_position(set, 3);
    add_position(set, 4);
    add_position(set, 5);
    assert(set->count == 5);
    remove_position_at_index(set, 2);
    assert(set->count == 4);
    remove_position_at_index(set, 2);
    assert(set->count == 3);
    remove_position_at_index(set, 2);
    assert(set->count == 2);
    add_position(set, 5);
    assert(set->count == 3);
    add_position(set, 5);
    assert(set->count == 4);
    remove_position_at_index(set, 0);
    assert(set->count == 3);
    remove_position_at_index(set, 1);
    assert(set->count == 2);
    free_position_set(set);
    printf("OK\n");
}

void run_position_set_test(){
    printf("Running test file \"%s\"\n", __FILE__);
    test_init_position_set();
    test_free_position_set();
    test_add_position();
    test_remove_position_at_index();
}
