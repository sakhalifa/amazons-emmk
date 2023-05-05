GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
CFLAGS = -std=c99 -Wall -Wextra -Wno-unknown-pragmas -fPIC -g3 -O3 -I$(GSL_PATH)/include --coverage
LDFLAGS = -lm -lgsl -lgslcblas -ldl -lrt \
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

%.so: player_ext.o graph.o board.o
	gcc -shared $(CFLAGS) $^ $(LDFLAGS) -o $@

all: build test

build: server client

server: server.o player_handle.o graph.o game.o board.o position_set.o
	gcc $(CFLAGS) $^ $(LDFLAGS) -o server

test: alltests

mcts.so: async_mcts.o position_set.o tree.o array_list.o util.o
mcts1.so: async_mcts.o position_set.o tree.o array_list.o util.o

#Yeah... I use this to say every single %_spec_heuristic.o has the same rules as variable_heuristic.o defined in Makefile.inc...
HEURISTIC_TARGETS = $(shell make -f Makefile.inc -qp | grep variable_heuristic.o: | cut -d':' -f2)
%_spec_heuristic.o: $(HEURISTIC_TARGETS)
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} -DAGG=$(AGG) -DDEF=$(DEF) $(CFLAGS) -o $@ $<

balanced_spec_heuristic.o: AGG=1
balanced_spec_heuristic.o: DEF=1

alphabeta.so: alphabeta.o balanced_spec_heuristic.o position_set.o 
alphabeta1.so: alphabeta.o balanced_spec_heuristic.o position_set.o

alphachad.so: alphachad.o balanced_spec_heuristic.o position_set.o 
alphachad1.so: alphachad.o balanced_spec_heuristic.o position_set.o

defensive_spec_heuristic.o: AGG=0
defensive_spec_heuristic.o: DEF=1
bouclier.so: alphabeta.o defensive_spec_heuristic.o position_set.o 

aggressive_spec_heuristic.o: AGG=1
aggressive_spec_heuristic.o: DEF=0
epee.so:alphabeta.o aggressive_spec_heuristic.o position_set.o

trickery.so: trickery.o

random.so: player.o position_set.o
random1.so: player.o position_set.o

segdumb.so: segdumb.o position_set.o
human.so: player_ext.o human.o

allclients: alphachad.so alphabeta.so mcts.so random.so bouclier.so epee.so random1.so mcts1.so alphabeta1.so

client: alphachad.so alphabeta.so random.so

alltests: test_main.o test_game.o test_position_set.o game.o \
player_handle.o graph.o board.o position_set.o \
mock_player.o player_handle.o
	gcc $(CFLAGS) $^ $(LDFLAGS) -o alltests

profiling: alphabeta.o board.o position_set.o graph.o player_handle.o graph.o game.o board.o position_set.o profiling.o
	gcc $(CFLAGS) $^ $(LDFLAGS) -o profiler

coverage: alltests
	./alltests
	gcov -o . $(SERVERDIR)/*.c $(COMMONDIR)/*.c


run: install
	./install/server ./install/alphabeta.so ./install/alphabeta1.so

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

