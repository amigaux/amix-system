
all: sioc

sioc: sioc.c
	$(CC) $(CFLAGS) -o sioc sioc.c

install: sioc
	strip sioc
	cpset sioc $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f sioc

