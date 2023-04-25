#!/bin/bash
SOURCEDIR=src
CLIENTDIR=$SOURCEDIR/client
SERVERDIR=$SOURCEDIR/server
COMMONDIR=$SOURCEDIR/common

find . -name "*.c" | xargs gcc -MM -I$SERVERDIR -I$CLIENTDIR -I$COMMONDIR  > Makefile.inc