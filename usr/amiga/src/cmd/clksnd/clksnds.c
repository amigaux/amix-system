#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>

#include "say.h"


	/* Note that order is not important except last file entry must
	 * be last (and the IDX defines must be adjusted) */
struct SND {
	char	*name;
	int	spos;
	int	slen;
} snds[] = {
	{"one", 3000, 6000},
	{"two", 11500, 5500},
	{"three", 20500, 5500},
	{"four", 31000, 5000},
	{"five", 41000, 5000},
	{"six", 51000, 3000},
	{"seven", 60000, 5000},
	{"eight", 69000, 4000},
	{"nine", 77500, 6000},
	{"eleven", 98500, 6500},
	{"twelve", 107000, 7000},
	{"thirteen", 116500, 7000},
	{"fourteen", 124500, 7500},
	{"fifteen", 133500, 7500},
	{"sixteen", 143000, 7500},
	{"seventeen", 152000, 7000},
	{"eighteen", 161000, 7000},
	{"nineteen", 169000, 7000},
	{"ten", 86500, 6000},
	{"twenty", 180000, 7000},
	{"thirty", 189000, 6000},
	{"forty", 197500, 6000},
	{"fifty", 206000, 5500},
	{"am", 219000, 7000},
	{"pm", 230500, 6500},
	{"quar", 200000, 300},
	{"thetimeis", 318000, 14000},
	{"oh", 347000, 6000},
};

#define	ONESIDX		0
#define	TEENSIDX	9
#define	TENSIDX		18
#define	NSNDS	(sizeof snds/(sizeof snds[0]))

static	char	*sbase;

void
loadsnds()
{
    register int sfd;
    register int slens;

    slens = snds[NSNDS-1].spos + snds[NSNDS-1].slen;

    if ((sbase = malloc(slens)) == NULL)
    {
	(void) fprintf(stderr, "%s: can't get memory for sounds!\n", progname);
	exit(1);
    }

    if ((sfd = open(SNDSFILE, O_RDONLY)) < 0)
    {
	(void) fprintf(stderr, "%s: can't open sounds file %s!\n", progname,
		       SNDSFILE);
	exit(1);
    }

    if (read(sfd, sbase, slens) != slens)
    {
	(void) fprintf(stderr, "%s: can't read sounds file %s!\n", progname,
		       SNDSFILE);
	exit(1);
    }

    close(sfd);
}


void
say(char *sn)
{
    register struct SND *sp;

    for (sp = snds; sp < &snds[NSNDS]; sp++)
	if (strcmp(sn, sp->name) == 0)
	{
	    write(1, sbase+sp->spos, sp->slen);
	    return;
	}

    (void) fprintf(stderr, "%s: don't know sound: %s!\n", progname, sn);
    exit(1);
}


void
sayidx(int idx)
{
    register struct SND *sp;

    sp = &snds[idx];
    write(1, sbase+sp->spos, sp->slen);
}


void
saynumber(int n)
{
    if (n < 0 || n > 59)
    {
	(void) fprintf(stderr, "%s: can't say number %d!\n", progname, n);
	exit(1);
    }

    if (n == 0)
	say("oh");
    else if (n >= 1 && n <= 9)
	sayidx(ONESIDX+n-1);
    else if (n >= 11 && n <= 19)
	sayidx(TEENSIDX+n-11);
    else
    {
	sayidx(TENSIDX+n/10-1);

	if (n%10 != 0)
	    sayidx(ONESIDX+n%10-1);
    }
}
