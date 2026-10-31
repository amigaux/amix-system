#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <amiga/screen.h>
#include "fractal.h"

static void writegif(FILE *fp, unsigned char **, int, int, int,
		     struct scolor *, int);

void
save_fractal(struct screen_attributes *screen,
	     struct fractal_set *set,
	     char *gifname)
{
    FILE *fp;

    if ((fp = fopen(gifname, "w")) == NULL)
    {
	(void) fprintf(stderr, "%s: couldn't open gif file for writing (%s)\n",
		       progname, strerror(errno));
	return;
    }

    writegif(fp, set->bp.bpl, set->bp.width, set->bp.height, set->bp.depth,
	     screen->scolor, screen->bp.depth);

    fclose(fp);
}

static void
writegif(FILE *fp, unsigned char **bp, int x, int y, int z, 
	 struct scolor *colormap, int csize)
{
    int i, numcols = (1 << z), bytes = x * y, X, Y;
    unsigned char *pic = (unsigned char *)malloc(bytes), *ptr=pic;
    unsigned char rmap[256], gmap[256], bmap[256];

    if (pic == NULL)
    {
	(void) fprintf(stderr, "%s: Couldn't write out fractal, mem error\n",
		       progname);
	return;
    }

    /*
    ** make up color maps.
    */

    if (csize == z)
    {
	for (i=0; i < numcols; i++)
	{
	    rmap[i] = colormap[i].red;
	    gmap[i] = colormap[i].green;
	    bmap[i] = colormap[i].blue;
	}
    }
    else
    {
	int c = 0, j;
	/* Attempt to stretch the colormap */
	for (i=0; i < numcols ; i++)
	{
	    int r, g, b, r1, g1, b1, d = csize - z;
	    int ir, ib, ig;

	    if (i == (numcols - 1))
		j = 0;
	    else
		j = i + 1;

	    r = colormap[i].red;
	    g = colormap[i].green;
	    b = colormap[i].blue;
	    r1 = colormap[j].red;
	    g1 = colormap[j].green;
	    b1 = colormap[j].blue;

	    ir = (r1 - r) / d;
	    ig = (g1 - g) / d;
	    ib = (b1 - b) / d;

	    for (j=0; j < d; j++)
	    {
		rmap[c] = r; r += ir;
		gmap[c] = g; g += ig;
		bmap[c] = b; b += ib;
		c++;
	    }
	}
    }

    /*
    ** make chunky bit buffer
    */

    for (Y=0; Y < y; Y++)
    {
	for (X=0; X < x; X++)
	{
	    *ptr = 0;
	    for (i=0; i < z; i++)
	    {
		*ptr |= (((bp[i][(Y*(x/8))+(X/8)] >> (7-(X%8)) )&1)<<i);
	    }
	    ptr++;
	}
    }


    WriteGIF(fp, pic, x, y, rmap, gmap, bmap, numcols, 0);

    /* be kind ... rewind */
    free(pic);
}
