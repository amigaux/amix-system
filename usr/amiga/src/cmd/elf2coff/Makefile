
all: elf2coff

elf2coff: elf2coff.c
	$(CC) $(CFLAGS) -o elf2coff elf2coff.c -lelf

install: elf2coff
	cpset elf2coff $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f *.o

clobber: clean
	rm -f elf2coff

