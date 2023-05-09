#include "test_distance.h"
#include "board_distance_table.h"
#include "game_impl.h"
#include "graph.h"
#include <assert.h>

void test_distance_lookup_table(){
    printf("\t%s\t", __func__);
	init_distance_lookup_table(init_square_graph(5));
	unsigned int distance[25] = 
	{
		0,1,1,1,1,
		1,1,2,2,2,
		1,2,1,2,2,
		1,2,2,1,2,
		1,2,2,2,1
	};
	compute_distance_lookup_table();
	unsigned int p1 = 0;
	for(unsigned int p2 = 0; p2 < 25; p2++){
		assert(distance[p2] == get_distance(p1, p2));
	}
	printf("OK\n");
}


void run_distance_tests(){
    printf("Running test file \"%s\"\n", __FILE__);
	test_distance_lookup_table();

}