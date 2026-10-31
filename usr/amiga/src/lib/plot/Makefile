

CFLAGS	= -O -I../../include
OBJECTS	= aspace.o color.o cont.o erase.o line.o move.o point.o rfill.o\
		space.o putpoint.o

plot.a: $(OBJECTS)
	ar r $@ $(OBJECTS)
install: plot.a
	mv plot.a ../../../lib
clean:
	-rm *.o
