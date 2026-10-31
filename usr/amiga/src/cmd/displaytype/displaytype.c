#include <sys/screen.h>

extern char *strrchr();

int main(argc, argv)
int argc;
char *argv[];
{
    int displaytype;
    char *progname = strrchr(argv[0], '/');
    if (progname)
	++progname;
    else
	progname = argv[0];

    displaytype = ioctl(0, SIOCDISPLAYTYPE);
    if (displaytype<0)
    {
	char p[100];
	(void)sprintf(p, "%s: standard input is not an Amiga screen device",
		      progname);
	perror(p);
	return 1;
    }
    if (!strcmp(progname, "pal"))
	return !(displaytype&DT_PAL);
    if (!strcmp(progname, "ntsc"))
	return !(displaytype&DT_NTSC);
    if (!strcmp(progname, "ecs"))
	return !((((displaytype&DT_DENISEVER)>>DTB_DENISEVER) == 0xfc) &&
		 ((((displaytype&DT_AGNUSVER)>>DTB_AGNUSVER) == 0x20) ||
		  (((displaytype&DT_AGNUSVER)>>DTB_AGNUSVER) == 0x30)));
    printf("0x%x\n", displaytype);
    return 0;
}
