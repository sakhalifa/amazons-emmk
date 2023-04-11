test.o: test.c
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
test_server.o: test/test_server.c src/server/game_impl.h \
 src/server/server.h src/server/player_handle.h src/common/player.h \
 src/common/move.h src/common/graph.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h \
 src/server/game.h src/server/player_handle.h test/mock_player.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
mock_player.o: test/mock_player.c src/server/player_handle.h \
 src/common/player.h src/common/move.h src/common/graph.h \
 src/common/move.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
test_main.o: test/test_main.c test/test_server.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
position_set.o: src/common/position_set.c src/common/position_set.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
graph.o: src/common/graph.c src/common/graph.h src/common/graph_ext.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
board.o: src/common/board.c src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/graph.h src/common/move.h \
 src/common/player.h src/common/position_set.h src/common/colors.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
game.o: src/server/game.c src/common/graph_ext.h src/common/graph.h \
 src/common/dir.h src/server/server.h src/server/player_handle.h \
 src/common/player.h src/common/move.h src/server/game.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
server.o: src/server/server.c src/common/graph_ext.h src/common/graph.h \
 src/common/dir.h src/common/move.h src/server/server.h \
 src/server/player_handle.h src/common/player.h src/common/move.h \
 src/server/game.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
player_handle.o: src/server/player_handle.c src/server/player_handle.h \
 src/common/player.h src/common/move.h src/common/graph.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
afk.o: src/client/afk.c src/client/player_ext.h src/common/player.h \
 src/common/move.h src/common/graph.h src/common/position_set.h \
 src/common/board.h src/common/dir.h src/common/graph_ext.h \
 src/common/player.h src/common/position_set.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
player.o: src/client/player.c src/common/graph.h src/client/player_ext.h \
 src/common/player.h src/common/move.h src/common/graph.h \
 src/common/position_set.h src/common/board.h src/common/dir.h \
 src/common/graph_ext.h src/common/player.h src/common/position_set.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
