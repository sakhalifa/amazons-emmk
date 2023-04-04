player1.o: src/client/player1.c src/common/move.h \
 src/client/player_custom.h src/common/player.h src/common/move.h \
 src/common/graph.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
player2.o: src/client/player2.c src/common/move.h \
 src/client/player_custom.h src/common/player.h src/common/move.h \
 src/common/graph.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
player.o: src/client/player.c src/common/graph.h \
 src/client/player_custom.h src/common/player.h src/common/move.h \
 src/common/graph.h src/common/board.h src/common/dir.h \
 src/common/player.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
board.o: src/common/board.c src/common/board.h src/common/dir.h \
 src/common/graph.h src/common/move.h src/common/player.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
graph.o: src/common/graph.c src/common/graph.h src/common/graph_ext.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
server_init.o: src/server/server_init.c src/common/graph_ext.h \
 src/common/graph.h src/common/dir.h src/server/server.h \
 src/server/player_handle.h src/common/player.h src/common/move.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
server.o: src/server/server.c src/common/graph_ext.h src/common/graph.h \
 src/common/dir.h src/server/server.h src/server/player_handle.h \
 src/common/player.h src/common/move.h src/server/server_init.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
player_handle.o: src/server/player_handle.c src/server/player_handle.h \
 src/common/player.h src/common/move.h src/common/graph.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
test_server.o: test/test_server.c src/server/server_init.h \
 src/server/server.h src/server/player_handle.h src/common/player.h \
 src/common/move.h src/common/graph.h
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
