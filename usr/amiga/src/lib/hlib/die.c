/*
 * Die simply prints an error message and exits.
 */
#include "hlib.h"


void
die(fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
char	*fmt;
{
	fflush(stdout);
	fprintf(stderr, fmt, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
	fprintf(stderr, "\n");
	exit(1);
}
