test.o: test.c
	gcc -c -I${COMMONDIR} -I${CLIENTDIR} -I${SERVERDIR} $(CFLAGS) -o $@ $<
