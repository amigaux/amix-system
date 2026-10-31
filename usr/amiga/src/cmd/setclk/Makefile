CC=gcc
CFLAGS=-g
LIBS=

all: setclk

setclk: setclk.o
	$(CC) $(CFLAGS) setclk.o -o setclk $(LIBS)

clean:
	rm -f *.o setclk core

clobber: clean

install: setclk
	cpset setclk $(ROOT)/usr/amiga/bin 755 amiga amiga

setclk.o: setclk.c
