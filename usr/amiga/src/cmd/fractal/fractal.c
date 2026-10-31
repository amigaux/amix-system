/*
** fractal - display fractal image on Amiga Unix Screen Device.
**
** Written for Bexley (my son) born April 16, 1991.
*/

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <memory.h>
#include <ctype.h>
#include <errno.h>
#include <amiga/screen.h>
#include "fractal.h"

/*
** Globals
*/

char *progname;
char *gifname = "frac.gif";

static void usage(void);

int computation_pid = -1;

int
main(int argc, char **argv)
{
    int c, error_flag=0;
    static struct fractal_set default_fractal_set =
    {
	MANDELBROT,			/* Default is a Mandelbrot */
	DEFAULT_MANDELBROT_SET,
	10000.0,			/* attractor point */
	128,				/* max interrations */
    };
    static struct fractal_set *fractal_set;
    static struct screen_attributes screen;
    static struct scrtype scrtype =
    {
	SF_LESSRES|SF_LESSPLANES,	/* flags */
	ST_ANY,				/* type */
	2048,				/* width */
	2048,				/* height */
	8,				/* depth */
	0,				/* padding */
	SM_INTERLACE|SM_OVERSCAN,
    };

    progname = *argv;

    /*
    ** Setup shared memory.
    */

    if (grab_shared_header_memory(&fractal_set) < 0)
    {
	(void) fprintf(stderr, "%s: Couldn't get shared memory segment (%s)\n",
		       progname, strerror(errno));
	exit(1);
    }

    *fractal_set = default_fractal_set;

    if (!strcmp(basename(progname), "julia"))
    {
	fractal_set->type = JULIA;
	fractal_set->coordinates = (struct fractal_coordinates)
	    DEFAULT_JULIA_SET;
    }
	
    while((c = getopt(argc, argv,
		      "?JMx:y:i:I:p:P:q:a:e:H:W:D:h:w:d:m:f:bBF:")) != EOF)
    {
	switch(c)
	{
	case 'J':
	    fractal_set->type = JULIA;
	    fractal_set->coordinates =
		(struct fractal_coordinates)DEFAULT_JULIA_SET;
	    break;

	case 'P':
	    fractal_set->coordinates.power =
		(unsigned int)strtoul(optarg, NULL, 0);
	    break;

	case 'M':
	    fractal_set->type = MANDELBROT;
	    fractal_set->coordinates =
		(struct fractal_coordinates)DEFAULT_MANDELBROT_SET;
	    break;

	case 'x':			/* center X */
	    fractal_set->coordinates.x = strtod(optarg, NULL);
	    break;

	case 'y':			/* center Y */
	    fractal_set->coordinates.y = strtod(optarg, NULL);
	    break;

	case 'i':			/* x increment */
	    fractal_set->coordinates.x_increment = strtod(optarg, NULL);
	    break;

	case 'I':			/* y incrmeent */
	    fractal_set->coordinates.y_increment = strtod(optarg, NULL);
	    break;

	case 'p':
	    fractal_set->coordinates.p = strtod(optarg, NULL);
	    break;

	case 'q':
	    fractal_set->coordinates.q = strtod(optarg, NULL);
	    break;

	case 'a':
	    fractal_set->attractor_point = (int)strtol(optarg, NULL, 0);
	    break;

	case 'e':
	    fractal_set->max_interrations = (int)strtol(optarg, NULL, 0);
	    break;

	case 'w':			/* screen width */
	    scrtype.dispx = (unsigned short)strtoul(optarg, NULL, 0);
	    break;

	case 'h':			/* screen height */
	    scrtype.dispy = (unsigned short)strtoul(optarg, NULL, 0);
	    break;

	case 'd':			/* screen depth */
	    scrtype.dispz = (unsigned short)strtoul(optarg, NULL, 0);
	    break;

	case 'f':
	    scrtype.flags = (unsigned short)strtoul(optarg, NULL, 0);
	    break;

	case 'm':
	    scrtype.modes = strtoul(optarg, NULL, 0);
	    break;

	case 'H':			/* computed height */
	    fractal_set->bp.height = (unsigned short)strtod(optarg, NULL);
	    break;

	case 'W':			/* computed width */
	    fractal_set->bp.width = (unsigned short)strtod(optarg, NULL);
	    break;

	case 'D':			/* computed depth */
	    fractal_set->bp.depth = (unsigned char)strtoul(optarg, NULL, 0);
	    if (fractal_set->bp.depth < 1 || fractal_set->bp.depth > MAXBPL)
	    {
		(void) fprintf(stderr,
			       "%s: invalid depth: %d.  Valid range: 1 - %d\n",
			       progname, fractal_set->bp.depth, MAXBPL);
		error_flag++;
	    }
	    break;

	case 'B':			/* batch mode no display */
	    screen.nodisplay = B_TRUE;
	    /* fallthrough case */
	case 'b':			/* batch display */
	    screen.batch = B_TRUE;
	    break;

	case 'F':			/* gif filename */
	    gifname = optarg;
	    break;

	case '?':
	    error_flag++;
	    break;
	}

	if (error_flag)
	    usage();
    }

    if (screen.nodisplay == B_FALSE)
    {
	if ((screen.sd = OpenScreen("Fractal Generator", &scrtype, 0)) < 0)
	{
	    (void) fprintf(stderr, "%s: Couldn't open screen device (%s)\n",
			   progname, ScreenError());

	    exit(1);
	}

	(void) fcntl(screen.sd, F_SETFL, O_NDELAY);

	memset(&screen.bp, 0, (size_t) sizeof(screen.bp));
    
	if (NewBitmap(screen.sd, &screen.bp) == -1)
	{
	    (void) fprintf(stderr, "%s: NewBitmap(%d, ...) failed (%s)\n",
			   progname, screen.sd, ScreenError());
	
	    goto CloseAndBarf;
	}
    
	if (set_colors(&screen) < 0)
	    goto CloseAndBarf;
    
	if (DisplayScreen(screen.sd))
	{
	    (void) fprintf(stderr, "%s: DisplayScreen(%d) (%s)\n",
			   progname, screen.sd, ScreenError());
	
	CloseAndBarf:
	    if (CloseScreen(screen.sd))
		(void) fprintf(stderr, "%s: CloseScreen(%d) failed (%s)\n",
			       progname, screen.sd, ScreenError());

	    exit(1);
	}

	screen.size = (screen.bp.height * screen.bp.width) / 8;
	screen.x = screen.y = 0;
    }
    else
    {
	/* default just incase none were given */
	screen.bp.depth = 4;
	screen.bp.width = 640;
	screen.bp.height = 400;
    }

    if (fractal_set->bp.depth == 0)
	fractal_set->bp.depth = screen.bp.depth;

    if (fractal_set->bp.width == 0)
	fractal_set->bp.width = screen.bp.width;

    if (fractal_set->bp.height == 0)
	fractal_set->bp.height = screen.bp.height;

    fractal_set->size = (fractal_set->bp.width * fractal_set->bp.height / 8);

#if WHATTHEHECKWASITHINKING
    /* Scale fractal to make it look correct on any screen ratio */

    fractal_set->coordinates.x_increment /= 
	((double)fractal_set->bp.height / (double)fractal_set->bp.width);
#endif

    if (grab_shared_image_memory(fractal_set) < 0)
    {
	(void) fprintf(stderr, "%s: Couldn't get shared memory segment (%s)\n",
		       progname, strerror(errno));

	exit(1);
    }

    computation_pid = fork_computation_process(fractal_set, &screen);
    if (computation_pid == -1)
    {
	(void) fprintf(stderr, "%s: Couldn't fork computation process (%s)\n",
		       progname, strerror(errno));
	clean_exit(1);
    }

    if (screen.nodisplay == B_FALSE)
    {
	screen.pointer.x = screen.bp.width / 2;
	screen.pointer.y = screen.bp.height / 2;
	screen.pointer.width = 20;
	screen.pointer.height = 20;
	screen.pointer.minx = screen.pointer.miny = 0;
	screen.pointer.maxx = screen.bp.width - 1;
	screen.pointer.maxy = screen.bp.height - 1;
	screen.color_cycle = B_FALSE;
    }

    start_fractal_computation(fractal_set, &screen);

    for (;;)
    {
	boolean_t fractal_done = B_FALSE;

	fractal_done = update_screen(fractal_set, &screen);

	if (fractal_done && screen.batch)
	{
	    save_fractal(&screen, fractal_set, gifname);
	    exit(0);
	}

	if (screen.batch == B_FALSE)
	    process_input_events(fractal_set, &screen, fractal_done);
    }
}

static void
usage()
{
    /*
    ** I never kissed a bear,
    ** I never kissed a goon,
    ** but I can shake a chicken
    ** in the middle of the room.
    **		- Wanda Jackson
    */
    (void) fprintf(stderr,
		   "usage: %s [-?JMBb] [-F filename] [-{xyiIpq} float] "
		   "[-{eawhdfmWHDP} int]\n", progname);

    exit(1);
}

void
clean_exit(int code)
{
    if (computation_pid != -1)
	(void) kill(computation_pid, SIGTERM);

    exit(1);
}
