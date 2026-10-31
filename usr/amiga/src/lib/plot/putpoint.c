#include "hlib.h"
#include "byteord.h"
#include <stdio.h>


void
_putpoint(x, y)
short	x,
	y;
{
	register int	ch;

	ch = fbyte(x);
	putchar(ch);
	ch = sbyte(x);
	putchar(ch);
	ch = fbyte(y);
	putchar(ch);
	ch = sbyte(y);
	putchar(ch);
}
