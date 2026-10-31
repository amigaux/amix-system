#include <fcntl.h>
#include <errno.h>
#include <amiga/screen.h>
#include <osfcn.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 * This "random" function "looks" more random on the screen.
 */
static unsigned int Rand(unsigned mod)
{
    static unsigned long x;
    x *= 0x13579;
    x += rand();
    return x % mod;
}


static char *progname;

/*
 * Fatal error exit.
 */
static void barf(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    (void)fprintf(stderr, "%s: ", progname);
    (void)vfprintf(stderr, fmt, ap);
    (void)putc('\n', stderr);
    va_end(ap);

    exit(1);
}


static int sd;				/* Screen descriptor */
static int bd;				/* Bitmap descriptor */
static struct Bitmap bitmap;		/* Bitmap information */


/*
 * Toggle random bits in the bitmap until an input event occurs
 * (a keystroke or mouse motion will exit this routine).
 */
static void sample(void)
{
    int i;
    struct inputevent event;

    (void)fcntl(sd, F_SETFL, O_NDELAY);	/* Request non-blocking reads */
    while ((i=read(sd, &event, sizeof event)) == 0)
    {
	for ( i=0 ; i<10000 ; ++i )
	    bitmap.bpl[0][Rand(bitmap.width * bitmap.height / 8)] ^=
		(1<<(rand()&7));
	for ( i=0 ; i<10000 ; ++i )
	    bitmap.bpl[1][Rand(bitmap.width * bitmap.height / 8)] ^=
		(1<<(rand()&7));
    }

    if (i < 0)
	barf("screen read: %s", strerror(errno));
    else
	(void)printf("i=%d, event=%08x %08x\n", i, event);
}


/*
 * Main function for screen(7A) and screen(3A) sample program.
 */
int main(int argc, char *argv[])
{
    struct scrtype scrtype;
    static struct scolor scolors[4] =
    {
	{ 0x00, 0xff, 0x00, 0x00, },	/* gray,red,green,blue */
	{ 0x55, 0x00, 0xff, 0x00, },	/* gray,red,green,blue */
	{ 0xaa, 0x00, 0x00, 0xff, },	/* gray,red,green,blue */
	{ 0xff, 0x00, 0x00, 0x00, },	/* gray,red,green,blue */
    };

    progname = argv[0];

    scrtype.flags = SF_LESSRES;
    scrtype.type = 0;
    scrtype.dispx = 800;
    scrtype.dispy = 600;
    scrtype.dispz = 2;
    scrtype.modes = 0;

    sd = OpenScreen("Sample screen program", &scrtype, 0);
    if (sd < 0)
	barf("OpenScreen: %s", ScreenError());
    printf("Got %s\n", ScreenDeviceName(sd));

    bd = NewBitmap(sd, &bitmap);
    if (bd < 0)
	barf("NewBitmap: %s", ScreenError());
    printf("Did NewBitmap, bd=%d, bpl0=0x%x, bpl1=0x%x\n",
	   bd, bitmap.bpl[0], bitmap.bpl[1]);

    if (ScreenColors(sd, scolors))
	barf("ScreenColors: %s", ScreenError());
    puts("Did ScreenColors");

    if (DisplayScreen(sd))
	barf("DisplayScreen: %s", ScreenError());
    puts("Did DisplayScreen");

    sample();

    if (CloseScreen(sd))
	barf("CloseScreen: %s", ScreenError());

    return 0;
}
