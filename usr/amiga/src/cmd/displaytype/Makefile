
all: displaytype

displaytype: displaytype.c
	$(CC) $(CFLAGS) -o displaytype displaytype.c

install: displaytype
	cpset displaytype $(ROOT)/usr/amiga/bin 755 amiga amiga
	-ln $(ROOT)/usr/amiga/bin/displaytype $(ROOT)/usr/amiga/bin/ecs
	-ln $(ROOT)/usr/amiga/bin/displaytype $(ROOT)/usr/amiga/bin/ntsc
	-ln $(ROOT)/usr/amiga/bin/displaytype $(ROOT)/usr/amiga/bin/pal

clean:
	rm -f *.o

clobber: clean
	rm -f displaytype

