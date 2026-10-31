CFLAGS=

all: amixpkg

amixpkg: amixpkg.c
	$(CC) $(CFLAGS) -o amixpkg amixpkg.c

clean:
	rm -f *.o

clobber: clean
	rm -f amixpkg

install: amixpkg
	strip amixpkg
	cpset amixpkg $(ROOT)/usr/amiga/bin 755 amiga amiga

