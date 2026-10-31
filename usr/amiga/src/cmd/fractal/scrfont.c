/*
** The font code was written by Dave Ballam for scrmenu.  He snarfed
** it from the console driver.
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <amiga/screen.h>
#include "scrfont.h"

struct font *
openfont(char *fn)
{
    register int ffd;
    struct stat sbuf;
    struct font *fp;

    if ((ffd = open(fn, O_RDONLY)) < 0)
	return(NULL);

    if (fstat(ffd, &sbuf) < 0)
    {
	close(ffd);
	return(NULL);
    }

    if ((fp = malloc(sbuf.st_size)) == NULL)
    {
	close(ffd);
	return(NULL);
    }

    if (read(ffd, fp, sbuf.st_size) != sbuf.st_size)
    {
	close(ffd);
	free(fp);
	return(NULL);
    }

    close(ffd);

    return(fp);
}

void
scrputstrat(struct Bitmap *bp,
	    struct font *fp,
	    int line,
	    int col,
	    register char *str)
{
    register int x, y;

#if 0
    y = (line+1) * fp->f_height;
    x = col * fp->f_width;
#else
    y = line;
    x = col;
#endif

    while (*str)
    {
	scrcon_putch(bp, x, y, fp, *str++, 0);
	x += fp->f_width;
    }
}

void
scrprintfat(struct Bitmap *bp,
	    struct font *fp,
	    int line,
	    int col,
	    char *fmt,
	    ...)
{
    va_list ap;
    char strbuf[256];

    va_start(ap, fmt);

    (void)vsprintf(strbuf, fmt, ap);

    va_end(ap);

    scrputstrat(bp, fp, line, col, strbuf);
}
