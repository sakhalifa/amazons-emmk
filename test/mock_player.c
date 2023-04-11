#include "player_handle.h"
#include "move.h"
#define UNUSED(x) (void)(x)


struct move_t mock_play(struct move_t move){
    UNUSED(move);
    struct move_t mock_move = { -1, -1, -1};
    return mock_move;
};

static char const *PLAYER_NAME = "Mock Player";

char const *mock_get_player_name(void){
    return PLAYER_NAME;
};

void mock_finalize(void){
}

void mock_initialize(unsigned int width, 
                struct graph_t* graph,
                unsigned int num_queens, 
                unsigned int* queens[NUM_PLAYERS] ){
    UNUSED(width);
    UNUSED(graph);
    UNUSED(num_queens);
    UNUSED(queens);
}

player_handle_t get_mock_player(){
    player_handle_t p = {
        .dl_handle = NULL,
        .play = mock_play,
        .finalize = mock_finalize,
        .initialize = mock_initialize,
        
    };
    return p;
}
