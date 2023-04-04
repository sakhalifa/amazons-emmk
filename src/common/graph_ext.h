#ifndef _GRAPH_EXT_H_
#define _GRAPH_EXT_H_
#include "graph.h"

/// @brief Will 
/// @param source The source graph. HAS TO BE CSR
/// @return new allocated graph
struct graph_t *copy_graph(struct graph_t *source);

void graph_free(struct graph_t* graph);
#endif