#!/bin/sh

PATH=/sbin:/usr/bin:/usr/sbin:/usr/amiga/bin

PROGRAM="$0"

REPEAT=false
BATCH=false
DONE=false

usage()
{
    echo "usage: $PROGRAM [-r] [-b] [:N|devicename]" >&2
    exit 1
}

while [ $# != 0 -a "$DONE" = false ]
do
    case "$1" in
	-r)
	    REPEAT=on
	    shift
	    ;;

	-b)
	    BATCH=on
	    shift
	    ;;

	-\?)
	    usage
	    ;;

	-*)
	    echo "$PROGRAM: invalid option \`$1'." >&2
	    usage
	    ;;

	 *)
	    DONE=true
	    ;;
    esac
done

if [ $# -gt 1 ]
then
    echo "$PROGRAM: bad argument count." >&2
    usage
fi

if [ $# = 1 ]
then
    # Device name given
	
    if [ "`expr \"$1\" : ':[0-9]'`" = 2 ]
    then
	# Drive number given
	DRIVENUM=`expr "$1" : ':\([0-9]\)'`
	DEVICE="/dev/rdsk/fd${DRIVENUM}f"
	PROMPT="drive fd${DRIVENUM}"
    elif [ "`expr \"$1\" : 'fd[0-9]'`" = 3 ]
    then
	# fdN given
	DEVICE="/dev/rdsk/${1}f"
	PROMPT="drive $1"
    elif [ "`expr \"$1\" : '/.*'`" = 0 -a ! -b "$1" ]
    then
	echo "$PROGRAM: bad device name given: $1" >&2
	exit 1
    else
	# Full device pathname given
	DEVICE="$1"
	PROMPT="device $1"
    fi
else
    # Default to /dev/rdsk/fd0f
    DEVICE=/dev/rdsk/fd0f
    PROMPT="drive fd0"
fi

if [ ! -c "$DEVICE" ]
then
    echo "$PROGRAM: Only raw devices can be formated." >&2
    exit 1
fi

while true
do
    if [ $BATCH = false ]
    then
	echo "Insert disk in $PROMPT and press [Return] or \`q' to Quit: \c"
	read foo

	if [ x"$foo" = xq ]
	then
	    exit 0
	fi
    fi

    dd if=/dev/zero bs=11k count=80 of="$DEVICE"

    if [ $REPEAT = false ]
    then
	exit 0
    fi
done
