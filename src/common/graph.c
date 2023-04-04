#include "graph.h"
#include "graph_ext.h"

struct graph_t *copy_graph(struct graph_t *source){
	struct graph_t *cpy = malloc(sizeof(struct graph_t));
	cpy->num_vertices = source->num_vertices;
	gsl_spmatrix_uint* tmp = gsl_spmatrix_uint_alloc(source->t->size1, source->t->size2);
	cpy->t = gsl_spmatrix_uint_compress(tmp, GSL_SPMATRIX_CSR);
	gsl_spmatrix_uint_free(tmp);
	gsl_spmatrix_uint_memcpy(cpy->t, source->t);
	return cpy;
}

void graph_free(struct graph_t* graph){
	gsl_spmatrix_uint_free(graph->t);
	free(graph);
}