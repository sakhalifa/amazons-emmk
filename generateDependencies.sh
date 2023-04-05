#!/bin/bash
SOURCEDIR=src
CLIENTDIR=$SOURCEDIR/client
SERVERDIR=$SOURCEDIR/server
COMMONDIR=$SOURCEDIR/common

find . -path ./fann -prune -o -name '*.c' -print | xargs gcc -MM -I$SERVERDIR -I$CLIENTDIR -I$COMMONDIR | sed -E 's/[^\\]$/&\n	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} -I${FANNDIR}\/src\/include $(CFLAGS) -o $@ $</g' > Makefile.inc