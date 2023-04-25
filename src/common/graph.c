#include "graph.h"
#include "graph_ext.h"

struct graph_t *copy_graph(struct graph_t *source){
	struct graph_t *cpy = malloc(sizeof(struct graph_t));
	cpy->num_vertices = source->num_vertices;
	cpy->t = gsl_spmatrix_uint_alloc_nzmax(source->t->size1, source->t->size2, source->t->nzmax, GSL_SPMATRIX_CSR);
	gsl_spmatrix_uint_memcpy(cpy->t, source->t);
	return cpy;
}

void graph_free(struct graph_t* graph){
	gsl_spmatrix_uint_free(graph->t);
	free(graph);
}