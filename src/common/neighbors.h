#ifndef _NEIGHBORS_H_
#define _NEIGHBORS_H_

#include "dir.h"

typedef struct  {
    unsigned int neighbors[NUM_DIRS];
    unsigned int num_neighbors;
} neighbors_t;

#endif