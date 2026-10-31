/* A getty-front-end which opens an Amix console screen and sets its */
/* graphics modes, resolution, etc., before running getty.  It does a */
/* special trick to have the device stay open when getty reopens its */
/* fds 0,1,2 -- it leaves file descriptor 3 as another handle on the device. */
/* This actually stays open even into the login shell, which may cause */
/* problems someday ... -=] Ford [=- */

/* RJS:  made it take arguments, do colors and load fonts
 *
 *  Args are:
 *
 *	-0 {color}	Set background color
 *	-1 {color}	Set foreground color
 *	-f font		Startup in font; absolute font filename or
 *			relative to FONTLIB
 *	-m mode		Startup with scrtype.modes
 *	-x n		Request a screen n pixels wide
 *	-y n		Request a screen n pixels high
 *	-g n		Attach to "screen group" #n (otherwise it's random)
 *	-n name		Set screen name to "name"
 *
 *  {color} is gg, RGB, ggRGB, RRGGBB, or ggRRGGBB.
 *
 *  Sample /etc/inittab entry:
 *
 * con7:23:respawn:/etc/getscr -f topaz11 -0 00000 -1 ff0b0 -m 0x8 con7
 *
 *  This loads the font /usr/amiga/lib/font/topaz11, sets the background
 *  color to black, the foreground color to green, and the modes to
 *  SM_NONLACE.
 *
 */


#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/screen.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<stdlib.h>


#define		MAX_FONT	65536
#define		FONTLIB		"/usr/amiga/lib/font/"

#define		TRUE	1
#define		FALSE	0


char *progname;


main(argc, argv)
int argc;
char *argv[];
{
extern int optind, opterr;
extern char *optarg;
int c;
struct scolor creg[32];
struct scolor reg0, reg1;
char fontname[200];
char devnam[200];
char buf[200];
struct scrtype s;
int groupnum = -1; /* -1 means pseudorandomly choose group */
char screenname[SCRNAMESIZE];
int set0, set1;
char *av[30];
int i;

	set0 = FALSE;
	set1 = FALSE;

	fontname[0] = '\0';
	progname = argv[0];

	s.flags = SF_LESSRES;
	s.type = 0;
	s.dispx = 2048;
	s.dispy = 2048;
	s.dispz = 1;
	s.modes = 0;

	opterr = 0;
	while ((c = getopt(argc, argv, "0:1:f:m:x:y:z:g:n:")) != -1) {
		switch (c) {
		case '0':	/* background color register */
			if (scan_color(optarg, &reg0))
				set0 = TRUE;
			break;

		case '1':	/* foreground color register */
			if (scan_color(optarg, &reg1))
				set1 = TRUE;
			break;

		case 'f':	/* load a font */
			if (*optarg == '/')
				strcpy(fontname, optarg);
			else {
				strcpy(fontname, FONTLIB);
				strcat(fontname, optarg);
			}
			break;

		case 'x':	/* set x resolution wanted */
			s.dispx = strtol(optarg, (char **)0, 0);
			break;

		case 'y':	/* set y resolution wanted */
			s.dispy = strtol(optarg, (char **)0, 0);
			break;

		case 'z':	/* set z resolution wanted */
			s.dispz = strtol(optarg, (char **)0, 0);
			break;

		case 'm':	/* set screen modes */
			s.modes = strtol(optarg, (char **)0, 0);
			break;

		case 'g':	/* set screen group (function-key) */
			groupnum = strtol(optarg, (char **)0, 0);
			break;

		case 'n':	/* set screen name */
			strncpy(screenname,optarg,sizeof screenname);
			break;
		}
	}

	if (optind < argc) {
		sprintf(devnam, "/dev/%s", argv[optind]);
		av[1] = argv[optind];
		optind++;
	} else {
		fprintf(stderr, "%s: no device specified\n", progname);
		opterr++;
	}

	av[0] = "getty";
	for (i = 2; optind < argc; i++, optind++)
		av[i] = argv[optind];
	av[i] = NULL;

	if (opterr) {
		sleep(10);
		exit(1);
	}

	close(0);
	close(1);
	close(2);
	if (open(devnam, O_RDWR) != 0) {
		dup(dup(open("/dev/console", O_WRONLY)));
		sprintf(buf, "%s: can't open %s", progname, devnam);
		perror(buf);
		sleep(10);
		exit(1);
	}

	dup(0);
	dup(0);
	dup(0);

	if (ioctl(0, SIOCSETTYPE, &s)) {
		sprintf(buf, "%s: warning: can't SIOCSETTYPE %s", progname,
								devnam);
		perror(buf);
	}

	if (ioctl(0, SIOCGETTYPE, &s)) {
		sprintf(buf, "%s: warning: can't initialize screen %s",
							progname, devnam);
		perror(buf);

		sleep(5);
		close(0);
		close(1);
		close(2);
		close(3);
	}

	if (ioctl(0, SIOCSETGROUP, groupnum) < 0) {
		sprintf(buf, "%s: warning: can't attach %s to group %d",
			progname, devnam, groupnum);
		perror(buf);
	}

	if (ioctl(0, SIOCSETNAME, screenname) < 0) {
		sprintf(buf, "%s: warning: can't set screen name to \"%.*s\"",
			progname, sizeof screenname, screenname);
		perror(buf);
	}

	if (set0 || set1) {
		if (ioctl(0, SIOCGETCMAP, creg)) {
			sprintf(buf, "%s: can't SIOCGETCMAP for %s",
						progname, devnam);
			perror(buf);
		}

		if (set0)
			memcpy(&creg[0], &reg0, sizeof(reg0));
		if (set1)
			memcpy(&creg[1], &reg1, sizeof(reg1));

		if (ioctl(0, SIOCSETCMAP, creg)) {
			sprintf(buf, "%s: can't SIOCSETCMAP for %s",
						progname, devnam);
			perror(buf);
		}
	}

	if (strlen(fontname))
		setfont(fontname);

	execv("/etc/getty", av);
	perror("can't exec getty");

	sleep(10);
	exit(1);
}


int scan2(const char *s, struct scolor *reg)
{
int gray;

	if (sscanf(s, "%2x", &gray) != 1)
		return 1;
	reg->red = reg->green = reg->blue = reg->gray = gray;
	return 0;
}


int scan3(const char *s, struct scolor *reg)
{
int red, green, blue;

	if (sscanf(s, "%1x%1x%1x", &red, &green, &blue) != 3)
		return 1;
	reg->red   = red * 0x11;
	reg->green = green * 0x11;
	reg->blue  = blue * 0x11;
	reg->gray  = (reg->red+reg->green+reg->blue)/3;
	return 0;
}


int scan5(const char *s, struct scolor *reg)
{
int gray, red, green, blue;

	if (sscanf(s, "%2x%1x%1x%1x", &gray, &red, &green, &blue) != 4)
		return 1;
	reg->red   = red * 0x11;
	reg->green = green * 0x11;
	reg->blue  = blue * 0x11;
	reg->gray  = gray;
	return 0;
}


int scan6(const char *s, struct scolor *reg)
{
int red, green, blue;

	if (sscanf(s, "%2x%2x%2x", &red, &green, &blue) != 3)
		return 1;
	reg->red   = red;
	reg->green = green;
	reg->blue  = blue;
	reg->gray  = (red+green+blue)/3;
	return 0;
}


int scan8(const char *s, struct scolor *reg)
{
int gray, red, green, blue;

	if (sscanf(s, "%2x%2x%2x%2x", &gray, &red, &green, &blue) != 4)
		return 1;
	reg->red   = red;
	reg->green = green;
	reg->blue  = blue;
	reg->gray  = gray;
	return 0;
}


int scan_color(s, reg)
char *s;
struct scolor *reg;
{
extern int opterr;
int err;

	switch (strlen(s)) {
	case 2:
		err = scan2(s, reg);
		break;
	case 3:
		err = scan3(s, reg);
		break;
	case 5:
		err = scan5(s, reg);
		break;
	case 6:
		err = scan6(s, reg);
		break;
	case 8:
		err = scan8(s, reg);
		break;
	default:
		err = 1;
		break;
	}

	if (err) {
		fprintf(stderr, "%s: illegal color %s\n", progname, s);
		opterr++;
		return(FALSE);
	}

	return(TRUE);
}


struct font *readfont(fnam)
char *fnam;
{
struct font *f;
struct stat st;
int fd;
int n;
int size;
char buf[200];

	if (stat(fnam, &st) < 0) {
		sprintf(buf, "%s: can't stat %s", progname, fnam);
		perror(buf);
		return(NULL);
	}

	size = st.st_size;

	if ((f = (struct font *) malloc(size)) == NULL) {
		fprintf(stderr, "%s: out of memory\n", progname);
		return(NULL);
	}

	if ((fd = open(fnam, 0)) < 0) {
		sprintf(buf, "%s: can't open file %s", progname, fnam);
		perror(buf);
		return(NULL);
	}

	if ((n = read(fd, f, size)) != size) {
		fprintf(stderr, "%s: read: expected %d, got %d\n", progname,
								size, n);
		perror("bad read on file");
		close(fd);
		return(NULL);
	}

	close(fd);

	if (f->f_length != size) {
		fprintf(stderr, "%s: font is corrupt\n", progname);
		return(NULL);
	}

	return(f);
}



setfont(fnam)
char *fnam;
{
struct font *f;
char buf[200];

	if ((f = readfont(fnam)) != NULL) {
		if ( ioctl(0, SIOCSETFONT, f) < 0) {
			sprintf(buf, "%s: can't set font", progname);
			perror(buf);
			return(1);
		} else
			return(0);
	}

	return(1);
}


