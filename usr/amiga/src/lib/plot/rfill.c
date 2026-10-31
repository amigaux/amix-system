#include "plot.h"
#include <stdio.h>


void
rfill(x0, y0, x1, y1)
short	x0,
	y0,
	x1,
	y1;
{
	putchar('R');
	_putpoint(x0, y0);
	_putpoint(x1, y1);
}
