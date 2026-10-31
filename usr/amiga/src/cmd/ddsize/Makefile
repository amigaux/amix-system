
all: ddsize

ddsize: ddsize.c
	$(CC) $(CFLAGS) -o ddsize ddsize.c

install: ddsize
	strip ddsize
	cpset ddsize $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f ddsize

