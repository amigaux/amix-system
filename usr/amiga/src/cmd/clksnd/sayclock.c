/*
** SayClock
**
** First pass by Dave Ballman
** Converted to /dev/audio by Keith Gabryelski
*/

#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include "say.h"

char *progname;

static void sayclock(void);

int
main(int argc, char *argv[])
{
    progname = *argv++; --argc;

    loadsnds();
    sayclock();

    exit(0);
}

static void
sayclock()
{
    struct tm *tp;
    time_t t;

    say("thetimeis");

    time(&t);

    tp = localtime(&t);

    if (tp->tm_hour == 0)
	saynumber(12);
    else if (tp->tm_hour >= 13)
	saynumber(tp->tm_hour - 12);
    else
	saynumber(tp->tm_hour);

    if (tp->tm_min != 0)
    {
	if (tp->tm_min <= 9)
	    say("oh");

	saynumber(tp->tm_min);
    }

    if (tp->tm_hour >= 12)
	say("pm");
    else
	say("am");
}
