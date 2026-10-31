#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <amiga/screen.h>
#include <osfcn.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 * "screen" library for convenient access to Amiga Unix "screen(7A)"
 * device driver.
 *
 * Dependencies:
 *   SCREEN_DEVICE environment variable
 *     contains name of "/dev/screen" device.
 *   SCREEN_DIR environment variable
 *     contains name of "/dev/scr/" directory.
 *   SCREEN_GROUP environment variable
 *     contains numeric string representation of "screen group" (F-key).
 *
 * Provides:
 *   int OpenScreen(char *title, struct scrtype *stp, int flags);
 *     Open a screen on the apropriate display.  The type of screen is
 *     optionally constrained by *stp and flags.  Returns "sfd" (screen
 *     file descriptor).
 *   int ScreenColors(int sfd, struct scolor colors[]);
 *     Set colormap of sfd to colors[].
 *   int NewBitmap(int sfd, struct Bitmap *bp);
 *     Create a new bitmap for sfd.  Appropriate default dimentions
 *     for sfd are if bp->width and bp->height are zero.  Bitmap memory
 *     is mmaped in and bp->bpl[] are filled in with bitplane addresses.
 *     The new bitmap is selected.  Returns "bd" (bit descriptor).
 *   int DisplayScreen(int sfd);
 *     SIOCDISPLAY and SIOCACTIVATE sfd.
 *   char *ScreenDeviceName(int sfd);
 *     Return device name corresponding to sfd, as in "/dev/scr/0".
 *   int CloseScreen(int sfd);
 *     Closes sfd.
 *   char *ScreenError(void);
 *     Return string explaining last failed call.
 *  
 */


static char errorbuf[100];		/* Yuck */
const struct scrtype DefaultScrtype = { SF_LESSRES, 0, 2048, 2048, 4, 0, };


char *ScreenError(void)
{
    return errorbuf;
}


int OpenScreen(const char *title, const struct scrtype *stp, int flags)
{
    int sfd;
    static char *screendevice;

    if (!screendevice && !(screendevice = getenv("SCREEN_DEVICE")))
	screendevice = "/dev/screen";

    if (flags & 0xffff)
    {
	strcpy(errorbuf, "Invalid flags");
	return -1;
    }

    /* Open device. */
    sfd = open(screendevice, O_RDWR);
    if (sfd < 0)
    {
	sprintf(errorbuf, "can't open %s: %s", screendevice, strerror(errno));
	return -1;
    }

    /* Set screen type. */
    if (stp)
	if (ioctl(sfd, SIOCSETTYPE, stp))
	{
	    sprintf(errorbuf, "SIOCSETTYPE %s: %s",
		    screendevice, strerror(errno));
	    close(sfd);
	    return -1;
	}

    /* Set screen group. */
    {
	char *g = getenv("SCREEN_GROUP");
	/* for now, don't SETGROUP if no SCREEN_GROUP in environment. */
	if (g)
	    if (ioctl(sfd, SIOCSETGROUP, g?strtol(g, (char **)0, 0):-1) < 0)
	    {
		sprintf(errorbuf, "SIOCSETGROUP %s: %s",
			screendevice, strerror(errno));
		close(sfd);
		return -1;
	    }
    }

    /* Set screen name. */
    {
	char scrtitle[SCRNAMESIZE];
	strncpy(scrtitle, title?title:ScreenDeviceName(sfd), sizeof scrtitle);
	if (ioctl(sfd, SIOCSETNAME, scrtitle))
	{
#ifdef NAME_MUST_WORK
	    sprintf(errorbuf, "SIOCSETNAME %s: %s",
		    screendevice, strerror(errno));
	    close(sfd);
	    return -1;
#endif
	}
    }

    return sfd;
}


int ScreenColors(int sfd, const struct scolor colors[])
{
    if (ioctl(sfd, SIOCSETCMAP, colors))
    {
	sprintf(errorbuf, "SIOCSETCMAP: %s", strerror(errno));
	return -1;
    }

    return 0;
}


int NewBitmap(int sfd, struct Bitmap *bp)
{
    int bd, i;

    {
	struct bmap bmap;

	if (bp->width && bp->height)
	    bmap.flags = bp->flags;
	else
	{
	    struct scrtype scrtype;
	    if (ioctl(sfd, SIOCGETTYPE, &scrtype))
	    {
		sprintf(errorbuf, "SIOCGETTYPE: %s", strerror(errno));
		return -1;
	    }

	    bmap.flags = 0;
	    bp->width  = scrtype.dispx;
	    bp->height = scrtype.dispy;
	    bp->depth  = scrtype.dispz;
	}

	bmap.bmapx = bp->width;
	bmap.bmapy = bp->height;
	bmap.bmapz = bp->depth;

	bd = ioctl(sfd, SIOCALLOCBMAP, &bmap);
    }

    if (bd < 0)
    {
	sprintf(errorbuf, "SIOCALLOCBMAP: %s", strerror(errno));
	return -1;
    }

    for ( i=0 ; i<bp->depth ; ++i )
    {
	bp->bpl[i] = (unsigned char *)mmap((caddr_t)0,
					   (bp->width * bp->height)/8,
					   PROT_READ|PROT_WRITE, MAP_SHARED,
					   sfd, BPADDR(bd,i,0));
	if (bp->bpl[i] == (unsigned char *)-1)
	{
	    sprintf(errorbuf, "mmap() of bitplanes failed: %s", strerror(errno));
	    return -1;
	}
    }

    if (ioctl(sfd, SIOCSELBMAP, bd))
    {
	sprintf(errorbuf, "SIOCSELBMAP: %s", strerror(errno));
	return -1;
    }

    return bd;
}


int DisplayScreen(int sfd)
{
    if (ioctl(sfd, SIOCGROUPFRONT) &&
	(ioctl(sfd, SIOCFRONT) || ioctl(sfd, SIOCACTIVATE)))
    {
	sprintf(errorbuf, "SIOCGROUPFRONT: %s", strerror(errno));
	return -1;
    }

    return 0;
}


char *ScreenDeviceName(int sfd)
{
    static char *screendir;
    static char buf[100];
    struct stat Stat;

    if (!screendir && !(screendir = getenv("SCREEN_DIR")))
	screendir = "/dev/scr/";

    if (fstat(sfd, &Stat))
    {
	sprintf(errorbuf, "fstat of screen descriptor: %s", strerror(errno));
	return 0;
    }

    sprintf(buf, "%s%d", screendir, SCREENMINOR(Stat.st_rdev));

    return buf;
}


int CloseScreen(int sfd)
{
    return close(sfd);
}
