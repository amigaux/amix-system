
CFLAGS=-g

all: rdb

rdb: rdb.o
	$(CC) -o rdb rdb.o $(LFLAGS)

rdb.o:	rdb.c

clean:
	rm -f *.o

clobber: clean
	rm -f rdb

install: rdb
	strip rdb
	cpset rdb $(ROOT)/usr/amiga/bin 755 amiga amiga

