
all: null_kludge

null_kludge: null_kludge.c
	$(CC) $(CFLAGS) -o null_kludge null_kludge.c -lelf

install: null_kludge
	cpset null_kludge $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f null_kludge

