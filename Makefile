GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
CFLAGS = -std=c99 -Wall -Wextra -Wno-unknown-pragmas -fPIC -g3 -O3 -I$(GSL_PATH)/include --coverage
LDFLAGS = -lm -lgsl -lgslcblas -ldl \
	-L$(GSL_PATH)/lib -L$(GSL_PATH)/lib64 \
	-Wl,--rpath=${GSL_PATH}/lib
SOURCEDIR = src
CLIENTDIR = ${SOURCEDIR}/client
SERVERDIR = ${SOURCEDIR}/server
COMMONDIR = ${SOURCEDIR}/common
TESTDIR = test
INSTALLDIR = install

.SUFFIXES:

%.o:
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<

%.so: player_ext.o graph.o dir.o board.o
	gcc -shared $(CFLAGS) $^ $(LDFLAGS) -o $@

all: build

build: server client

server: server.o player_handle.o graph.o game.o board.o position_set.o
	gcc $(CFLAGS) $^ $(LDFLAGS) -o server

test: alltests

mcts.so: async_mcts.o position_set.o tree.o array_list.o util.o
mcts1.so: async_mcts.o position_set.o tree.o array_list.o util.o

alphabeta.so: alphabeta.o position_set.o 
alphabeta1.so: alphabeta.o position_set.o

random.so: player.o dir.o position_set.o
random1.so: player.o dir.o position_set.o

human.so: player_ext.o human.o

client: mcts.so alphabeta.so random.so mcts1.so alphabeta1.so random1.so

alltests: test_main.o test_game.o game.o \
player_handle.o graph.o board.o position_set.o \
mock_player.o player_handle.o
	gcc $(CFLAGS) $^ $(LDFLAGS) -o alltests

profiling: alphabeta.o board.o position_set.o graph.o player_handle.o graph.o game.o board.o position_set.o profiling.o
	gcc $(CFLAGS) $^ $(LDFLAGS) -o profiler

coverage: alltests
	./alltests
	gcov -o . server/game.c


run: install
	./install/server ./install/random.so ./install/alphabeta.so

install: build test
	cp server install/
	cp *.so install/
	cp alltests install/alltests

clean:
	@rm -vf *~ src/*~
	@rm -fv *.o *.so *.gcno *.gcda *.gcov *.out
	@rm -vrf install/*
	@find . -executable -type f -not -iname "*.*" -delete
	

.PHONY: client install test clean

include Makefile.inc

