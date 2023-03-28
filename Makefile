GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
CFLAGS = -std=c99 -Wall -Wextra -fPIC -g3 -I$(GSL_PATH)/include
LDFLAGS = -lm -lgsl -lgslcblas -ldl \
	-L$(GSL_PATH)/lib -L$(GSL_PATH)/lib64 \
	-Wl,--rpath=${GSL_PATH}/lib
SOURCEDIR = src
BUILDDIR = build
CLIENTDIR = ${SOURCEDIR}/client
SERVERDIR = ${SOURCEDIR}/server
COMMONDIR = ${SOURCEDIR}/common
TESTDIR = test
INSTALLDIR = install

all: createdir build

createdir:
	- mkdir -p build/{client,common,server,test}

build: server client

common/%.o: ${COMMONDIR}/%.c
	gcc -c -I${COMMONDIR} $(CFLAGS) -o $(BUILDDIR)/$@ $<

server/%.o: ${SERVERDIR}/%.c
	gcc -c -I${COMMONDIR} -I${SERVERDIR} $(CFLAGS) -o $(BUILDDIR)/$@ $<

server: server/server.o server/player_handle.o
	gcc $(CFLAGS) $(addprefix $(BUILDDIR)/, $^) $(LDFLAGS) -o ${INSTALLDIR}/server

client/%.o: ${CLIENTDIR}/%.c
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} $(CFLAGS) -o $(BUILDDIR)/$@ $<

client: client/player1.o client/player2.o
	gcc -shared $(BUILDDIR)/client/player1.o -o ${INSTALLDIR}/player1.so
	gcc -shared $(BUILDDIR)/client/player2.o -o ${INSTALLDIR}/player2.so

test/%.o: ${TESTDIR}/%.c
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} -I$(TESTDIR) $(CFLAGS) -o $(BUILDDIR)/$@ $<

alltests: test/server.o
	gcc $(CFLAGS) $(addprefix $(BUILDDIR)/, $^) $(LDFLAGS) -o ${INSTALLDIR}/test

test: alltests

install: server client test

clean:
	@rm -f *~ src/*~

.PHONY: client install test clean
