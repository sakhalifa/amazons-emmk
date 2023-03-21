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

all: build

build: server client

common/%.o: ${COMMONDIR}/%.c
	- gcc -c -I${COMMONDIR} -I${COMMONDIR} $(CFLAGS) -o $(BUILDDIR)/$@ $<


server/%.o: ${SERVERDIR}/%.c
	- gcc -c -I${COMMONDIR} -I${SERVERDIR} $(CFLAGS) -o $(BUILDDIR)/$@ $<

server:

client/%.o: ${CLIENTDIR}/%.c
	- gcc -c -I${COMMONDIR} -I${CLIENTDIR} $(CFLAGS) -o $(BUILDDIR)/$@ $<

client: client/player.o common/player.h 
	- gcc $(CFLAGS) $(addprefix $(BUILDDIR)/, $^) -o project

alltests:

test: alltests

install: server client test

clean:
	@rm -f *~ src/*~

.PHONY: client install test clean
