#include <osfcn.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/stropts.h>
#include <termio.h>

struct termio termio = {
	0xd06,			/* iflags */
	0x5,			/* oflags */
	0xbf,			/* cflags */
	0x8e3b,			/* lflags */
	0x3,			/* ldisc */
	3,0x1c,8,0x15,4,0,0,0
};


const char *argv[] = { "install", "/etc/profile", (const char *)0, };
const char *envv[] = { "PATH=/bin:/etc", "HOME=/", "SHELL=/bin/sh", "LOGNAME=root", (const char *)0, };


int main(void)
{
    (void)sigset(SIGHUP, SIG_IGN);
    (void)sigset(SIGINT, SIG_IGN);
    (void)sigset(SIGQUIT, SIG_IGN);
    (void)sigset(SIGTERM, SIG_IGN);

    if (fork()>0)
    {
	if (open("/dev/cage", O_RDONLY))
	{
	    (void)open("/dev/console", O_WRONLY);
	    (void)dup(0);
	    (void)dup(0);
	    perror("cager: open");
	    return 1;
	}
    }

    if ((open("/dev/null", O_RDWR) != 0) ||
	(open("/dev/console", O_RDWR) != 1) ||
	(dup(0) != 2))
	return -1;

    if (ioctl(1, I_FIND, "ldterm") != 1)
	if (ioctl(1, I_PUSH, "ldterm"))
	    perror("I_PUSH(ldterm)");

    while (1)
    {
	int pid = fork();
	int pid2, status;

	switch (pid)
	{
	case -1:
	    perror("init: fork");
	    (void)sleep(5);
	    break;
	case 0:
	    (void)sigset(SIGHUP, SIG_DFL);
	    (void)sigset(SIGINT, SIG_DFL);
	    (void)sigset(SIGQUIT, SIG_DFL);
	    (void)sigset(SIGTERM, SIG_DFL);
	    (void)setpgrp();
	    if (close(0) ||
		(open("/dev/console", O_RDWR) != 0) ||
		close(1) ||
		(dup(0) != 1) ||
		close(2) ||
		(dup(0) != 2) ||
		ioctl(0, TCSETA, &termio))
	    {
		perror("init: console");
		return -1;
	    }
	    if (access(argv[1], 1))
		argv[1] = "-i";
	    (void)execve("/bin/sh", argv, envv);
	    perror("init: exec");
	    (void)sleep(5);
	    return -1;
	default:
	    do
	    {
		pid2 = wait(&status);
		if (pid2 < 0)
		    perror("init: wait");
	    }
	    while (pid2 != pid);
	    printf("init: status 0x%04x\n", status);
	    break;
	}
    }
}
