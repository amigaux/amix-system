#include <osfcn.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int errno;


char *progname;

int main(int argc, char *argv[])
{
    progname = strrchr(argv[0], '/');
    if (progname)
	++progname;
    else
	progname = argv[0];

    if (progname[0] == 'u')
    {
	/* UMOUNT code */

	if (argc<2)
	{
	    fprintf(stderr, "Usage: %s spec\n", progname);
	    return -1;
	}

	if (umount(argv[1]))
	{
	    (void)fprintf(stderr, "%s: unmount of %s failed: %s\n",
			  progname, argv[1], strerror(errno));
	    return 1;
	}
    }
    else
    {
	int mflag = MS_DATA;
	char *fstyp = 0;
	char *dataptr = 0;
	int datalen = 0;

	/* MOUNT code */

	if (argc<3)
	{
	    fprintf(stderr, "Usage: %s spec dir [mflag [fstyp [data]]]\n",
		    progname);
	    return -1;
	}

	if (argc>3)
	    mflag |= strtol(argv[3], (char **)0, 0);

	if (argc>4)
	    if (isdigit(argv[4][0]))
		fstyp = (char *)strtol(argv[4], (char **)0, 0);
	    else
		fstyp = argv[4];

	if (argc>5)
	    datalen = strlen(dataptr = argv[5]);

	if (mount(argv[1], argv[2], mflag, fstyp, dataptr, datalen))
	{
	    (void)fprintf(stderr, "%s: mount of %s as %s failed: %s\n",
			  progname, argv[1], argv[2], strerror(errno));
	    return 1;
	}
    }

    return 0;
}
