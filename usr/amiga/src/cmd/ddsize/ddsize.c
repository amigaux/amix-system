

/*
 * print SCSI disk capacity (in 512-byte blocks)
 */

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<stdio.h>
#include	"/usr/amiga/src/include/rico.h"
#include	"/usr/amiga/src/include/sd.h"


#define	NSENSE	16


bool	ddsize( ),
	readcapacity( ),
	requestsense( ),
	printsense( ),
	gsio( ),
	complain( );


main( argc, argv)
char	*argv[];
{

	unless (argc == 2)
		return (usage( ));
	return (done( ddsize( argv[1])));
}


done( ok)
bool	ok;
{

	return (ok? 0: 1);
}


bool
ddsize( dev)
char	*dev;
{
	struct sdcom	sdcom;
	struct stat	s;
	uint		ca;
	uchar		sense[NSENSE];

	unless (stat( dev, &s) == 0)
		return (complain( "can't find %s", dev));
	switch (s.st_mode & S_IFMT) {
	case S_IFBLK:
	case S_IFCHR:
		break;
	default:
		return (complain( "%s isn't a device", dev));
	}
	sdcom.card = sdcard( s.st_rdev);
	sdcom.unit = sdunit( s.st_rdev);
	unless ((requestsense( &sdcom, sense))
	and (readcapacity( &sdcom, &ca)))
		return (FALSE);
	switch (sdcom.status) {
	case 0:
		printf( "%d\n", ca);
		return (TRUE);
	case 2:
		if (requestsense( &sdcom, sense))
			printsense( &sdcom, sense);
		return (FALSE);
	case 8:
		return (complain( "device busy"));
	}
	return (complain( "unknown device condition"));
}


bool
readcapacity( cp, ca)
struct sdcom	*cp;
uint		*ca;
{
	uint	cbuf[2];

	cp->reading = TRUE;
	clearcdb( cp->cdb);
	cp->cdb[0] = 0x25;
	cp->addr = cbuf;
	cp->nbyte = sizeof cbuf;
	unless (gsio( cp))
		return (FALSE);
	*ca = cbuf[0];
	return (TRUE);
}


bool
requestsense( cp, sense)
struct sdcom	*cp;
uchar		sense[];
{

	cp->reading = TRUE;
	clearcdb( cp->cdb);
	cp->cdb[0] = 3;
	cp->cdb[4] = NSENSE;
	cp->addr = sense;
	cp->nbyte = NSENSE;
	unless (gsio( cp))
		return (FALSE);
	switch (cp->status) {
	case 0:
		return (TRUE);
	case 2:
		return (complain( "device crazy"));
	case 8:
		return (complain( "device busy"));
	}
	return (complain( "unknown device condition"));
}


bool
printsense( cp, sense)
struct sdcom	*cp;
uchar		sense[];
{
	static char *stab[] = {
		"no sense",
		"recovered error",
		"not ready",
		"medium error",
		"hardware error",
		"illegal request",
		"unit attention",
		"fubar",
		"fubar",
		"fubar",
		"fubar",
		"aborted command"
	};
	uint	k;

	k = sense[2] & 0x0F;
	if (k < nel( stab))
		return (complain( "%s (0x%X 0x%X)", stab[k], sense[2], sense[12]));
	return (complain( "loony tunes (0x%X 0x%X)", sense[2], sense[12]));
}


bool
gsio( cp)
struct sdcom	*cp;
{
	static char	GSDEVICE[]	= "/dev/scsi";
	static		fd		= -1;
	extern char	*sys_errlist[];
	extern		errno;

	if (fd < 0) {
		fd = open( GSDEVICE, 0);
		if (fd < 0)
			return (complain( "can't open %s", GSDEVICE));
	}
	if (ioctl( fd, *(uint *)"GSIO", cp) < 0)
		return (complain( "ioctl failed (%s)", sys_errlist[errno]));
	unless (cp->okay)
		return (complain( "SCSI bus failure"));
	return (TRUE);
}


clearcdb( p)
uchar	*p;
{
	struct sdcom	s;
	uint		i;

	for (i=0; i<nel( s.cdb); ++i)
		p[i] = 0;
}


bool
complain( mesg, arg0, arg1, arg2)
char	*mesg;
{

	fprintf( stderr, "ddsize: ");
	fprintf( stderr, mesg, arg0, arg1, arg2);
	fprintf( stderr, "\n");
	return (FALSE);
}


usage( )
{

	fprintf( stderr, "usage: ddsize device\n");
	return (2);
}
