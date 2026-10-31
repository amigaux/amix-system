#include <sys/types.h>
#include <sys/signal.h>
#include <amiga/screen.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "fractal.h"

#define	ODD(x)	(x&0x1)

typedef	struct COMPLEX
{
    double	rp;			/* real part */
    double	ip;			/* imaginary part */
} complex_t;

static void julia(struct fractal_set *);
static void mandelbrot(struct fractal_set *);

static complex_t cmul(complex_t, complex_t);

pid_t
fork_computation_process(struct fractal_set *set,
			 struct screen_attributes *screen)
{
    int i;
    pid_t chpid;

    if ((chpid = fork()) > 0 || chpid == -1)
	return chpid;			/* This is the parents */

    for (i=0; i < screen->bp.depth; ++i)
	(void) munmap(screen->bp.bpl[i], screen->size);
    (void) CloseScreen(screen->sd);

    (void) sigset(SIGTERM, SIG_DFL);

    for (;;)
    {
	int i;

	while (set->owned != START_COMPUTATION)
	{
	    if (set->owned == STOP_COMPUTATION)	/* race condition can */
		set->owned = STOPPED;	/* cause this to occur */

	    (void) sleep(1);
	}

	/*
	** Clear image
	*/

	for (i=0; i < set->bp.depth; ++i)
	    (void) memset(set->bp.bpl[i], 0, set->size);

	set->base = 0;
	set->owned = COMPUTING;
	
	if (set->type == JULIA)
	    julia(set);
	else
	    mandelbrot(set);

	set->owned = STOPPED;
    }
    /*NOTREACHED*/
}

/*
**	          2
**	 Z    = Z   + C
**	  n+1    n
**
** where Z and C are complex numbers:
**
**       Z    = X + iY
**
**       C    = P + iQ
**
** thus:
**	               2
**	Z    = (X + iY)  + P + iQ
**	 n+1
**
** or:
**		2    2
**	Z    = X  - Y  + 2XiY + P + iQ
**	 n+1
**
** or:
**	        2    2
**	Z    = X  - Y  + P + i(2XY + Q)
**	 n+1
**
** or:
**	        2    2
**	X    = X  - Y  + P
**	 n+1
**
**	Y    = 2XY + Q
**	 n+1
**
**
** For The Mandelbrot Set we interate P and iQ leaving X and iY fixed.
** For The Julia Sets we interate X and iY leaving P and iQ fixed.
** That is, P and iQ are our screen (x,y) pair in The Mandlebrot Set
** and X and iY are our screen (x,y) pair in The Julia set.
*/

static void
julia(struct fractal_set *set)
{
    int modcol;
    register int count, bit, x, y, i;
    register double X, Y, tmp;
    double keep_X, keep_Y;
    double start_x, start_y;

    modcol = (set->max_interrations / ((1<<set->bp.depth)-1));
    if (modcol == 0)
	modcol = 1;

    /* Compute start (x, y) */
    start_x = set->coordinates.x -
	(set->coordinates.x_increment*set->bp.width / 2);
    start_y = set->coordinates.y -
	(set->coordinates.y_increment*set->bp.height / 2);

    keep_Y = start_y;
    for (y=0; y < set->bp.height; ++y)
    {
	keep_X = start_x;
	for (x=0; x < set->bp.width; )
	{
	    unsigned long thebytes[MAXBPL];

	    (void) memset(thebytes, 0, sizeof(thebytes));

	    for (bit=31; bit >= 0 && x < set->bp.width; --bit)
	    {
		X = keep_X;
		Y = keep_Y;

		for (count=0; count < set->max_interrations; ++count)
		{
		    double tmp = X*X - Y*Y + set->coordinates.p;
		    Y = 2*X*Y + set->coordinates.q;
		    X = tmp;
		    if ((X*X + Y*Y) > set->attractor_point)
			break;
		}

		if (count < set->max_interrations)
		{
		    count = (count / modcol) + 1;

		    for (i=0; i < set->bp.depth; ++i)
			if (count & (1 << i))
			    thebytes[i] |= 1<<bit;
		} /* else { thebytes[0...31]=0; }  */

		keep_X += set->coordinates.x_increment;
		++x;
	    }


	    for (i=0; i < set->bp.depth; ++i)
		*(unsigned long *)(&set->bp.bpl[i][set->base]) |= thebytes[i];

	    set->base += 4;
	    if (set->owned == STOP_COMPUTATION)
		return;			/* Restart computation */
	}

	keep_Y += set->coordinates.y_increment;
    }
}

static void
mandelbrot(struct fractal_set *set)
{
    int modcol;
    register double P, Q;
    register int count, bit, x, y, i;
    double start_x, start_y;
    register complex_t Z, ctmp;
    complex_t K, btmp;

    K.rp = set->coordinates.p;
    K.ip = set->coordinates.q;

    /* Compute start (x, y) */
    start_x = set->coordinates.x - (set->coordinates.x_increment *
				    set->bp.width / 2);
    start_y = set->coordinates.y - (set->coordinates.y_increment *
				    set->bp.height / 2);
    modcol = (set->max_interrations / ((1<<set->bp.depth)-1));

    if (modcol == 0)
	modcol = 1;

    Q = start_y;
    for (y=0; y < set->bp.height; ++y)
    {
	P = start_x;
	for (x=0; x < set->bp.width; )
	{
	    unsigned long thebytes[MAXBPL];

	    (void) memset(thebytes, 0, sizeof(thebytes));

	    for (bit=31; bit >= 0 && x < set->bp.width; --bit)
	    {
		Z = K;

		for (count=0; count < set->max_interrations; ++count)
		{
		    int nodd = 0, power;

		    power = set->coordinates.power;

		    if (power == 0)
		    {
			Z = (complex_t){1,0};
		    }
		    else if (power != 1)
		    {
			while (power > 1)
			{
			    ctmp = Z;

			    if (ODD(power))
				nodd++;
#if 0
			    Z.rp = ctmp.rp*ctmp.rp - ctmp.ip*ctmp.ip;
			    Z.ip = ctmp.rp*ctmp.ip + ctmp.ip*ctmp.rp;
#else
			    Z = cmul(ctmp, ctmp);
#endif
			    power >>= 1;
			}

			while (nodd-- > 0)
			{
			    ctmp = Z;
#if 0
			    Z.rp = K.rp*ctmp.rp - K.ip*ctmp.ip;
			    Z.ip = K.rp*ctmp.ip + K.ip*ctmp.rp;
#else
			    Z = cmul(K, ctmp);
#endif
			}
		    }

		    Z.rp += P;
		    Z.ip += Q;

		    if ((Z.rp*Z.rp + Z.ip*Z.ip) > set->attractor_point)
			break;
		}

		if (count < set->max_interrations)
		{
		    count = (count / modcol) + 1;

		    for (i=0; i < set->bp.depth; ++i)
		    {
			if (count & (1 << i))
			    thebytes[i] |= 1<<bit;
		    }
		} /* else { thebytes[0...31]=0; }  */

		P += set->coordinates.x_increment;
		++x;
	    }

	    for (i=0; i < set->bp.depth; ++i)
		*(unsigned long *)(&set->bp.bpl[i][set->base]) |= thebytes[i];

	    set->base += 4;
	    if (set->owned == STOP_COMPUTATION)
		return;			/* Restart computation */
	}

	Q += set->coordinates.y_increment;
    }
}

static complex_t
cmul(complex_t a, complex_t b)
{
    static complex_t c;
	
    c.rp = a.rp*b.rp - a.ip*b.ip;
    c.ip = a.rp*b.ip + a.ip*b.rp;

    return c;
}
