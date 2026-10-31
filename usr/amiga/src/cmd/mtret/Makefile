
all: mtret

mtret: mtret.c
	$(CC) $(CFLAGS) -I/usr/amiga/src/include -o mtret mtret.c

install: mtret
	cpset mtret $(ROOT)/usr/amiga/bin 2755 amiga sys

clean:
	rm -f *.o

clobber: clean
	rm -f mtret

