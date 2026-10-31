#include <stdio.h>

/* alloc.c: memory routines which exit on failure */

extern char *malloc(), *realloc(), *calloc();
extern char *progname;

char *Malloc(nbytes)
unsigned nbytes;
{
    char *p = malloc(nbytes);
    if (p==NULL)
    {
	fprintf(stderr, "%s: out of memory\n", progname);
	exit(-4);
    }
    return p;
}


char *Calloc(nelem, elsize)
unsigned nelem, elsize;
{
    char *p = calloc(nelem, elsize);
    if (p==NULL)
    {
	fprintf(stderr, "%s: out of memory\n", progname);
	exit(-4);
    }
    return p;
}


char *Realloc(oldptr, newsize)
char *oldptr;
register unsigned newsize;
{
    register char *ptr;

    if (oldptr)
	ptr=realloc(oldptr, newsize);
    else
	ptr=malloc(newsize);

    if (!ptr)
    {
	fprintf(stderr, "%s: out of memory\n", progname);
	exit(-4);
    }

    return ptr;
}
