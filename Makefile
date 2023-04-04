GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
CFLAGS = -std=c99 -Wall -Wextra -fPIC -g3 -I$(GSL_PATH)/include --coverage
LDFLAGS = -lm -lgsl -lgslcblas -ldl \
	-L$(GSL_PATH)/lib -L$(GSL_PATH)/lib64 \
	-Wl,--rpath=${GSL_PATH}/lib
SOURCEDIR = src
CLIENTDIR = ${SOURCEDIR}/client
SERVERDIR = ${SOURCEDIR}/server
COMMONDIR = ${SOURCEDIR}/common
TESTDIR = test
INSTALLDIR = install

all: build

build: server client alltests

server: server.o player_handle.o graph.o game.o board.o
	gcc $(CFLAGS) $^ $(LDFLAGS) -o server

test: alltests

player1.so: player1.o
	gcc -shared $(CFLAGS) player1.o $(LDFLAGS) -o player1.so

player2.so: player2.o
	gcc -shared $(CFLAGS) player1.o $(LDFLAGS) -o player2.so

client: player1.so player2.so

alltests: test_server.o game.o player_handle.o graph.o board.o
	gcc $(CFLAGS) $^ $(LDFLAGS) -o alltests

install: server client
	cp server install/
	cp player*.so install/
	cp alltests install/alltests

clean:
	@rm -f *~ src/*~
	@rm -f *.{o,so,gcno,gcda}
	@rm -rf install/*
	@find . -executable -type f -not -iname "*.*" -delete
	

.PHONY: client install test clean


include Makefile.inc
