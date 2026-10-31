#include <stdio.h>
#include "hstyle.h"
#include "assert.h"


/*
 * Function declarations.
 */
char	*alloc(),			/* non-failing malloc */
	*ralloc(),			/* non-failing realloc */
	*getline(),			/* read line to buffer */
	*mgetline(),			/* read line to allocated buffer */
	*ending();			/* ending of ordinal name */
void	die(),				/* issue error message and exit */
	asfail(),			/* assertion failure routine */
	mmove(),			/* move non-overlapping memory */
	sreopen();			/* non-failing freopen */
FILE	*sopen();			/* non-failing fopen */
