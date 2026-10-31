#include "plot.h"
#include <stdio.h>


void
move(x, y)
short	x,
	y;
{
	short	p[2];

	putchar('m');
	_putpoint(x, y);
}
