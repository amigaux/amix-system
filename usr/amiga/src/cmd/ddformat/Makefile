
all: ddformat

ddformat: ddformat.c
	$(CC) $(CFLAGS) -o ddformat ddformat.c

install: ddformat
	cpset ddformat $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f ddformat

