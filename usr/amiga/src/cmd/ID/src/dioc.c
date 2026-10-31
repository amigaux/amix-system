/*
 *  dioc.c
 *  Issue a DIOC ioctl call on standard input
 *
 */

#include <osfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#if IS_IT_OFFICIAL_YET
#include <sys/dioc.h> /* What will this really be called? */
#else
#ifdef BACKWARD_CHARACTER_CONSTANTS
#define DIOC			'COID'
#define DIOCHARDWARENAME	'WHCD'
#else
#define DIOC			'DIOC'
#define DIOCHARDWARENAME	'DCHW'
#endif
#endif


static const char *const commands[] =
{
    "dioc",
    "hardwarename",
};


#define	NUM_COMMANDS	(sizeof commands / sizeof (char *))


static int streq(const char *a, const char *b)
{
    while (*a && *b)
    {
	if (toupper(*a) != toupper(*b))
	    return 0;
	++a;
	++b;
    }

    return !*a && !*b;
}


static int parse_command(const char *progname, const char *nam)
{
    int i;

    for (i = 0; i < NUM_COMMANDS; ++i)
	if (streq(nam, commands[i]))
	    return i;

    (void)fprintf(stderr, "%s: bad command: %s\n", progname, nam);
    exit(1);
}


static void show_usage(const char *progname)
{
    int i;

    (void)fprintf(stderr, "%s: usage:\n", progname);

    for (i = 0; i < NUM_COMMANDS; ++i)
	fprintf(stderr, "    %2d  or  %s\n", i, commands[i]);

    exit(1);
}


int main(int argc, char *argv[])
{
    int cmd;
    char buf[1024];

    if (argc < 2)
	cmd = 0;			/* default command is SIOC */
    else if (strcmp(argv[1], "?") == 0 ||
	     strcmp(argv[1], "-?") == 0)
	show_usage(argv[0]);
    else if (*argv[1] >= '0' && *argv[1] <= '9')
	cmd = atoi(argv[1]);
    else
	cmd = parse_command(argv[0], argv[1]);

    switch (cmd)
    {
    case 0:		/* DIOC */
	return ioctl(0, DIOC);

    case 1:		/* SIOCFRONT */
	if (ioctl(0, DIOCHARDWARENAME, buf)) {
	    perror("DIOCHARDWARENAME");
	    return 1;
	}
	puts(buf);
	return fclose(stdout) != NULL;

    default:
	fprintf(stderr, "%s: Invalid command: %s\n",
		argv[0], argv[1]);
	return 1;
    }
}
