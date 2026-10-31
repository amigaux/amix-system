
all: format

format: format.sh
	cp format.sh format
	chmod 755 format

install: format
	cpset format $(ROOT)/usr/amiga/bin 755 amiga amiga

clean:
	rm -f format

clobber: clean

