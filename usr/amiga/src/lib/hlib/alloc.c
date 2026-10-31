/*
 * Alloc is an un-failable interface to malloc.
 */
#include "hlib.h"

char	*
alloc(len)
unsigned	len;
{
	register char	*res;
	char	*malloc();

	res = malloc(len);
	if (res == NULL)
		die("Out of space");
	return (res);
}
