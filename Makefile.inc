async_mcts.o: src/client/async_mcts.c src/client/player_ext.h \
 src/common/player.h src/common/move.h src/common/graph.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h src/common/tree.h \
 src/common/util.h src/common/array_list.h
alphabeta.o: src/client/alphabeta.c src/common/move.h src/common/board.h \
 src/common/dir.h src/common/graph_ext.h src/common/graph.h \
 src/common/move.h src/common/player.h src/common/position_set.h \
 src/client/player_ext.h src/common/player.h
player.o: src/client/player.c src/client/player_ext.h src/common/player.h \
 src/common/move.h src/common/graph.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h \
 src/common/position_set.h
afk.o: src/client/afk.c src/client/player_ext.h src/common/player.h \
 src/common/move.h src/common/graph.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h
position_set.o: src/common/position_set.c src/common/position_set.h
board.o: src/common/board.c src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/graph.h src/common/move.h \
 src/common/player.h src/common/position_set.h src/common/colors.h
dir.o: src/common/dir.c src/common/dir_ext.h src/common/dir.h
util.o: src/common/util.c src/common/util.h
tree.o: src/common/tree.c src/common/tree.h src/common/util.h \
 src/common/array_list.h
array_list.o: src/common/array_list.c src/common/array_list.h \
 src/common/util.h
graph.o: src/common/graph.c src/common/graph.h src/common/graph_ext.h
server.o: src/server/server.c src/common/graph_ext.h src/common/graph.h \
 src/common/dir.h src/common/move.h src/server/server.h \
 src/server/player_handle.h src/common/player.h src/common/move.h \
 src/server/game.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h \
 src/common/colors.h
game.o: src/server/game.c src/common/graph_ext.h src/common/graph.h \
 src/common/dir_ext.h src/common/dir.h src/server/server.h \
 src/server/player_handle.h src/common/player.h src/common/move.h \
 src/server/game.h src/common/board.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h
player_handle.o: src/server/player_handle.c src/server/player_handle.h \
 src/common/player.h src/common/move.h src/common/graph.h
test_thread.o: test_thread.c
profiling.o: profiling.c src/common/player.h src/common/move.h \
 src/common/graph.h src/common/graph.h src/server/game.h \
 src/server/server.h src/server/player_handle.h src/common/board.h \
 src/common/dir.h src/common/graph_ext.h src/common/player.h \
 src/common/position_set.h
test_game.o: test/test_game.c test/test_game.h test/game_impl.h \
 src/server/server.h src/server/player_handle.h src/common/player.h \
 src/common/move.h src/common/graph.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h \
 src/server/game.h src/server/server.h src/server/player_handle.h \
 test/mock_player.h
mock_player.o: test/mock_player.c src/server/player_handle.h \
 src/common/player.h src/common/move.h src/common/graph.h \
 src/common/move.h src/common/graph_ext.h
test_main.o: test/test_main.c test/test_game.h
