OBJECTS =	snftoauf.o
CC =		gcc
CFLAGS =	-O -IfromX

snftoauf:	${OBJECTS}
	${CC} ${CFLAGS} -o snftoauf ${OBJECTS}

install: snftoauf
	cpset snftoauf $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f ${OBJECTS} snftoauf

clobber: clean

