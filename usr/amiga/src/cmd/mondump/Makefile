
all: mondump

mondump: mondump.c
	$(CC) $(CFLAGS) -o mondump mondump.c

install: mondump
	cpset mondump $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f mondump

