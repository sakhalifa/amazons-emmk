#ifndef _POSITION_SET_H_
#define _POSITION_SET_H_

#include <stdlib.h>

typedef struct {
    unsigned int* positions;
    size_t count;
} position_set;

position_set* init_position_set(size_t capacity);
void add_position(position_set* set, unsigned int position);
void free_position_set(position_set *position_set);
void print_position_set(position_set* set);

#endif // _POSITION_SET_H_