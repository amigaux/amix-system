

CFLAGS	= -O -I../../include
OBJECTS	= alloc.o die.o

hlib.a: $(OBJECTS)
	ar r $@ $(OBJECTS)
install: hlib.a
	mv hlib.a ../../../lib
clean:
	-rm *.o
