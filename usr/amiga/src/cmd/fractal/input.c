#include <sys/time.h>
#include <sys/types.h>
#include <amiga/screen.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "fractal.h"
#include "qualifiers.h"

extern char *progname;

static void process_key_event(struct fractal_set *,
			      struct screen_attributes *,
			      struct inputevent *);
static void process_mouse_event(struct fractal_set *,
				struct screen_attributes *,
				struct inputevent *,
				int);

/*
** process_input_events: process as many screen input events as
** possible.
*/

void
process_input_events(struct fractal_set *set,
		     struct screen_attributes *screen,
		     int block)
{
    int events_read, retval;
    int current_event, last_event;
    struct inputevent event_list[EVENT_LIST_SIZE];
    fd_set readfds, writefds, exceptfds;
    struct timeval *timeout, tout;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);
    FD_SET(screen->sd, &readfds);

    if (screen->color_cycle)
    {
	tout.tv_sec = 0;
	tout.tv_usec = 4000;
	timeout = &tout;
    }
    else
    {
	tout.tv_sec = 1;
	tout.tv_usec = 0;

	timeout = block ? NULL : &tout;
    }

    if (select(screen->sd+1, &readfds, &writefds, &exceptfds, timeout) <=0)
	return;
 

    if ((events_read=read(screen->sd, event_list, sizeof(event_list))) < 0)
    {
	/* Ignore Errors... who cares about them here */
	return;
    }

    last_event = events_read / sizeof(struct inputevent);
    current_event = 0;

    while (current_event < last_event)
    {
	switch(event_list[current_event].type)
	{
	case IETYPE_ASCII:
	    process_mouse_event(set, screen, NULL, HACK_MOUSE_CLEANUP);
	    process_key_event(set, screen, &event_list[current_event]);
	    break;

	case IETYPE_MOUSE:
	    process_mouse_event(set, screen, &event_list[current_event],
				HACK_MOUSE_DELAY_MOTION);
	    break;
	}

	++current_event;
    }

    process_mouse_event(set, screen, NULL, HACK_MOUSE_CLEANUP);
}

/*
** process_key_event: process one screen key event.
*/
static void
process_key_event(struct fractal_set *set,
		  struct screen_attributes *screen,
		  struct inputevent *ie)
{
    static unsigned char lastkeys[4];
    static int lastkey = 0;

    switch ((ie->code)&0xFF)
    {
    case META('q'):			/* quit */
	lastkey = 0;
	clean_exit(0);
	break;

    case META('c'):			/* color program */
    {
	if (fork() == 0)
	{
	    /* fork a process and exec scrcols */
	    execl(COLOR_PROGRAM, "scrcols", ScreenDeviceName(screen->sd),
		  (char *)0);
	    exit(1);
	}
	break;
    }

    case META('j'):			/* Do a julia set */
	if (set->type == MANDELBROT)
	{
	    double start_x, start_y;

	    stop_fractal_computation(set);

	    /* Compute start (x, y) */
	    start_x = set->coordinates.x - (set->coordinates.x_increment *
					    set->bp.width / 2);
	    start_y = set->coordinates.y - (set->coordinates.y_increment *
					    set->bp.height / 2);
	    set->type = JULIA;

	    set->coordinates.x = set->coordinates.p;
	    set->coordinates.y = set->coordinates.q;

	    set->coordinates.p = start_x + (set->coordinates.x_increment *
					    (screen->x + screen->pointer.x));
	    set->coordinates.q = start_y + (set->coordinates.y_increment *
					    (screen->y + screen->pointer.y));

	    screen->x = screen->y = 0;
	    start_fractal_computation(set, screen);
	}
	lastkey = 0;
	break;

    case META('m'):			/* Do a mandelbrot set */
	if (set->type == JULIA)
	{
	    double start_x, start_y;

	    stop_fractal_computation(set);

	    /* Compute start (x, y) */
	    start_x = set->coordinates.x - (set->coordinates.x_increment *
					    set->bp.width / 2);
	    start_y = set->coordinates.y - (set->coordinates.y_increment *
					    set->bp.height / 2);
	    set->type = MANDELBROT;

	    set->coordinates.x = set->coordinates.p;
	    set->coordinates.y = set->coordinates.q;

	    set->coordinates.p = start_x + (set->coordinates.x_increment *
					    (screen->x + screen->pointer.x));
	    set->coordinates.q = start_y + (set->coordinates.y_increment *
					    (screen->y + screen->pointer.y));

	    screen->x = screen->y = 0;
	    start_fractal_computation(set, screen);
	}
	lastkey = 0;
	break;

    case CR:
    case META('z'):			/* zoom in */
	{
	    struct zoombox zb;
	    double start_x, start_y;

	    /*
	    ** And in The End
	    ** The love you take
	    ** is equal to the love
	    **	    ... You Make
	    **		The Beatles
	    */

	    if (!screen->zoombox.displayed || screen->zoombox.height == 0)
	    {
		remove_zoombox(screen);
		return;			/* Don't zoom if no box */
	    }

	    zb = screen->zoombox;

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

	    remove_zoombox(screen);

	    stop_fractal_computation(set);

	    /* Zoom in dude -- Bart Simpson */

		/* Compute start (x, y) */
	    start_x = set->coordinates.x - (set->coordinates.x_increment *
					    (set->bp.width / 2));

	    start_y = set->coordinates.y -
		(set->coordinates.y_increment * (set->bp.height / 2));

	    set->coordinates.x = start_x +
		(screen->x + zb.x + (zb.width / 2)) *
		    set->coordinates.x_increment;

	    set->coordinates.y = start_y +
		(screen->y + zb.y + (zb.height / 2)) *
		    set->coordinates.y_increment;

	    set->coordinates.x_increment =
		(set->coordinates.x_increment * zb.width) / set->bp.width;

	    set->coordinates.y_increment =
		(set->coordinates.y_increment * zb.height) / set->bp.height;

	    screen->x = screen->y = 0;

	    start_fractal_computation(set, screen);
	}
	break;

    case META('Z'):			/* zoom out */
	{
	    struct zoombox zb;
	    double start_x, start_y;

	    if (!screen->zoombox.displayed || screen->zoombox.height == 0)
	    {
		remove_zoombox(screen);
		return;			/* Don't zoom if no box */
	    }

	    zb = screen->zoombox;

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

	    remove_zoombox(screen);

	    stop_fractal_computation(set);

	    /* Zoom in dude -- Bart Simpson */

	    /* Compute start (x, y) */
	    start_x = set->coordinates.x - (set->coordinates.x_increment *
					    (set->bp.width / 2));

	    start_y = set->coordinates.y -
		(set->coordinates.y_increment * (set->bp.height / 2));

	    set->coordinates.x_increment =
		(set->coordinates.x_increment * set->bp.width) / zb.width;

	    set->coordinates.y_increment =
		(set->coordinates.y_increment * set->bp.height) / zb.height;

	    set->coordinates.x =
		(start_x - ((screen->x + zb.x) *
			    set->coordinates.x_increment)) +
				((set->bp.width / 2) *
				 set->coordinates.x_increment);

	    set->coordinates.y =
		(start_y - ((screen->y + zb.y) *
			    set->coordinates.y_increment)) +
				((set->bp.height / 2) *
				 set->coordinates.y_increment);

	    screen->x = screen->y = 0;

	    start_fractal_computation(set, screen);
	}
	break;

    case META('s'):
	save_fractal(screen, set, gifname);
	break;

    case ESC:
	remove_zoombox(screen);
	lastkey = 0;
	break;

    case TAB:
	if (screen->color_cycle)
	    screen->color_cycle = B_FALSE;
	else
	    screen->color_cycle = B_TRUE;
	lastkey = 0;
	break;

    case CSI:
	lastkeys[0] = CSI;
	lastkey = 1;
	break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '?':
	if (lastkey == 1)
	{
	    lastkeys[1] = (ie->code)&0xFF;
	    lastkey = 2;
	}
	else
	    lastkey = 0;
	break;

    case '~':
	if (lastkey == 2)		/* function key or help key? */
	{
	    switch (lastkeys[1])
	    {
	    case '?':			/* Help key */
		break;
	    }
	}
	lastkey = 0;
	break;
	    
    default:
	lastkey = 0;
	break;
    }
}

/*
** process_mouse_event: Process one mouse event.
*/
static void
process_mouse_event(struct fractal_set *set,
		    struct screen_attributes *screen,
		    struct inputevent *ie,
		    int mousehack)
{
    static int DELTAX, DELTAY;
    static unsigned long QUALIFIERS;
    static int down_buttons;

    if (mousehack == HACK_MOUSE_CLEANUP)
    {
	if (DELTAX == 0 && DELTAY == 0)
	    return;			/* optimization */

	if (screen->zoombox.displayed)
	{
	    if (QUALIFIERS&QUALIFIERS_CONTROL)
	    {
		/*
		** Shift key held down when mouse is dragged
		** and a zoombox is in effect.  This means
		** we should move the box.
		*/
		       
		remove_pointer(&screen->bp, &screen->pointer);
		move_zoombox(screen, DELTAX, DELTAY);
		display_pointer(&screen->bp, &screen->pointer);
	    }
	    else if (screen->zoombox.zooming)
	    {
		/* Just resize the zoombox */
		if (QUALIFIERS&QUALIFIERS_SHIFT)
		    resize_zoombox(screen, DELTAX, DELTAY);
		else
		    resize_zoombox(screen, DELTAX + DELTAY, DELTAX + DELTAY);

		    move_pointer(&screen->bp, &screen->pointer,
			     screen->zoombox.x + screen->zoombox.width -
			     screen->pointer.x,
			     screen->zoombox.y + screen->zoombox.height -
			     screen->pointer.y);
		    
	    }
	    else
		move_pointer(&screen->bp, &screen->pointer, DELTAX, DELTAY);
	}
	else if (screen->slide_window)
	{
	    slide_window(set, screen, DELTAX, DELTAY);
	}
	else
	{
	    move_pointer(&screen->bp, &screen->pointer, DELTAX, DELTAY);
	}

	DELTAX = 0;
	DELTAY = 0;
	return;
    }

    if (ie->qualifiers != QUALIFIERS)
    {
	process_mouse_event(set, screen, NULL, HACK_MOUSE_CLEANUP);
	QUALIFIERS = ie->qualifiers;
    }

    switch (ie->class)
    {
    case IECLASS_MOUSEBUTTON:
	{
	    int button, down_p;

	    button = ie->code >> 8;
	    down_p = ie->code & 1;

	    if (down_p)
	    {
		if (down_buttons&(1 << button))
		    return;
		down_buttons |= (1 << button);
	    }
	    else
	    {
		if (!(down_buttons&(1 << button)))
		    return;
		down_buttons &= ~(1 << button);
	    }

	    process_mouse_event(set, screen, NULL, HACK_MOUSE_CLEANUP);

	    switch (button)
	    {
	    case SLIDE_BUTTON:
		if (!screen->zoombox.displayed)
		{
		    screen->slide_window = down_p ? B_TRUE : B_FALSE;
		}
		break;

	    case ZOOM_BUTTON:
		if (!screen->slide_window)
		{
		    if (down_p)
		    {
			if (!screen->zoombox.zooming)
			{
			    if (screen->zoombox.displayed)
				remove_zoombox(screen);

			    screen->zoombox.zooming = B_TRUE;
			    screen->zoombox.x = screen->pointer.x;
			    screen->zoombox.y = screen->pointer.y;
			    screen->zoombox.width = screen->zoombox.height = 0;
			    display_zoombox(screen);
			}
		    }
		    else
			screen->zoombox.zooming = B_FALSE;
		}
	    }
	}
	break;

    case IECLASS_MOUSEMOTION:
	{
	    signed char deltax, deltay;

	    deltax = (signed char)(ie->code>>8)&0xFF;
	    deltay = (signed char)(ie->code&0xFF);

	    DELTAX += deltax;
	    DELTAY += deltay;
	}
	break;
    }
}
