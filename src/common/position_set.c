#include <stdio.h>

#include "position_set.h"


position_set* init_position_set(size_t capacity) {
    position_set* to_return = (position_set*) malloc(sizeof(position_set));
    to_return->positions = (unsigned int*) malloc(sizeof(unsigned int) * capacity);
    to_return->count = 0;
    return to_return;
}

void free_position_set(position_set *position_set){
    free(position_set->positions);
    position_set->positions = NULL;
    free(position_set);
}

void add_position(position_set* set, unsigned int position) {
    set->positions[set->count] = position;
    set->count++;
}

void print_position_set(position_set* set) {
    printf("[ ");
    for (size_t i = 0; i < set->count; ++i) {
        printf("%u ", set->positions[i]);
    }
    printf("]\n");
}