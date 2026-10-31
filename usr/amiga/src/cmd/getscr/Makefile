
all: getscr

getscr: getscr.c
	$(CC) $(CFLAGS) -o getscr getscr.c

install: getscr
	cpset getscr $(ROOT)/usr/amiga/etc 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f getscr

