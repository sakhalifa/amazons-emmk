#include "position_set.h"



position_set* init_position_set(size_t capacity) {
    position_set* to_return = (position_set*) malloc(sizeof(position_set));
    to_return->positions = (unsigned int*) malloc(sizeof(unsigned int) * capacity);
    to_return->count = 0;
    return to_return;
}

void add_position(position_set* set, unsigned int position) {
    set->positions[set->count] = position;
    set->count++;
}