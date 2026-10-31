#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <memory.h>
#include <ctype.h>
#include <errno.h>
#include <amiga/screen.h>
#include "fractal.h"

boolean_t
update_screen(struct fractal_set *set, struct screen_attributes *screen)
{
    int base, i;
    int xsize, ysize, basey, y1, y2;
    boolean_t boxing;

    base = set->base;

    if (screen->nodisplay)
	return (base == set->base);

    if (screen->color_cycle)
	cycle_colors(screen);

    if (base != screen->base)
    {
	int ymin, ymax;			/* screen's min and max y we will */
	boolean_t ptr, box;		/*    draw into */

	ymin = screen->base / (set->bp.width / 8);

	if (ymin < screen->y)
	    return;			/* window is in non displayed area */

	ymin -= screen->y;
	ymax = (base / (set->bp.width / 8)) - screen->y;

	if (ymax >= screen->bp.height)
	    ymax = screen->bp.height - 1;

	if ((ptr = screen->pointer.displayed) == B_TRUE)
	{
	    /* Remove pointer if it is in the way */

	    if ((screen->pointer.y - (screen->pointer.height / 2)) <= ymax &&
		(screen->pointer.y + (screen->pointer.height / 2)) >= ymin)
		remove_pointer(&screen->bp, &screen->pointer);
	}

	/* Remove zoombox if it is in the way */

	if ((box = screen->zoombox.displayed) == B_TRUE)
	{
	    struct zoombox zb = screen->zoombox;

	    if (zb.width < 0)
	    {
		/*
		** Grabbed a square from lower right to upper left.
		*/
				
		zb.width = -zb.width;
		zb.height = -zb.height;
		zb.x -= zb.width;
		zb.y -= zb.height;
	    }

	    boxing = screen->zoombox.zooming;
	    if (zb.y <= ymax && (zb.y + zb.height) >= ymin)
		remove_zoombox(screen);
	}

	xsize = min((screen->bp.width+screen->x), (set->bp.width - screen->x));
	basey = screen->base / (set->bp.width / 8);

	for (i=0; i < screen->bp.depth; ++i)
	{
	    for (y1=ymin, y2=ymin + screen->y; y1 < ymax; ++y1, ++y2)
	    {
		memcpy(screen->bp.bpl[i] + (y1 * (screen->bp.width/8)),
		       set->bp.bpl[i] +
		       (y2 * (set->bp.width/8)) + (screen->x/8),
		       (xsize+7)/8);
	    }
	}

	screen->base = base;

	if (ptr)
	    display_pointer(&screen->bp, &screen->pointer);

	if (box)
	{
	    display_zoombox(screen);
	    screen->zoombox.zooming = boxing;
	}
    }
    
    return (base == screen->size);
}

void
start_fractal_computation(struct fractal_set *set,
			  struct screen_attributes *screen)
{
    register int i;

    while (set->owned != STOPPED)
	(void) sleep(1);

    set->owned = START_COMPUTATION;

    if (screen->nodisplay == B_FALSE)
    {
	remove_zoombox(screen);
	remove_pointer(&screen->bp, &screen->pointer);
    }

    /*
    ** Clear screen
    */

    for (i=0; i < screen->bp.depth; ++i)
	(void) memset(screen->bp.bpl[i], 0, screen->size);

    while (set->owned != COMPUTING)
	(void) sleep(1);

    screen->base = 0;

    if (screen->nodisplay == B_FALSE)
    {
	display_pointer(&screen->bp, &screen->pointer);
    }
}

void
stop_fractal_computation(struct fractal_set *set)
{
    set->owned = STOP_COMPUTATION;

    while (set->owned != STOPPED)
	sleep(1);

}

void
slide_window(struct fractal_set *set,
	     struct screen_attributes *screen,
	     int deltax,
	     int deltay)
{
    int i, ysize, xsize, y1, y2;
    int oldx = screen->x, oldy = screen->y;
    boolean_t ptr;

    screen->x += deltax;
    screen->y += deltay;

    if (screen->x < 0 || set->bp.width < screen->bp.width)
	screen->x = 0;
    else if ((set->bp.width - screen->x) < screen->bp.width)
    {
	screen->x = set->bp.width - screen->bp.width;
    }

    if (screen->y < 0 || set->bp.height < screen->bp.height)
	screen->y = 0;
    else if ((set->bp.height - screen->y) < screen->bp.height)
    {
	screen->y = set->bp.height - screen->bp.height;
    }

    if (screen->x == oldx && screen->y == oldy)
	return;				/* optimization */

    /* update screen */
    ysize = min(screen->bp.height, set->bp.height-screen->y);
    xsize = min(screen->bp.width, set->bp.width-screen->x);

    if ((ptr = screen->pointer.displayed) == B_TRUE)
	remove_pointer(&screen->bp, &screen->pointer);

    for (i=0; i < screen->bp.depth; ++i)
    {
	for (y1=0, y2=screen->y; y1 < ysize; ++y1, ++y2)
	{
	    memcpy(screen->bp.bpl[i] + (y1 * (screen->bp.width/8)),
		   (set->bp.bpl[i] + (y2 * (set->bp.width/8))) +
		   (screen->x/8),
		   (xsize+7)/8);
	}
    }

    if (ptr)
	display_pointer(&screen->bp, &screen->pointer);
}
