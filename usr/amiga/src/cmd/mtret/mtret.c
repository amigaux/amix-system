

#include	<sys/types.h>
#include	<sys/mkdev.h>
#include	<sys/stat.h>
#include	<stdio.h>
#include	"rico.h"
#include	"/usr/amiga/src/include/sd.h"


#define	NSENSE	12


int		gsfd;
uchar		sense[12];
extern		errno;
extern char	*sys_errlist[];

bool	mtret( ),
	request_sense( ),
	load( ),
	getdev( ),
	baddevice( ),
	complain( );


main( argc, argv)
char	*argv[];
{

	switch (argc) {
	case 1:
		return (done( mtret( "/dev/rmt/4")));
	case 2:
		return (done( mtret( argv[1])));
	}
	return (usage( ));
}


static
done( ok)
bool	ok;
{

	return (ok? 0: 1);
}


static bool
mtret( dev)
char	*dev;
{
	struct sdcom	c;
	dev_t		a,
			b;

	unless ((getdev( "/dev/rmt/4", &a))
	and (getdev( dev, &b)))
		return (FALSE);
	unless (major( a) == major( b))
		return (baddevice( dev));
	gsfd = open( "/dev/scsi", 2);
	if (gsfd < 0)
		return (complain( "can't access /dev/scsi"));
	c.card = sdcard( b);
	c.unit = sdunit( b);
	return (request_sense( &c) && load( &c));
}


static bool
request_sense( cp)
struct sdcom	*cp;
{

	cp->reading = TRUE;
	cp->cdb[0] = 0x03;
	cp->cdb[1] = 0;
	cp->cdb[2] = 0;
	cp->cdb[3] = 0;
	cp->cdb[4] = sizeof sense;
	cp->cdb[5] = 0;
	cp->nbyte = sizeof sense;
	cp->addr = sense;
	unless (ioctl( gsfd, 'GSIO', cp) == 0)
		return (complain( "ioctl GSIO failed (%s)", sys_errlist[errno]));
	unless (cp->okay)
		return (complain( "SCSI timeout"));
	switch (cp->status) {
	case 0:
		return (TRUE);
	case 8:
		return (complain( "device busy"));
	}
	return (complain( "hardware is crazy"));
}


static bool
load( cp)
struct sdcom	*cp;
{

	cp->cdb[0] = 0x1B;
	cp->cdb[1] = 0;
	cp->cdb[2] = 0;
	cp->cdb[3] = 0;
	cp->cdb[4] = 1<<1 | 1<<0;
	cp->cdb[5] = 0;
	cp->nbyte = 0;
	unless (ioctl( gsfd, 'GSIO', cp) == 0)
		return (complain( "ioctl GSIO failed (%s)", sys_errlist[errno]));
	unless (cp->okay)
		return (complain( "SCSI timeout"));
	switch (cp->status) {
	case 0:
		return (TRUE);
	case 2:
		if (request_sense( cp))
			printsense( );
		return (FALSE);
	case 8:
		return (complain( "device busy"));
	}
	return (complain( "hardware is crazy"));
}


static
printsense( )
{
	static uchar *key[] = {
		0,
		0,
		"not ready",
		"media error",
		"hardware error",
		"illegal request",
		"unit attention",
		"data protect",
		"blank check",
		0,
		0,
		"aborted command",
		0,
		"volume overflow"
	};

	uint s = sense[2] & 0x0F;
	if ((s < nel( key))
	and (key[s]))
		fprintf( stderr, "mtret: %s (", key[s]);
	else
		fprintf( stderr, "mtret: sense %d (", s);
	printbits( sense[8], "   CNI   WRPEOMUDEBNLFIL");
	printbits( sense[9], "      NDT   BOMBPE      ");
	fprintf( stderr, "%d)\n", *(ushort *)(sense+10));
}


static
printbits( bits, names)
uchar	*names;
{

	while (*names) {
		if (bits & 1<<7)
			fprintf( stderr, "%.3s ", names);
		bits <<= 1;
		names += 3;
	}
}


static bool
getdev( name, devp)
char	*name;
dev_t	*devp;
{
	struct stat	s;

	unless (stat( name, &s) == 0)
		return (complain( "can't access %s", name));
	unless ((s.st_mode&S_IFMT) == S_IFCHR)
		return (baddevice( name));
	*devp = s.st_rdev;
	return (TRUE);
}


bool
baddevice( file)
char	*file;
{

	return (complain( "%s is not a tape device", file));
}


static bool
complain( mesg, arg0)
char	*mesg;
{

	fprintf( stderr, "mtret: ");
	fprintf( stderr, mesg, arg0);
	fprintf( stderr, "\n");
	return (FALSE);
}


static
usage( )
{

	fprintf( stderr, "usage: mtret [ device ]\n");
	return (2);
}
