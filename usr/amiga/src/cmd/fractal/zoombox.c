#include <amiga/screen.h>
#include "fractal.h"

static void draw_zoombox(struct screen_attributes *);

void
display_zoombox(struct screen_attributes *screen)
{
    if (screen->zoombox.displayed)
	return;

    draw_zoombox(screen);

    screen->zoombox.displayed = B_TRUE;
}

void
remove_zoombox(struct screen_attributes *screen)
{
    if (!screen->zoombox.displayed)
	return;

    draw_zoombox(screen);

    screen->zoombox.displayed = B_FALSE;
    screen->zoombox.zooming = B_FALSE;
}

/* resize_zoombox Returns the actual delta value used for resizing the box */
void
resize_zoombox(struct screen_attributes *screen, int deltax, int deltay)
{
    boolean_t boxing;

    if ((screen->zoombox.y + screen->zoombox.height + deltay) < 0 ||
	(screen->zoombox.y + screen->zoombox.height + deltay) >=
	screen->bp.height ||
	(screen->zoombox.x + (screen->zoombox.height + deltax)) < 0 ||
	(screen->zoombox.x + (screen->zoombox.height + deltax)) >=
	 screen->bp.width)
	return;

    boxing = screen->zoombox.zooming;
    if (screen->zoombox.displayed)
	remove_zoombox(screen);

    screen->zoombox.width += deltax;
    screen->zoombox.height += deltay;

    display_zoombox(screen);
    screen->zoombox.zooming = boxing;
}

void
move_zoombox(struct screen_attributes *screen, int deltax, int deltay)
{
    boolean_t boxing;

    boxing = screen->zoombox.zooming;
    if (screen->zoombox.displayed)
	remove_zoombox(screen);

    if ((screen->zoombox.x + deltax) < 0)
    {
	screen->zoombox.x = 0;
    }
    else if ((screen->zoombox.x + screen->zoombox.width + deltax) >=
	       screen->bp.width)
    {
	screen->zoombox.x = screen->bp.width - screen->zoombox.width;
    }
    else
    {
	screen->zoombox.x += deltax;
    }

    if ((screen->zoombox.y + deltay) < 0)
    {
	screen->zoombox.y = 0;
    }
    else if ((screen->zoombox.y + screen->zoombox.height + deltay) >=
	       screen->bp.height)
    {
	screen->zoombox.y = screen->bp.height - screen->zoombox.height;
    }
    else
    {
	screen->zoombox.y += deltay;
    }

    display_zoombox(screen);

    screen->zoombox.zooming = boxing;
}

void
draw_zoombox(struct screen_attributes *screen)
{
    int sx, sy, ex, ey;

    if (screen->zoombox.x < 0)
	sx = 0;
    else if (screen->zoombox.x >= screen->bp.width)
	sx = screen->bp.width - 1;
    else
	sx = screen->zoombox.x;

    if (screen->zoombox.y < 0)
	sy = 0;
    else if (screen->zoombox.y >= screen->bp.height)
	sy = screen->bp.height - 1;
    else
	sy = screen->zoombox.y;
	
    ex = sx + screen->zoombox.width;
    ey = sy + screen->zoombox.height;

    if (ex < 0)
	ex = 0;
    else if (ex >= screen->bp.width)
	ex = screen->bp.width - 1;

    if (ey < 0)
	ey = 0;
    else if (ey >= screen->bp.height)
	ey = screen->bp.height - 1;

    line(&screen->bp, sx, sy, sx, ey, -1);
    line(&screen->bp, sx, ey, ex, ey, -1);
    line(&screen->bp, ex, ey, ex, sy, -1);
    line(&screen->bp, ex, sy, sx, sy, -1);
}
