
all: ddmode

ddmode: ddmode.c
	$(CC) $(CFLAGS) -o ddmode ddmode.c

install: ddmode
	cpset ddmode $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f ddmode

