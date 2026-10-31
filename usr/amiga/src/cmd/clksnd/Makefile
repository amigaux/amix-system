OBJECTS = clksnds.o sayclock.o
LIBS	=
CFLAGS= -O

all: sayclock au

sayclock: $(OBJECTS)
	$(CC) $(CFLAGS) -o sayclock $(OBJECTS) $(LIBS)

au:	au.o
	$(CC) $(CFLAGS) -o au au.o $(LIBS)

install:
	cpset sayclock $(ROOT)/usr/amiga/bin 755 amiga amiga
	cpset au $(ROOT)/usr/amiga/bin 755 amiga amiga
	cpset vdate $(ROOT)/usr/amiga/bin 755 amiga amiga
	cpset rptsnd $(ROOT)/usr/amiga/bin 755 amiga amiga
	find sounds clksnds.as -depth -print | cpio -pmdv $(ROOT)/usr/amiga/lib/audio

$(OBJECTS):	say.h

clean:
	-rm -f *.o *~ core

clobber: clean
	rm -f sayclock au
