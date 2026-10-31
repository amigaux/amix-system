
all: samp

samp: samp.c
	$(CC) $(CFLAGS) -o samp samp.c

install: samp
	cpset samp $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f samp

