CC=gcc
CFLAGS=-g
LIBS=

all: aen

aen: aen.o trace.o bitfields.o
	$(CC) $(CFLAGS) aen.o trace.o bitfields.o -o aen $(LIBS)

clean:
	rm -f *.o aen core

clobber: clean

install: aen
	cpset aen $(ROOT)/usr/amiga/bin 755 amiga amiga

aen.o: aenuser.h
trace.o: aenuser.h structs.h bitfields.h
bitfields.o: bitfields.h
