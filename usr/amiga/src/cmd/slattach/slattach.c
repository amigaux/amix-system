/*
 * slattach: Configure a serial line as a SLIP network interface.
 *	Also becomes a demon which holds the stream together
 *	and tears it down if the serial connection is broken.
 *	This can be run at system startup for permanent connections,
 *	or invoked by some sort of dialer program (yet to be written)
 *	for dial-up SLIP.
 */


#include <sys/types.h>
#include <sys/stropts.h>
#include <sys/conf.h>
#include <sys/utsname.h>
#include <errno.h>
#include <signal.h>
#include <termios.h>
#include <osfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#ifdef BACKWARDS_CHARACTER_CONSTANTS
#define SLIP_WHICH_INTERFACE 'hwls'
#else
#define SLIP_WHICH_INTERFACE 'slwh'
#endif


static void catch(int sig)
{
}


static int stty(struct termios *tp, int speed)
{
    tp->c_iflag = 0;
    tp->c_oflag = 0;
    tp->c_lflag = 0;
    tp->c_cc[VMIN]=1;
    tp->c_cc[VTIME]=1;
    tp->c_cflag = CS8|CREAD|HUPCL;
    switch (speed)
    {
    case 50:	tp->c_cflag |= B50; break;
    case 1200:	tp->c_cflag |= B1200; break;
    case 2400:	tp->c_cflag |= B2400; break;
    case 4800:	tp->c_cflag |= B4800; break;
    case 9600:	tp->c_cflag |= B9600; break;
    case 19200:	tp->c_cflag |= B19200; break;
    case 38400:	tp->c_cflag |= B38400; break;
    default:
        fprintf(stderr, "Unknown speed %d\n", speed);
	return 1;
    }
    return 0;
}


int main(int argc, char *argv[])
{
    int n;
    char buf[512];
    struct termios termios;

    if (argc < 3)
    {
	fprintf(stderr, "Usage: %s speed local-address remote-address [ifconfig args]\n", argv[0]);
	return -1;
    }

    if (ioctl(0, TCGETS, &termios) ||
	stty(&termios, atoi(argv[1])) ||
	ioctl(0, TCSETS, &termios))
    {
	fprintf(stderr, "%s: can't set speed - %s\n",
		argv[0], strerror(errno));
	return 1;
    }

    while (!ioctl(0, I_POP, 0))
	;

    if (ioctl(0, I_PUSH, "slipmod"))
    {
	fprintf(stderr, "%s: PUSH of slipmod failed: %s\n",
		argv[0], strerror(errno));
	return 1;
    }

    if ((n=ioctl(0, SLIP_WHICH_INTERFACE)) < 0)
    {
	fprintf(stderr, "%s: SLIP_WHICH_INTERFACE failed: %s\n",
		argv[0], strerror(errno));
	return 1;
    }

    {
	int i;
	char *p = buf;
	p += sprintf(p, "ifconfig sl%d", n);
	for ( i=2 ; i<argc ; ++i )
	    p += sprintf(p, " %s", argv[i]);
	p += sprintf(p, " up");
    }
    if (system(buf))
	fprintf(stderr, "%s: `%s' failed (status %d)\n", argv[0], buf);
    else
    {
	(void)sigset(SIGTERM, catch);
	(void)sigset(SIGINT, catch);
	(void)sigset(SIGHUP, catch);
	pause();
	(void)sigset(SIGHUP, SIG_IGN);

	sprintf(buf, "ifconfig sl%d down", n);
	(void)system(buf);
    }

    while (!ioctl(0, I_POP, 0))
	;

    return 1;
}
