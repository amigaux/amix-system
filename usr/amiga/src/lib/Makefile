
all: knlist.o
	cd screen; $(MAKE)

install:
	cd screen; $(MAKE) install
	cp knlist.o $(ROOT)/usr/amiga/lib

clean:
	cd screen; $(MAKE) clean
	rm -f *.o

clobber: clean

