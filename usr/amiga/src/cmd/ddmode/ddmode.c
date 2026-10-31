

/*
 * set operating mode for SCSI Quantum disk
 */

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<stdio.h>
#include	"../../include/rico.h"
#include	"/usr/src/uts/m68k/amiga/alien/sd.h"


/*
 * AUTOMATIC READ REALLOCATION ENABLED (ARRE) - An automatic read reallocation
 * enabled bit 6 of byte 2 set to a value of one indicates that the target
 * shall enable automatic reallocation of bad blocks.  The execution of the
 * automatic reallocation is similar to the function of the REASSIGN BLOCKS
 * command, but is initiated by the ProDrive (tm) Series when a hard error is
 * encountered (i.e. same non-zero ECC syndrome on two consecutive retries, see
 * Appendix A).  An ARRE bit set to a value of zero indicates that the target
 * shall not perform automatic reallocation of bad blocks.  If RC is one, this
 * bit is ignored.
 *
 * REALLOCATE UNCORRECTABLE ERROR ENABLED (RUEE) - A reallocate uncorrectable
 * error enabled bit 4 of byte 2 set to a value of one indicates that if the
 * ARRE bit in Page 1 of the Error Recovery Parameters is set, the ProDrive
 * Series will automatically reallocate uncorrectable hard errors.  The
 * uncorrectable (bad) data is written to a new block and a new ECC (one that
 * is correct for the BAD data) is appended.  This permits the new block to be
 * read without error; however, the data is still the original uncorrectable or
 * BAD data.  A RUEE bit set to a value of zero indicates that the ProDrive
 * Series will not attempt automatic reallocation if the hard error encountered
 * is determined to be uncorrectable.  In this case, a CHECK CONDITION with a
 * sense key of MEDIUM ERROR - Uncorrectable Data Error - will result.
 *
 *	Quoted from "Product Description Manual", ProDrive 40S/80S,
 *	January 1988, Quantum Corporation
 */

#define	set_arre( mode)	((mode)[4+8+2] |= 1 << 6)
#define	set_ruee( mode)	((mode)[4+8+2] |= 1 << 4)

#define	NSENSE	16
#define	NMODE	(4+8+8)


bool	query;

char	**init( );
bool	ddmode( ),
	modesense( ),
	modeselect( ),
	requestsense( ),
	printsense( ),
	gsio( ),
	complain( );


main( argc, argv)
char	**argv;
{

	unless (argv = init( argv))
		return (usage( ));
	return (done( ddmode( argv[0])));
}


done( ok)
bool	ok;
{

	return (ok? 0: 1);
}


char	**
init( av)
char	**av;
{

	if ((++av)[0] && streq( av[0], "-query")) {
		query = TRUE;
		++av;
	}
	if ((av[0])
	and (not av[1]))
		return (av);
	return (0);
}


bool
ddmode( dev)
char	*dev;
{
	struct sdcom	sdcom;
	struct stat	s;
	uchar		sense[NSENSE],
			mode[NMODE];

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
	and (modesense( &sdcom, mode, 0x01)))
		return (FALSE);
	if (query)
		switch (sdcom.status) {
		case 0:
			printmode( mode, "Error Recovery Parameters");
			break;
		case 2:
			if (requestsense( &sdcom, sense))
				printsense( &sdcom, sense);
			return (FALSE);
		case 8:
			return (complain( "device busy"));
		default:
			return (complain( "unknown device condition"));
		}
	else {
		set_arre( mode);
		mode[0] = 0;
		mode[4+1] = 0;
		mode[4+2] = 0;
		mode[4+3] = 0;
		mode[4+8+0] &= ~ (1<<7);
		unless (modeselect( &sdcom, mode))
			return (FALSE);
		switch (sdcom.status) {
		case 0:
			break;
		case 2:
			if (requestsense( &sdcom, sense))
				printsense( &sdcom, sense);
			return (FALSE);
		case 8:
			return (complain( "device busy"));
		default:
			return (complain( "unknown device condition"));
		}
	}
	unless (modesense( &sdcom, mode, 0x39))
		return (FALSE);
	if (query)
		switch (sdcom.status) {
		case 0:
			printmode( mode, "Quantum Unique Drive Control Parameters");
			break;
		case 2:
			if (requestsense( &sdcom, sense))
				printsense( &sdcom, sense);
			return (FALSE);
		case 8:
			return (complain( "device busy"));
		default:
			return (complain( "unknown device condition"));
		}
	else {
		set_ruee( mode);
		mode[0] = 0;
		mode[4+1] = 0;
		mode[4+2] = 0;
		mode[4+3] = 0;
		mode[4+8+0] &= ~ (1<<7);
		unless (modeselect( &sdcom, mode))
			return (FALSE);
		switch (sdcom.status) {
		case 0:
			break;
		case 2:
			if (requestsense( &sdcom, sense))
				printsense( &sdcom, sense);
			return (FALSE);
		case 8:
			return (complain( "device busy"));
		default:
			return (complain( "unknown device condition"));
		}
	}
	return (TRUE);
}


bool
modesense( cp, mode, page)
struct sdcom	*cp;
uchar		mode[];
{

	cp->reading = TRUE;
	clearcdb( cp->cdb);
	cp->cdb[0] = 0x1A;
	cp->cdb[1] = 0;
	cp->cdb[2] = page;
	cp->cdb[3] = 0;
	cp->cdb[4] = NMODE;
	cp->cdb[5] = 0;
	cp->addr = mode;
	cp->nbyte = NMODE;
	return (gsio( cp));
}


bool
modeselect( cp, mode)
struct sdcom	*cp;
uchar		mode[];
{

	cp->reading = FALSE;
	clearcdb( cp->cdb);
	cp->cdb[0] = 0x15;
	cp->cdb[1] = 1;
	cp->cdb[2] = 0;
	cp->cdb[3] = 0;
	cp->cdb[4] = NMODE;
	cp->cdb[5] = 0;
	cp->addr = mode;
	cp->nbyte = NMODE;
	return (gsio( cp));
}


printmode( mode, label)
uchar	mode[];
char	*label;
{
	uint	i;

	printf( "%s:\n\t", label);
	for (i=0; i<NMODE; ++i)
		printf( "%02X ", mode[i]);
	printf( "\n");
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
		return (complain( "SCSI bus timeout"));
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

	fprintf( stderr, "ddmode: ");
	fprintf( stderr, mesg, arg0, arg1, arg2);
	fprintf( stderr, "\n");
	return (FALSE);
}


usage( )
{

	fprintf( stderr, "usage: ddmode [ -query ] device\n");
	return (2);
}
