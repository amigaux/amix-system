#include "plot.h"
#include <stdio.h>


void
point(x, y)
short	x,
	y;
{
	short	p[2];

	putchar('p');
	_putpoint(x, y);
}
