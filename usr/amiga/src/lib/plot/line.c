#include "plot.h"
#include <stdio.h>


void
line(x0, y0, x1, y1)
short	x0,
	y0,
	x1,
	y1;
{
	short	p[4];

	putchar('l');
	_putpoint(x0, y0);
	_putpoint(x1, y1);
}
