#include <sys/types.h>
#include <sys/sys3b.h>
#include <libgen.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define TRUE  (1)
#define FALSE (0)

#define	SECONDS_PER_MINUTE	(60)
#define MINUTES_PER_HOUR	(60)
#define HOURS_PER_DAY		(24)
#define SECONDS_PER_HOUR	(SECONDS_PER_MINUTE * MINUTES_PER_HOUR)
#define SECONDS_PER_DAY		(SECONDS_PER_HOUR * HOURS_PER_DAY)

#define	dysize(A) (((A)%4)? 365: 366)	/* number of days per year */

static int dmsize[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#ifndef BEXTIME_FILE
#define BEXTIME_FILE "/etc/clkzone"
#endif /* BEXTIME */

static char *bextime = BEXTIME_FILE;
static char *bexley = ":GMT";		/* default hardware clock time */
static char bexleyTZ[BUFSIZ+3];

static char *progname;
static const char *devclock = "/dev/clock";
static int set_clock, display_clock, set_hardware_clock;

static int write_clock(char *), read_clock(char *);
static time_t read_from_clock_device(void);
static void convert_time(time_t, char *);
static void usage(void);

int
main(int argc, char **argv)
{
    int retval=1, c, error_flag=0;
    char *given_time;
    FILE *fp;
    char buf[BUFSIZ];
    extern char *optarg;

    progname = *argv;

    if (!strcmp(*argv, "getclk"))
    {
	char buffer[11];

	retval = read_clock(buffer);

	if (retval)
	    (void) puts(buffer);

	return !retval;
    }

    if ((fp = fopen(bextime, "r")) != NULL)
    {

	if (fgets(buf, BUFSIZ, fp) != NULL)
	{
	    char *p;

	    p = strtok(buf, " \t\n;");

	    if (strlen(p))
		bexley = buf;
	}
    }

    sprintf(bexleyTZ, "TZ=%s", bexley);

    if (argc == 1)
    {
	time_t thetime = read_from_clock_device();
	char buffer[BUFSIZ*2], tmpbuf[sizeof("MMDDhhmmYY")];

	/*
	** Set the system time to what the hardware thinks is correct.
	** The hardware clock stores time in local time zone for compatibility
	** with AmigaDOS.
	*/
	if (thetime == -1)
	{
	    fprintf(stderr, "%s: couldn't get time from hardware clock (%s)\n",
		    progname, strerror(errno));
	    return 1;
	}

	convert_time(thetime, tmpbuf);
	sprintf(buffer, "%s /usr/bin/date %s", bexleyTZ, tmpbuf);
	system(buffer);

	return 0;
    }

    while((c = getopt(argc, argv, "ps?")) != EOF)
    {
	switch(c)
	{
	case 'p':
	    display_clock++;
	    break;

	case 's':
	    set_clock++;
	    break;

	case '?':
	    error_flag++;
	    break;
	}

	if (error_flag)
	    usage();
    }

    if (optind < argc)
    {
	if (optind == argc - 1)
	{
	    set_hardware_clock++;
	    given_time = argv[optind];
	}
	else
	    usage();
    }

    if (set_clock && set_hardware_clock)
	usage();

    if (set_clock)
    {
	char buffer[11];
	struct tm *tm;
	time_t thetime;

	/*
	** Set the hardware clock to the current system time.
	*/

	thetime = time((time_t)0);
	putenv(bexleyTZ);
	tm = localtime(&thetime);
	++tm->tm_mon;
#ifdef DEBUG
	(void) printf("localtime (from system): %02d%02d%02d%02d%02d\n",
		      tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min,
		      tm->tm_year);
#endif

	(void) sprintf(buffer, "%02d%02d%02d%02d%02d", tm->tm_mon, tm->tm_mday,
		       tm->tm_hour, tm->tm_min, tm->tm_year);

	retval = write_clock(buffer);

	if (!retval)
	    return 1;
    }

    if (set_hardware_clock)
    {
	retval = write_clock(given_time);

	if (!retval)
	    return 1;
    }

    if (display_clock)
    {
	char buffer[11];

	retval = read_clock(buffer);

	if (retval)
	    (void) puts(buffer);
    }

    return !retval;
}

static void
usage()
{
    (void) fprintf(stderr, "usage: %s [-p?] [-s | mmddHHMMYY]\n", progname);
    exit(1);
}

static int
read_clock(char *buffer)
{
    time_t thetime;

    thetime = read_from_clock_device();
    if (thetime == -1)
	return FALSE;

    convert_time(thetime, buffer);

    return TRUE;
}

static void
convert_time(time_t thetime, char *buffer)
{
    int month, days, hours, mins, secs, year;
    int i;
    register int d1;
    long hms, day;

    /* break time into days */

    day = thetime / SECONDS_PER_DAY; /* Number of days */
    hms = thetime % SECONDS_PER_DAY; /* Hours left for this day (in seconds) */

    /* generate hours:minutes:seconds */

    d1 = hms / 60;		/* hours left for this day (in minutes) */
    secs = hms % 60;		/* seconds in this day */

    hours = d1 / 60;		/* hours in this day */
    mins = d1 % 60;		/* minutes in this day */

    /* year number */

    for (year=70; day >= dysize(year); year++)
	day -= dysize(year);

    days = day;				/* days left in this year */

    /* generate month */

    if (dysize(year) == 366)		/* Leap year? */
	dmsize[1] = 29;			/*   then February has a leap day. */

    for (month=0; days >= dmsize[month]; ++month)
	days -= dmsize[month];
    ++month;
    ++days;

    dmsize[1] = 28;

    (void) sprintf(buffer, "%02d%02d%02d%02d%02d", month, days, hours, mins,
		   year);
}

static time_t
read_from_clock_device()
{
    time_t thetime;

    int fd = open(devclock, O_RDONLY);

    if (fd < 0)
    {
	(void) fprintf(stderr, "%s: Couldn't access clock driver: %s (%s)\n",
		       progname, devclock, strerror(errno));

	return -1;
    }

    if (read(fd, &thetime, sizeof(time_t)) < 0)
    {
	(void) fprintf(stderr,
		       "%s: Couldn't get current time from hardware (%s)\n",
		       progname, strerror(errno));

	close(fd);
	return -1;
    }

    close(fd);

    return thetime;
}

static int
write_clock(char *buffer)
{
    char *p;
    time_t thetime;
    int fd, month, days, hours, mins, year;
    int i;

    fd = open(devclock, O_WRONLY);

    if (fd < 0)
    {
	(void) fprintf(stderr, "%s: Couldn't access clock driver: %s (%s)\n",
		       progname, devclock, strerror(errno));

	return FALSE;
    }

    if (sscanf(buffer, "%02d%02d%02d%02d%02d", &month, &days, &hours, &mins,
	       &year) != 5)
    {
	(void) fprintf(stderr, "%s: Bad date format \"%s\"\n", progname,
		       buffer);
	close(fd);
	return FALSE;
    }

    thetime = 0;

    /* Add year */

    if (year < 70)
	year += 100;			/* ooooooohhhhh */

    for (i=70; i < year; i++)
	thetime += dysize(i) * SECONDS_PER_DAY;

    /* Add in month */
    if (dysize(year) == 366)
	dmsize[1] = 29;

    while (--month)
        thetime += dmsize[month-1] * SECONDS_PER_DAY;

    dmsize[1] = 28;

    /* Add in days */
    thetime += (days-1) * SECONDS_PER_DAY;

    /* Add in hours */
    thetime += hours * SECONDS_PER_HOUR;

    /* Add in minutes */
    thetime += mins * SECONDS_PER_MINUTE;

#ifdef DEBUG
    {
	struct tm *tm;

	/* We do a gmtime() here because we are setting the hardware clock */
	/* to local time.  Weird yes but this is how AMIGADOS does it. */
	tm = gmtime(&thetime);
	++tm->tm_mon;
	(void) printf("time (to hardware): buffer=`%s', "
		      "thetime=`%02d%02d%02d%02d%02d'\n", buffer,
		      tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min,
		      tm->tm_year);
    }
#endif

    if (write(fd, &thetime, sizeof(time_t)) < 0)
    {
	(void) fprintf(stderr, "%s: Couldn't set current time (%s)\n",
		       progname, strerror(errno));

	close(fd);
	return FALSE;
    }

    close(fd);

    return TRUE;
}

