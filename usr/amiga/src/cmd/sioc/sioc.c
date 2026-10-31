/*
 *  sioc.c
 *  Issue an ioctl call on standard input
 *
 */

#define		MAX_KEYMAP	65536
#define		MAX_FONT	65536

#include <sys/types.h>
#include <sys/amiga/screen.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


char *commands[] = {
	"sioc",
	"front",
	"back",
	"activate",
	"getkmap",
	"setkmap",
	"setdefkmap",
	"winsize",
	"getfont",
	"setfont",
	"setdeffont",
	"gettype",
	"displaytype",
	"setdisplaytype",
	"setgroup",
	"getgroup",
	"setdisplayadjust",
	"getdisplayadjust",
	"setname",
	"getname",
	"settype",
	"allocbmap",
	"selbmap",
	"blankwait",
};


#define	NUM_COMMANDS	(sizeof(commands) / sizeof(char *))


streq(a, b)
char *a;
char *b;
{
	while (*a && *b) {
		if (toupper(*a) != toupper(*b))
			return(0);
		a++;
		b++;
	}

	if (!*a && !*b)
		return(1);
	else
		return(0);
}


parse_command(progname, nam)
char *progname;
char *nam;
{
int i;

	for (i = 0; i < NUM_COMMANDS; i++)
		if (streq(nam, commands[i]))
			return(i);

	fprintf(stderr, "%s: bad command: %s\n", progname, nam);
	exit(1);
}


show_usage(progname)
char *progname;
{
int i;

	fprintf(stderr, "%s: usage:\n", progname);

	for (i = 0; i < NUM_COMMANDS; i++)
		fprintf(stderr, "    %s\n", commands[i]);

	exit(1);
}


main(argc, argv)
int argc;
char **argv;
{
int cmd;
struct swinsize ws;		/* window size structure */
unsigned int displaytype;

	if (argc < 2)
		cmd = 0;			/* default command is SIOC */
	else if (strcmp(argv[1], "?") == 0
	     ||  strcmp(argv[1], "-?") == 0)
		show_usage(argv[0]);
	else if (*argv[1] >= '0' && *argv[1] <= '9')
		cmd = atoi(argv[1]);
	else
		cmd = parse_command(argv[0], argv[1]);

	switch(cmd) {
		case 0:		/* SIOC */
			if ( ioctl(0, SIOC, NULL) < 0)
				return(1);	/* not a screen device */
			else
				return(0);	/* yes, a screen device */

		case 1:		/* SIOCFRONT */
			if ( ioctl(0, SIOCFRONT, NULL) < 0 ) {
				perror("SIOCFRONT");
				return(1);
			} else
				return(0);

		case 2:		/* SIOCBACK */
			if ( ioctl(0, SIOCBACK, NULL) < 0) {
				perror("SIOCBACK");
				return(1);
			} else
				return(0);

		case 3:		/* SIOCACTIVATE */
			if ( ioctl(0, SIOCACTIVATE, NULL) < 0) {
				perror("SIOCACTIVATE");
				return(1);
			} else
				return(0);

		case 4:		/* SIOCGETKMAP */
			return( getkeymap() );

		case 5:		/* SIOCSETKMAP */
			if (argc < 3)
				return( setkeymap(NULL) );
			else
				return( setkeymap(argv[2]) );

		case 6:		/* SIOCSETDEFKMAP */
			if (argc < 3)
				return( setdefkeymap(NULL) );
			else
				return( setdefkeymap(argv[2]) );

		case 7:		/* SIOCWINSIZE */
			if ( ioctl(0, SIOCWINSIZE, &ws) >= 0 ) {
				printf("LINES=%d COLUMNS=%d\n",
						ws.charsy, ws.charsx);
				printf("PLINES=%d PCOLUMNS=%d\n",
						ws.pixsy, ws.pixsx);
				return(0);
			}
			perror("SIOCWINSIZE");
			return(1);

		case 8:		/* SIOCGETFONT */
			return( getfont(argc-2, argv+2) );

		case 9:		/* SIOCSETFONT */
			return( setfont(argc-2, argv+2) );

		case 10:	/* SIOCSETDEFFONT */
			if (argc < 3)
				return( setdeffont(NULL) );
			else
				return( setdeffont(argv[2]) );

		case 11:	/* SIOCGETTYPE */
			return( gettype() );

		case 12:	/* SIOCDISPLAYTYPE */
			if ( (displaytype=ioctl(0, SIOCDISPLAYTYPE)) !=
			      (unsigned int)-1 ) {
				printf("0x%x\n", displaytype);
				return(0);
			}
			perror("SIOCDISPLAYTYPE");
			return(1);

		case 13:	/* SIOCSETDISPLAYTYPE */
			if (argc < 3) {
				fprintf(stderr, "%s %s: missing argument\n",
					argv[0], argv[1]);
				return(1);
			}
			return(setdisplaytype(argv[2]));

		case 14:	/* SIOCSETGROUP */
			{
				int gn;
				if (argc < 3)
					gn = -1;
				else
					gn = strtol(argv[2], (char **)0, 0);
				if ( (gn=ioctl(0, SIOCSETGROUP, gn)) < 0 ) {
					perror("SIOCSETGROUP");
					return(1);
				}
				printf("%d\n", gn);
			}
			return(0);

		case 15:	/* SIOCGETGROUP */
			{
				int gn;
				if ( (gn=ioctl(0, SIOCGETGROUP)) < 0 ) {
					perror("SIOCGETGROUP");
					return(1);
				}
				printf("%d\n", gn);
			}
			return(0);

		case 16:	/* SIOCSETDISPLAYADJUST */
			{
				struct displayadjust displayadjust;
				if (argc < 6) {
					fprintf(stderr, "%s %s: missing arguments\n",
						argv[0], argv[1]);
					return(1);
				}
				if (ioctl(0, SIOCGETDISPLAYADJUST, &displayadjust)) {
					perror("SIOCGETDISPLAYADJUST");
					return(1);
				}
				displayadjust.xoffset =
					strtol(argv[2], (char **)0, 0);
				displayadjust.yoffset =
					strtol(argv[3], (char **)0, 0);
				displayadjust.xoverscan =
					strtol(argv[4], (char **)0, 0);
				displayadjust.yoverscan =
					strtol(argv[5], (char **)0, 0);
				if (ioctl(0, SIOCSETDISPLAYADJUST, &displayadjust)) {
					perror("SIOCSETDISPLAYADJUST");
					return(1);
				}
			}
			return(0);

		case 17:	/* SIOCGETDISPLAYADJUST */
			{
				struct displayadjust displayadjust;
				if (ioctl(0, SIOCGETDISPLAYADJUST, &displayadjust)) {
					perror("SIOCGETDISPLAYADJUST");
					return(1);
				}
				printf("xoffset:    %d\n", displayadjust.xoffset);
				printf("yoffset:    %d\n", displayadjust.yoffset);
				printf("xoverscan:  %d\n", displayadjust.xoverscan);
				printf("yoverscan:  %d\n", displayadjust.yoverscan);
			}
			return(0);

		case 18:	/* SIOCSETNAME */
			{
				char buf[SCRNAMESIZE];
				strncpy(buf, argv[2], SCRNAMESIZE);
				if (ioctl(0, SIOCSETNAME, buf)) {
					perror("SIOCSETNAME");
					return(1);
				}
			}
			return(0);

		case 19:	/* SIOCGETNAME */
			{
				char buf[SCRNAMESIZE];
				if (ioctl(0, SIOCGETNAME, buf)) {
					perror("SIOCGETNAME");
					return(1);
				}
				printf("%.*s\n", sizeof(buf), buf);
			}
			return(0);

		case 20:	/* SIOCSETTYPE */
			return( settype(argc-2, argv+2) );

		case 21:	/* SIOCALLOCBMAP */
			return( allocbmap(argc-2, argv+2) );

		case 22:	/* SIOCSELBMAP */
			{
				if (ioctl(0, SIOCSELBMAP, atoi(argv[2]))) {
					perror("SIOCSELBMAP");
					return(1);
				}
			}
			return(0);

		case 23:	/* SIOCBLANKWAIT */
			{
				if (ioctl(0, SIOCBLANKWAIT,
					  (argc>2)?atoi(argv[2])*1000:600)) {
					perror("SIOCBLANKWAIT");
					return(1);
				}
			}
			return(0);

		default:
			fprintf(stderr, "%s: Invalid command: %s\n",
				argv[0], argv[1]);
	}

	exit(1);
}


int setdisplaytype(s)
char *s;
{
unsigned int displaytype;

	if (*s == '-' || *s == '+') {
		if ( (displaytype=ioctl(0, SIOCDISPLAYTYPE)) ==
		     (unsigned int)-1 ) {
			perror("SIOCDISPLAYTYPE");
			return(1);
		}
		if (*s++ == '+')
			displaytype |= strtol(s, (char **)0, 0);
		else
			displaytype &= ~strtol(s, (char **)0, 0);
	} else
		displaytype = strtol(s, (char **)0, 0);

	if ( ioctl(0, SIOCSETDISPLAYTYPE, displaytype) == 0 )
		return(0);
	perror("SIOCSETDISPLAYTYPE");
	return(1);
}


getkeymap() {
struct keymap *km;		/* keymap header structure */
extern char *malloc();

	if ((km = (struct keymap *) malloc(MAX_KEYMAP)) == NULL) {
		fprintf(stderr, "SIOCGETKMAP: out of memory\n");
		return(1);
	}

	km->km_length = MAX_KEYMAP;

	if (ioctl(0, SIOCGETKMAP, km) < 0) {
		perror("SIOCGETKMAP");
		return(1);
	}

	km->km_count = 0;	/* force the reference count to zero */

	if (write(1, km, km->km_length) < km->km_length) {
		perror("bad write to stdout");
		return(1);
	}

	return(0);
}


struct keymap *readkeymap(fnam)
char *fnam;
{
struct keymap *km;
struct stat st;
extern char *malloc();
int fd;
int n;
int size;

	if (stat(fnam, &st) < 0) {
		perror("can't stat file");
		return(NULL);
	}

	size = st.st_size;

	if ((km = (struct keymap *) malloc(size)) == NULL) {
		fprintf(stderr, "sioc: out of memory\n");
		return(NULL);
	}

	if ((fd = open(fnam, 0)) < 0) {
		perror("can't open file");
		return(NULL);
	}

	if ((n = read(fd, km, size)) != size) {
		fprintf(stderr, "read: expected %d, got %d\n", size, n);
		perror("bad read on file");
		close(fd);
		return(NULL);
	}

	close(fd);

	if (km->km_length != size) {
		fprintf(stderr, "sioc: keymap is corrupt\n");
		return(NULL);
	}

	return(km);
}


setkeymap(fnam)
char *fnam;
{
struct keymap *km;

	if (fnam == NULL) {	/* system default keymap */
		if ( ioctl(0, SIOCSETKMAP, 0) < 0) {
			perror("SIOCSETKMAP");
			return(1);
		} else
			return(0);
	}

	if ((km = readkeymap(fnam)) != NULL) {
		if ( ioctl(0, SIOCSETKMAP, km) < 0) {
			perror("SIOCSETKMAP");
			return(1);
		} else
			return(0);
	}

	return(1);
}


setdefkeymap(fnam)
char *fnam;
{
struct keymap *km;

	if (fnam == NULL) {	/* system default keymap */
		if ( ioctl(0, SIOCSETDEFKMAP, 0) < 0) {
			perror("SIOCSETDEFKMAP");
			return(1);
		} else
			return(0);
	}

	if ((km = readkeymap(fnam)) != NULL) {
		if ( ioctl(0, SIOCSETDEFKMAP, km) < 0) {
			perror("SIOCSETDEFKMAP");
			return(1);
		} else
			return(0);
	}

	return(1);
}


static char *findfont(const char *fnam, const char *suffix)
{
	static char buf[2048];
	struct stat st;

	sprintf(buf, "%s%s", fnam, suffix?suffix:"");
	if (stat(buf, &st) < 0 && errno == ENOENT) {
		sprintf(buf, "/usr/amiga/lib/font/%s%s", fnam, suffix?suffix:"");
		if (stat(buf, &st) < 0) {
			if (errno == ENOENT)
				return 0;
			sprintf(buf, "%s%s", fnam, suffix?suffix:"");
		}
	}

	return buf;
}


struct font *readfont(fnam)
char *fnam;
{
struct font *f;
extern char *malloc();
int fd;
int n;
struct stat st;
int size;

	if ((fd = open(fnam, 0)) < 0 ||
	    fstat(fd, &st) < 0) {
		perror("can't open file");
		return(NULL);
	}

	size = st.st_size;

	if ((f = (struct font *) malloc(size)) == NULL) {
		fprintf(stderr, "sioc: out of memory\n");
		return(NULL);
	}

	if ((n = read(fd, f, size)) != size) {
		fprintf(stderr, "read: expected %d, got %d\n", size, n);
		perror("bad read on file");
		close(fd);
		return(NULL);
	}

	close(fd);

	if (f->f_length != size) {
		fprintf(stderr, "sioc: font is corrupt\n");
		return(NULL);
	}

	return(f);
}


getfont(argc, argv)
int argc;
char *argv[];
{
int fontnum = 0;
struct font *f;
extern char *malloc();

	if (argc>1)
		fontnum = atoi(argv[0]);

	if ((f = (struct font *) malloc(MAX_FONT)) == NULL) {
		fprintf(stderr, "SIOCGETFONT: out of memory\n");
		return(1);
	}

	f->f_length = MAX_FONT;

	if (ioctl(0, SIOCGETFONT+fontnum, f) < 0) {
		perror("SIOCGETFONT");
		return(1);
	}

	f->f_count = 0;	/* force the reference count to zero */

	if (write(1, f, f->f_length) < f->f_length) {
		perror("bad write to stdout");
		return(1);
	}

	return(0);
}


static int number(char *s, int *ip)
{
	int num = strtol(s, &s, 10);
	if (*s)
		return 0;
	*ip = num;
	return 1;
}


setfont(argc, argv)
int argc;
char *argv[];
{
int fontnum = 0;
struct font *f;
char *fontfile;

	if (argc==0) {
		/* sioc setfont */
		if (ioctl(0, SIOCSETFONT+0, 0) ||
		    ioctl(0, SIOCSETFONT+1, 0) ||
		    ioctl(0, SIOCSETFONT+2, 0) ||
		    ioctl(0, SIOCSETFONT+3, 0)) {
			perror("SIOCSETFONT");
			return(1);
		} else
			return(0);
	}

	if (number(argv[0], &fontnum)) {
		if (argc==1) {
			/* sioc setfont fontnum */
			if ( ioctl(0, SIOCSETFONT+fontnum, 0) < 0) {
				perror("SIOCSETFONT");
				return(1);
			} else
				return(0);
		}

		/* sioc setfont fontnum fontfilename */
		fontfile = findfont(argv[1], NULL);
		if ((f = readfont(fontfile?fontfile:argv[1])) != NULL) {
			if ( ioctl(0, SIOCSETFONT+fontnum, f) < 0) {
				perror("SIOCSETFONT");
				return(1);
			} else
				return(0);
		}
	}

	/* sioc setfont fontfilename */
	fontfile = findfont(argv[0], NULL);
	if (!fontfile)
		fontfile = findfont(argv[0], ".0");
	if (!fontfile)
		fontfile = argv[0];

	if ((f = readfont(fontfile)) == NULL)
		return(1);

	if ( ioctl(0, SIOCSETFONT+0, f) < 0) {
		perror("SIOCSETFONT");
		return(1);
	}

	if (fontfile = findfont(argv[0], ".1")) {
		if ((f = readfont(fontfile)) != NULL)
			return(1);
		if ( ioctl(0, SIOCSETFONT+1, f) < 0) {
			perror("SIOCSETFONT");
			return(1);
		}
	}

	if (fontfile = findfont(argv[0], ".2")) {
		if ((f = readfont(fontfile)) != NULL)
			return(1);
		if ( ioctl(0, SIOCSETFONT+2, f) < 0) {
			perror("SIOCSETFONT");
			return(1);
		}
	}

	if (fontfile = findfont(argv[0], ".3")) {
		if ((f = readfont(fontfile)) != NULL)
			return(1);
		if ( ioctl(0, SIOCSETFONT+3, f) < 0) {
			perror("SIOCSETFONT");
			return(1);
		}
	}

	return 0;
}



setdeffont(argc, argv)
int argc;
char *argv[];
{
int fontnum = 0;
struct font *f;

	if (argc>0 && number(argv[0], &fontnum))
		++argv, --argc;
	if (argc<1) {
		if ( ioctl(0, SIOCSETDEFFONT+fontnum, 0) < 0) {
			perror("SIOCSETDEFFONT");
			return(1);
		} else
			return(0);
	}

	if ((f = readfont(argv[0])) != NULL) {
		if ( ioctl(0, SIOCSETDEFFONT+fontnum, f) < 0) {
			perror("SIOCSETDEFFONT");
			return(1);
		} else
			return(0);
	}

	return(1);
}


gettype() {
struct scrtype s;

	if (ioctl(0, SIOCGETTYPE, &s) < 0) {
		perror("SIOCGETTYPE");
		return(1);
	}

	printf("dispx = %d\n", s.dispx);
	printf("dispy = %d\n", s.dispy);
	printf("dispz = %d\n", s.dispz);

	printf("Modes 0x%lx: ", s.modes);
	if (s.modes & SM_HAM)
		printf(" SM_HAM");
	if (s.modes & SM_HALFBRITE)
		printf(" SM_HALFBRITE");
	if (s.modes & SM_INTERLACE)
		printf(" SM_INTERLACE");
	if (s.modes & SM_NONLACE)
		printf(" SM_NONLACE");
	if (s.modes & SM_HEDLEY)
		printf(" SM_HEDLEY");
	if (s.modes & SM_NONHEDLEY)
		printf(" SM_NONHEDLEY");
	if (s.modes & SM_HIRES)
		printf(" SM_HIRES");
	if (s.modes & SM_LORES)
		printf(" SM_LORES");
	printf("\n");

	return(0);
}


/* usage: sioc settype flags x y z modes type pad0 */
settype(argc, argv)
int argc;
char *argv[];
{
	static struct scrtype scrtype;

	if (argc>0)
		scrtype.flags = strtol(argv[0], (char **)0, 0);
	else
		scrtype.flags = SF_MORERES;

	if (argc>1)
		scrtype.dispx = strtol(argv[1], (char **)0, 0);

	if (argc>2)
		scrtype.dispy = strtol(argv[2], (char **)0, 0);

	if (argc>3)
		scrtype.dispz = strtol(argv[3], (char **)0, 0);

	if (argc>4)
		scrtype.modes = strtol(argv[4], (char **)0, 0);

	if (argc>5)
		scrtype.type = strtol(argv[5], (char **)0, 0);

	if (argc>6)
		scrtype.pad0 = strtol(argv[6], (char **)0, 0);

	if (ioctl(0, SIOCSETTYPE, &scrtype) < 0) {
		perror("SIOCSETTYPE");
		return(1);
	}

	return(0);
}


/* usage: sioc allocbmap flags x y z */
allocbmap(argc, argv)
int argc;
char *argv[];
{
	int n;
	struct scrtype scrtype;
	static struct bmap bmap;

	if (argc>0)
		bmap.flags = strtol(argv[0], (char **)0, 0);

	if (argc<4 && ioctl(0, SIOCGETTYPE, &scrtype) == 0) {
		bmap.bmapx = scrtype.dispx;
		bmap.bmapy = scrtype.dispy;
		bmap.bmapz = scrtype.dispz;
	}

	if (argc>1)
		bmap.bmapx = strtol(argv[1], (char **)0, 0);

	if (argc>2)
		bmap.bmapy = strtol(argv[2], (char **)0, 0);

	if (argc>3)
		bmap.bmapz = strtol(argv[3], (char **)0, 0);

	if ((n=ioctl(0, SIOCALLOCBMAP, &bmap)) < 0) {
		perror("SIOCALLOCBMAP");
		return(1);
	}
	printf("%d\n", n);
}
