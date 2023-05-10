segdumb.o: src/client/segdumb.c src/common/player.h src/common/move_ext.h \
 src/common/move.h src/common/graph.h src/client/player_ext.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h src/common/move.h \
 src/common/position_set.h
async_mcts.o: src/client/async_mcts.c src/client/player_ext.h \
 src/common/player.h src/common/move_ext.h src/common/move.h \
 src/common/graph.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h \
 src/common/tree.h src/common/util.h src/common/array_list.h
alphabeta.o: src/client/alphabeta.c src/client/heuristic.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/graph.h src/common/move_ext.h src/common/move.h \
 src/common/player.h src/common/position_set.h src/client/alphabeta.h
function_deepening.o: src/client/function_deepening.c \
 src/common/move_ext.h src/common/move.h src/client/player_ext.h \
 src/common/player.h src/common/move_ext.h src/common/graph.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h src/client/alphabeta.h
time_deepening.o: src/client/time_deepening.c src/client/alphabeta.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/graph.h src/common/move_ext.h src/common/move.h \
 src/common/player.h src/common/position_set.h src/common/move_ext.h \
 src/client/player_ext.h src/common/player.h
trickery.o: src/client/trickery.c src/common/player.h \
 src/common/move_ext.h src/common/move.h src/common/graph.h \
 src/common/move_ext.h
player.o: src/client/player.c src/client/player_ext.h src/common/player.h \
 src/common/move_ext.h src/common/move.h src/common/graph.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h src/common/position_set.h
variable_heuristic.o: src/client/variable_heuristic.c \
 src/client/heuristic.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/graph.h src/common/move_ext.h \
 src/common/move.h src/common/player.h src/common/position_set.h
afk.o: src/client/afk.c src/client/player_ext.h src/common/player.h \
 src/common/move_ext.h src/common/move.h src/common/graph.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h
player_ext.o: src/client/player_ext.c src/client/player_ext.h \
 src/common/player.h src/common/move_ext.h src/common/move.h \
 src/common/graph.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h \
 src/common/graph.h
human.o: src/client/human.c src/client/player_ext.h src/common/player.h \
 src/common/move_ext.h src/common/move.h src/common/graph.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h
territory_heuristic.o: src/client/territory_heuristic.c \
 src/client/heuristic.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/graph.h src/common/move_ext.h \
 src/common/move.h src/common/player.h src/common/position_set.h \
 src/common/linked_list.h
linked_list.o: src/common/linked_list.c src/common/linked_list.h
position_set.o: src/common/position_set.c src/common/position_set.h
board.o: src/common/board.c src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/graph.h src/common/move_ext.h \
 src/common/move.h src/common/player.h src/common/position_set.h \
 src/common/colors.h
zobrist_hash.o: src/common/zobrist_hash.c src/common/zobrist_hash.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/graph.h src/common/move_ext.h src/common/move.h \
 src/common/player.h src/common/position_set.h src/lib/mt64.h
util.o: src/common/util.c src/common/util.h
tree.o: src/common/tree.c src/common/tree.h src/common/util.h \
 src/common/array_list.h
array_list.o: src/common/array_list.c src/common/array_list.h \
 src/common/util.h
graph.o: src/common/graph.c src/common/graph.h src/common/graph_ext.h
mt19937-64.o: src/lib/mt19937-64.c src/lib/mt64.h
server.o: src/server/server.c src/common/graph_ext.h src/common/graph.h \
 src/common/dir.h src/common/move_ext.h src/common/move.h \
 src/server/server.h src/server/player_handle.h src/common/player.h \
 src/common/move_ext.h src/server/game.h src/common/board.h \
 src/common/dir.h src/common/graph_ext.h src/common/player.h \
 src/common/position_set.h src/common/colors.h
game.o: src/server/game.c src/common/graph_ext.h src/common/graph.h \
 src/common/dir.h src/server/server.h src/server/player_handle.h \
 src/common/player.h src/common/move_ext.h src/common/move.h \
 src/server/game.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h
player_handle.o: src/server/player_handle.c src/server/player_handle.h \
 src/common/player.h src/common/move_ext.h src/common/move.h \
 src/common/graph.h
test_thread.o: test_thread.c
profiling.o: profiling.c src/common/player.h src/common/move_ext.h \
 src/common/move.h src/common/graph.h src/common/graph.h \
 src/server/game.h src/server/server.h src/server/player_handle.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h
test_game.o: test/test_game.c test/test_game.h test/game_impl.h \
 src/server/server.h src/server/player_handle.h src/common/player.h \
 src/common/move_ext.h src/common/move.h src/common/graph.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h src/server/game.h \
 src/server/server.h src/server/player_handle.h test/mock_player.h
test_position_set.o: test/test_position_set.c test/test_position_set.h \
 src/common/position_set.h
test_board.o: test/test_board.c test/test_board.h src/common/board.h \
 src/common/dir.h src/common/graph_ext.h src/common/graph.h \
 src/common/move_ext.h src/common/move.h src/common/player.h \
 src/common/position_set.h src/common/graph.h test/game_impl.h \
 src/server/server.h src/server/player_handle.h src/common/player.h
mock_player.o: test/mock_player.c src/server/player_handle.h \
 src/common/player.h src/common/move_ext.h src/common/move.h \
 src/common/graph.h src/common/move_ext.h src/common/graph_ext.h
common.o: test/common.c test/common.h
test_main.o: test/test_main.c test/test_game.h test/test_position_set.h \
 test/test_board.h
