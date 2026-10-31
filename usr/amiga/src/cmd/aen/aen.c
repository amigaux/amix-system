/*
** aen - do some personal lance ethernet driver geefing.
**
** -D num	set debug mode.
** -c		Get Configuration.
** -d devname	Set ethernet device to geef with.
** -e addr	Set ethernet address.
** -l addr	Set logical address.
** -m mode	Set Mode.
** -s		Get status.
** -z		Clear status.
** -n		Number of boards
** -S		Silent mode get number of boards.
*/

#include <sys/types.h>
#include <sys/param.h>
#include <memory.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include "aenuser.h"
#include "extern.h"

#define TRUE	1
#define FALSE	0

int aenfd, oldmode, trace, trace_permiscious, packet_hacking;
char *program_name, device_name[MAXPATHLEN];
aen_config_t aen_config;

static int status, get_config, clear_status, debug, setdebug;
static int number_of_boards_p, number_of_boards, mode;
static int set_ethernet_address, set_logical_address, set_mode;
static unsigned char ethernet_address[6], logical_address[8];

static int shutup;

static char *symbolize_mode(unsigned short);
static int get_address(char *, unsigned char *, int);

int
main(int argc, char **argv)
{
    int c, error_flag=0;
    aen_status_t aen_status;

    program_name = *argv;

    while((c = getopt(argc, argv, "nzsSce:l:m:d:D:?tTp")) != EOF)
    {
	switch(c)
	{
	case 'm':			/* set mode */
	    (void) fprintf(stderr, "%s: Option not supported yet\n",
			   program_name);
	    error_flag++;
	    break;

	case 'e':			/* set ethernet address */
	    if (!get_address(optarg,
			     ethernet_address,
			     sizeof(ethernet_address)))
		error_flag++;
	    set_ethernet_address=TRUE;
	    break;

	case 'l':			/* set logical addrees */
	    if (!get_address(optarg,
			     logical_address,
			     sizeof(logical_address)))
		error_flag++;
	    set_logical_address=TRUE;
	    break;

	case 'd':
	    if (optarg[0] == '/')
		strcpy(device_name, optarg);
	    else
		sprintf(device_name, "/dev/%s", optarg);
	    break;

	case 'S':			/* backwards compatibilty */
	    shutup++;
	    /* Fall through case */
	case 'n':
	    number_of_boards_p++;
	    break;

	case 's':
	    status++;
	    break;

	case 'c':
	    get_config++;
	    break;

	case 'z':
	    clear_status++;
	    break;

	case 'p':
	    packet_hacking++;
	    trace++;
	    break;

	case 'T':
	    trace_permiscious++;
	    /* Fall Through case */
	case 't':
	    trace++;
	    break;

	case 'D':
	    debug = strtol(optarg, (char **)NULL, 0);
	    setdebug++;
	    break;

	case '?':
	    error_flag++;
	    break;
	}

	if (error_flag)
	{
	    (void) fprintf(stderr,
			   "usage: %s [-nzsSc?] [-e addr] [-l addr] [-m mode] [-d dev]\n",
			   program_name);
	    exit(1);
	}
    }

    if (device_name[0] == '\0')
	strcpy(device_name, "/dev/aen0");


    if ((aenfd = open(device_name, O_RDONLY)) < 0 && !number_of_boards_p)
    {
	(void) fprintf(stderr, "%s: couldn't open %s (%s)\n",
		       program_name, device_name,
		       strerror(errno));
	return 1;
    }

    if (number_of_boards_p)
    {
	if (aenfd == -1 ||
	    ioctl(aenfd, AEN_NUMBER_OF_BOARDS, &number_of_boards) < 0)
	    number_of_boards = 0;

	if (!shutup)
	    (void) printf("%d\n", number_of_boards);

	return !number_of_boards;
    }

    if (clear_status)
    {
	if (ioctl(aenfd, AEN_CLEAR_STATUS, 0) < 0)
	{
	    (void) fprintf(stderr, "%s: couldn't clear status for %s (%s)\n",
			   program_name, device_name, strerror(errno));
	    return 1;
	}
    }

    if (ioctl(aenfd, AEN_GET_CONFIG, &aen_config) < 0)
    {
	(void) fprintf(stderr,
		       "%s: couldn't get configuration info from %s (%s)\n",
		       program_name, device_name, strerror(errno));
	return 1;
    }

    if (set_ethernet_address)
	memcpy(aen_config.paddress,
	       ethernet_address,
	       sizeof(aen_config.paddress));

    if (set_logical_address)
	memcpy(aen_config.laddress,
	       logical_address,
	       sizeof(aen_config.laddress));

    if (set_mode)
	aen_config.mode = mode;

    if (set_ethernet_address || set_logical_address || set_mode)
    {
	if (ioctl(aenfd, AEN_SET_CONFIG, &aen_config) < 0)
	{
	    (void) fprintf(stderr, "%s: couldn't set configuration: %s (%s)\n",
			   program_name, device_name, strerror(errno));
	    return 1;
	}
    }

    if (ioctl(aenfd, AEN_GET_STATUS, &aen_status) < 0)
    {
	(void) fprintf(stderr, "%s: couldn't get status from %s (%s)\n",
		       program_name, device_name, strerror(errno));
	return 1;
    }

    (void) printf("Device (");

    switch (aen_status.board_state)
    {
    case BOARD_RESET:
	(void) printf("Reset");
	break;

    case BOARD_IN_INIT_CODE:
	(void) printf("Intializing");
	break;

    case BOARD_RUNNING:
	(void) printf("Running");
	break;

    default:
	(void) printf("Unknown State");
	break;
    }

    (void) printf("):\t%s\n", device_name);

    (void) printf("Ethernet Address:\t%s\n",
		  p_ethernet_address(aen_config.paddress));

    if (status)
    {
	(void) printf("Logical Address:\t"
		      "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
		      aen_config.laddress[0], aen_config.laddress[1],
		      aen_config.laddress[2], aen_config.laddress[3],
		      aen_config.laddress[4], aen_config.laddress[5],
		      aen_config.laddress[6], aen_config.laddress[7]);

	(void) printf("Board Address:\t\t0x%x\n", aen_config.board_base);
	(void) printf("Mode:\t\t\t%s\n", symbolize_mode(aen_config.mode));
	(void) printf("Packets Sent:\t\t%u\n", aen_status.packets_sent);
	(void) printf("Packets Received:\t%u\n",
		      aen_status.packets_received);
	(void) printf("Trailers Received:\t%u\n", aen_status.trailers);
	(void) printf("Failed allocb():\t%u\n", aen_status.allocbs_failed);
	(void) printf("Failed putq():\t\t%u\n", aen_status.couldnt_put);
	(void) printf("Memory Errors:\t\t%u\n", aen_status.memory_error);
	(void) printf("Packets Missed:\t\t%u\n", aen_status.miss_error);
	(void) printf("Collision Errors:\t%u\n",
		      aen_status.collision_error);
	(void) printf("Babble Errors:\t\t%u\n", aen_status.babble_error);
	(void) printf("Buffer Errors:\t\t%u\n", aen_status.buffer_error);
	(void) printf("Overflow Errors:\t%u\n", aen_status.overflow);
	(void) printf("Framming Errors:\t%u\n", aen_status.framming);
	(void) printf("CRC Errors:\t\t%u\n", aen_status.crc);
	(void) printf("Bad Starts:\t\t%u\n", aen_status.bad_start);
	(void) printf("Debug:\t\t\t0x%x\n", aen_status.board_debug);
    }

    if (trace)
	trace_packets();

    (void) close(aenfd);

    return 0;
}

static int
get_address(char *input_buffer, unsigned char *output_buffer, int size)
{
    int i;
    unsigned long value;
    char *p;

    if (strchr(input_buffer, ':') == NULL)
    {
	/* Should be of the FF2345AC7823 variety */
	if (strlen(input_buffer) != size * 2)
	    goto error;

	p = input_buffer;
	for (i=0; i < size; ++i)
	{
	    char buf[3];

	    buf[0] = *p++;
	    buf[1] = *p++;
	    buf[2] = '\0';
	    value = strtoul(buf, NULL, 16);
	    if (value > 255)
		goto error;

	    output_buffer[i] = (unsigned char)value;
	}
    }
    else
    {
	/* Should be of the 23:75:77:FF:12:33 variety */
	if ((p = strtok(input_buffer, ":")) == NULL)
	    goto error;

	do
	{
	    value = strtoul(p, NULL, 16);
	    if (value > 255)
		goto error;

	    output_buffer[i++] = (unsigned char)value;
	} while (i != size && (p = strtok(NULL, ":")) != NULL);
    }

    return TRUE;

error:
    (void) fprintf(stderr, "%s: bad address %s\n", program_name,
		   input_buffer);
    return FALSE;
}

static char *
symbolize_mode(unsigned short mode)
{
    static char retstr[64];
    int need_or = 0;

#define CHECK_N_CAT(MASK, SYMBOL)					\
    if (mode&MASK)							\
    {									\
	if (need_or)							\
	    strcat(retstr, "|");					\
	strcat(retstr, SYMBOL);						\
	need_or++;							\
    }

    if (!mode)
	return "NORMAL";

    if (mode&AEN_MODE_PROM)
    {
	strcpy(retstr, "PROM");
	need_or++;
    }

    CHECK_N_CAT(AEN_MODE_INTL, "INTL");
    CHECK_N_CAT(AEN_MODE_DRTY, "DRTY");
    CHECK_N_CAT(AEN_MODE_COLL, "COLL");
    CHECK_N_CAT(AEN_MODE_DTCR, "DTCR");
    CHECK_N_CAT(AEN_MODE_LOOP, "LOOP");
    CHECK_N_CAT(AEN_MODE_DTX, "DTX");
    CHECK_N_CAT(AEN_MODE_DRX, "DRX");

    return retstr;
}
