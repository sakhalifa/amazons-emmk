SOURCEDIR="src"
CLIENTDIR="$SOURCEDIR/client"
SERVERDIR="$SOURCEDIR/server"
COMMONDIR="$SOURCEDIR/common"

find . -name *.c | xargs gcc -MM -I$SERVERDIR -I$CLIENTDIR -I$COMMONDIR | sed -E 's/[^\\]$/&\n	gcc -c -I${COMMONDIR} -I${CLIENTDIR} $(CFLAGS) -o $@ $</g' > dependencies.d