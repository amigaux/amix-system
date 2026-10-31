#include <amiga/screen.h>
#include "scrfx.h"
static void verticalline(struct Bitmap *, int, int, int, int);
static void horizontalline(struct Bitmap *, int, int, int, int);

/*
** point(); Set point (x, y) int Bitmap to color.
*/

inline void
point(struct Bitmap *Bitmap,
      register int x,
      register int y,
      int color)
{
    unsigned int plane, byte, mask;
    unsigned int width = Bitmap->width/8;	/* width in bytes */
    int xorpoint = (color == -1 ? 1 : 0);

    if (x < 0 || y < 0 || y >= Bitmap->height || x >= Bitmap->width)
    {
	return;				/* Point outside of Bitmap */
    }

    byte = (y * width) + (x/8);		/* byte to modify */
    mask = (1 << (7 - (x%8)));		/* bit to modify as byte mask */

    for (plane=0; plane < Bitmap->depth; ++plane, color >>= 1)
    {
	if (xorpoint)
	{
	    Bitmap->bpl[plane][byte] ^= mask;
	}
	else
	{
	    if (color&1)
		Bitmap->bpl[plane][byte] |= mask;
	    else
		Bitmap->bpl[plane][byte] &= ~mask;
	}
    }
}

/* Draw line */
void
line(struct Bitmap *Bitmap, int x1, int y1, int x2, int y2, int color)
{
    int r, tmp;
    int dx, dy;

    dx = (x2 - x1);
    dy = (y2 - y1);

    if (!dy)
    {
	horizontalline(Bitmap, x1, y1, x2, color);
	return;
    }

    if (!dx)
    {
	verticalline(Bitmap, x1, y1, y2, color);
	return;
    }

    if (dx < 0)
    {
	tmp = x1; x1 = x2; x2 = tmp; dx = -dx;
	tmp = y1; y1 = y2; y2 = tmp; dy = -dy;
    }

    if (dy < 0)
    {
	if (dx > -dy)
	{
	    r = dx/2;

	    point(Bitmap, x1++, y1, color);

	    for (; x1 < x2; ++x1)
	    {
		r += dy;

		if (r <= 0)
		{
		    r += dx;
		    --y1;
		}
		point(Bitmap, x1, y1, color);
	    }
	}
	else
	{
	    r = dy/2;

	    point(Bitmap, x1, y1--, color);

	    for (; y1 > y2; --y1)
	    {
		r += dx;

		if (r > 0)
		{
		    r += dy;
		    ++x1;
		}
		point(Bitmap, x1, y1, color);
	    }
	}
    }
    else
    {
	if (dx > dy)
	{
	    r = dx/2;

	    point(Bitmap, x1++, y1, color);

	    for (; x1 < x2; ++x1)
	    {

		r -= dy;

		if (r <= 0)
		{
		    r += dx;
		    ++y1;
		}
		point(Bitmap, x1, y1, color);
	    }
	}
	else
	{
	    r = dy/2;

	    point(Bitmap, x1, y1++, color);

	    for (; y1 < y2; y1++)
	    {
		r -= dx;

		if (r <= 0)
		{
		    r += dy;
		    ++x1;
		}
		point(Bitmap, x1, y1, color);
	    }
	}
    }

}

static void
horizontalline(struct Bitmap *Bitmap, int x1, int y, int x2, int color)
{
    int length;
    unsigned int byte, bs, tmp;
    unsigned int width = Bitmap->width/8;	/* something useful */
    static unsigned char edgetable[8][9] =
    {
	{ 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, },
	{ 0x00, 0x00, 0x40, 0x60, 0x70, 0x78, 0x7C, 0x7E, 0x7F, },
	{ 0x00, 0x00, 0x00, 0x20, 0x30, 0x38, 0x3C, 0x3E, 0x3F, },
	{ 0x00, 0x00, 0x00, 0x00, 0x10, 0x18, 0x1C, 0x1E, 0x1F, },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0C, 0x0E, 0x0F, },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x06, 0x07, },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, },
    };


    if (y < 0 || y >= Bitmap->height)
	return;				/* clip Y */

    if (x1 > x2)
    {
	tmp = x1;
	x1 = x2;
	x2 = tmp;
    }

    if (x1 >= Bitmap->width || x2 < 0)
	return;				/* entire line is off screen */

    if (x1 < 0)
	x1 = 0;			/* clip left */

    if (x2 >= Bitmap->width)
	x2 = Bitmap->width;			/* clip right */

    byte = (y * width) + (x1/8);

    length = x2 - x1;

    if (length <= 0)
	return;				/* zero length line */

    /* <IMPLEMENT> do word writes when possible */

    /* Left edge line detection */
    if ((bs = (x1%8)))
    {
	unsigned char mask = edgetable[bs][(length > 8) ? 8 : (x2%8)];

	putbyte(Bitmap, byte, mask, color);
	byte++;
	length -= 8 - bs;

	if (length <= 0)
	    return;			/* line ends */
    }

    for (; length > 8; byte++, length -= 8)
	putbyte(Bitmap, byte, 0xFF, color);

    if (length <= 0)
	return;			/* line ends */

    /* right edge of line detection */
    putbyte(Bitmap, byte, edgetable[0][length], color);
}

static void
verticalline(struct Bitmap *Bitmap, int x, int y1, int y2, int color)
{
    unsigned char mask;
    unsigned int startbyte, endbyte, tmp;
    unsigned int bytewidth = Bitmap->width/8;	/* something useful */

    if (x < 0 || x >= Bitmap->width)
	return;				/* clip X */

    if (y1 > y2)
    {
	tmp = y1;
	y1 = y2;
	y2 = tmp;
    }

    if (y2 < 0 || y1 >= Bitmap->height)
	return;				/* entire line is off screen */

    if (y1 < 0)
	y1 = 0;				/* clip up */

    if (y2 >= Bitmap->height)
	y2 = Bitmap->height - 1;	/* clip down */

    startbyte = (y1*bytewidth)+(x/8);
    endbyte = (y2*bytewidth)+(x/8);
    mask = 1 << (7 - (x%8));

    while (startbyte < endbyte)
    {
	putbyte(Bitmap, startbyte, mask, color);
	startbyte += bytewidth;
    }
}

void
box(struct Bitmap *Bitmap, int x, int y, int h, int w, int c)
{
    int i;

    for (i=0; i < h; ++i, ++y)
	horizontalline(Bitmap, x, y, x + w, c);
}
