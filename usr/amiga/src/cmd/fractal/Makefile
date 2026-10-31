FRACOBJS= fractal.o input.o pointer.o zoombox.o ipc.o compute.o screen.o \
	  scrfx.o colors.o gif.o gifwr.o
FRACLIBS=-lscreen -lgen

COLOROBJS= scrcols.o pointer.o scrfx.o scrfont.o font.o
COLORLIBS=-lscreen

SLICEOBJS=fracslice.o

CFLAGS=-O
CC=gcc

all: fractal scrcols fracslice

fractal: $(FRACOBJS)
	$(CC) $(CFLAGS) $(FRACOBJS) -o $@ $(FRACLIBS)

scrcols: $(COLOROBJS)
	$(CC) $(CFLAGS) $(COLOROBJS) -o $@ $(COLORLIBS)

fracslice: $(SLICEOBJS)
	$(CC) $(CFLAGS) $(SLICEOBJS) -o $@ $(SLICELIBS)

font.s:		font.m4
	m4 font.m4 > font.s

clean:
	rm -f $(FRACOBJS) $(COLOROBJS) core

clobber: clean
	rm -f fractal scrcols

install: all
	cpset fractal $(ROOT)/usr/amiga/bin 755 amiga amiga
	cpset scrcols $(ROOT)/usr/amiga/bin 755 amiga amiga
	ln $(ROOT)/usr/amiga/bin/fractal $(ROOT)/usr/amiga/bin/julia

$(FRACOBJS): fractal.h pointer.h
$(COLOROBJS): pointer.h
