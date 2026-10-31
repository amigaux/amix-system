
all: color

color: color.c
	$(CC) $(CFLAGS) -o color color.c

install: color
	cpset color $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f color

