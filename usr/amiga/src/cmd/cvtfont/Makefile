
CFLAGS=-g

OFILES=alloc.o font.o loadseg.o

all:	cvtfont

cvtfont:	$(OFILES)
	cc -o cvtfont font.o alloc.o loadseg.o

$(OFILES):	screen.h
	cc -c -I/usr/amigados/includes $(CFLAGS) $*.c

shar:
	shar -v Makefile *.[ch] > ../cvtfont.shar

