set path=($home/bin /usr/local/bin /usr/bin /usr/ccs/bin /sbin /usr/sbin /etc /usr/ucb /usr/amiga/bin /usr/public/bin /usr/hosts /usr/games .)

switch ( `tty` )
	case /dev/console:
	case /dev/syscon:
	case /dev/systty:
	case /dev/term/con*:
		set term = amiga
		breaksw
endsw
stty erase '^H' kill '^U' intr '^C' eof '^D' quit '^\'
stty iexten echoke echoe echoctl echoprt -istrip tab0

cat -s /etc/motd
if ( { mail -e } ) then
	echo "you have mail"
endif
if ( ${?LOGNAME} ) then
	if ( ${LOGNAME} != root ) then
		news -n
	endif
endif

umask 022

set history=25

