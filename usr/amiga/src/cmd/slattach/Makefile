
#  Note:  must define BACKWARDS_CHARACTER_CONSTANTS if CI5 compiler is used
CC=gcc
CFLAGS=

all: slattach

slattach: slattach.c
	$(CC) $(CFLAGS) -o slattach slattach.c

install: slattach
	cpset slattach $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f slattach

