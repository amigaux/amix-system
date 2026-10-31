
CFLAGS=-Kpic -O

libscreen.so : screen.o
	ld -G -h /usr/amiga/lib/libscreen.so -o libscreen.so screen.o

install : libscreen.so
	cp libscreen.so $(ROOT)/usr/amiga/lib
	[ -h $(ROOT)/usr/lib/libscreen.so ] || \
	ln -s /usr/amiga/lib/libscreen.so $(ROOT)/usr/lib

clean :
	rm -f libscreen.so screen.o

clobber: clean

