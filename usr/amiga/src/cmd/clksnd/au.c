
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "audio.h"

static void usage(void);

char *progname;

int
main(int argc, char *argv[])
{
    int c, error_flag=0;
    unsigned int tmp;

    progname = *argv;

    while((c = getopt(argc, argv, "?p:v:")) != EOF)
    {
	switch(c)
	{
	case 'p':
	    tmp = (unsigned int)strtoul(optarg, (char **)NULL, 0); 
	    (void) ioctl(1, AUDIO_PERIOD, &tmp);
	    break;
		
	case 'v':
	    tmp = (unsigned int)strtoul(optarg, (char **)NULL, 0); 
	    (void) ioctl(1, AUDIO_VOLUME, &tmp);
	    break;

	case '?':
	    error_flag++;
	    break;
	}

	if (error_flag)
	    usage();
    }

    exit(0);
}

static void
usage()
{
    fprintf(stderr, "usage: %s [-v N] [-p N]\n", progname);
    exit(1);
}

