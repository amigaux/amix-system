#include <amiga/screen.h>
#include "fractal.h"
#include "scrfx.h"

static void draw_pointer(struct Bitmap *bp, struct pointer *pointer);

void
display_pointer(struct Bitmap *bp, struct pointer *pointer)
{
    if (pointer->displayed)
	return;

    draw_pointer(bp, pointer);

    pointer->displayed = B_TRUE;
}

void
remove_pointer(struct Bitmap *bp, struct pointer *pointer)
{
    if (!pointer->displayed)
	return;

    draw_pointer(bp, pointer);

    pointer->displayed = B_FALSE;
}

void
move_pointer(struct Bitmap *bp, struct pointer *pointer, int deltax,
	     int deltay)
{
    remove_pointer(bp, pointer);

    pointer->x += deltax;

    if (pointer->x < pointer->minx)
	pointer->x = pointer->minx;
    else if (pointer->x > pointer->maxx)
	    pointer->x = pointer->maxx;
	
    pointer->y += deltay;
    
    if (pointer->y < pointer->miny)
	pointer->y = pointer->miny;
    else if (pointer->y > pointer->maxy)
	    pointer->y = pointer->maxy;

    display_pointer(bp, pointer);
}

static void
draw_pointer(struct Bitmap *bp, struct pointer *pointer)
{
    int hx, hy, hl;
    int vx, vy, vl;

    hx = pointer->x - (pointer->width / 2);
    hy = pointer->y;
    if (hx < 0)
    {
	hl = pointer->width + hx;
	hx = 0;
    } else if (hx >= bp->width)
    {
	hl = pointer->width - (hx - bp->width);
	hx = bp->width - 1;
    } else
	hl = pointer->width;

    vx = pointer->x;
    vy = pointer->y - (pointer->height / 2);
    if (vy < 0)
    {
	vl = pointer->width + vy;
	vy = 0;
    } else if (vy >= bp->height)
    {
	vl = pointer->height - (vy - bp->height);
	vy = bp->height - 1;
    } else
	vl = pointer->height;


    line(bp, hx, hy, hx + hl, hy, -1);
    line(bp, vx, vy, vx, vy + vl, -1);
}
