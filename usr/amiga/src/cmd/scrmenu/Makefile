CC	= gcc
OBJECTS	= processinfo.o scrmenu.o font.o

scrmenu:	${OBJECTS}
	${CC} -o scrmenu ${OBJECTS}

${OBJECTS}:	scrmenu.h

font.s:		font.m4
	m4 font.m4 > font.s

install: scrmenu
	cpset scrmenu $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f ${OBJECTS} scrmenu font.s

clobber: clean
