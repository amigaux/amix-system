#
#  Makefile to produce an installation root floppy
#

all: root

root:
	/usr/lib/fs/s5/mkfs /dev/dsk/fd0 proto 2 11
	mount -F s5 /dev/dsk/fd0 /mnt
	ln /mnt/bin/ln /mnt/bin/cp
	ln /mnt/bin/ln /mnt/bin/mv
	ln /mnt/etc/mount /mnt/etc/umount
	ln -s /bin /mnt/usr/bin
	ln -s /bin /mnt/sbin
	ln -s /mnt/usr/share/lib /mnt/usr/share/lib
	ln -s /mnt/etc/magic /mnt/etc/magic
	(cd /usr/amiga/lib/kmap; ls | cpio -pmdv /mnt/etc/kmap)
	sync
	umount /mnt


#  To make a boot floppy, see the Makefile and README in /stand
#  The methods to produce a boot floppy below are obsolete.

#
#  Old A3000 booting style
#

A3000boot: etc/coffboot A3000fd0unix
	cat etc/coffboot A3000fd0unix > /dev/dsk/fd0

#
#  Old A2500 booting style
#

A2500boot: A2500fd0unix boot_proto
	/usr/lib/fs/s5/mkfs /dev/dsk/fd0 boot_proto 2 11
	echo Boot > /dev/dsk/fd0

