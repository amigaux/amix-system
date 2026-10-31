
all: atdevs

atdevs: atdevs.c
	$(CC) $(CFLAGS) -o atdevs atdevs.c

install: atdevs
	cpset atdevs $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f atdevs

