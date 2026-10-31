#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <osfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>


static unsigned int random(unsigned mod)
{
    static unsigned long x;
    x *= 0x13579;
    x += rand();
    return x % mod;
}


static const char LISTFILE[] = "/usr/amiga/lib/.list";
static int incurses=0;


static void barf(int status, char *fmt, ...)
{
    va_list ap;
    if (incurses)
	endwin();
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    putc('\n', stderr);
    va_end(ap);
    exit(1);
}


static int sigflag=0;
static void sigcatch(int sig)
{
    ++sigflag;
}


static void doit(char *str)
{
    WINDOW *wp;
    unsigned int xsize = strlen(str) + 1;
    unsigned int ysize = 3;
    unsigned int xpos = random(COLS-xsize);
    unsigned int ypos = random(LINES-ysize);

    wp=newwin(ysize, xsize, ypos, xpos);
    mvwaddstr(wp, 1, 1, str);
    box(wp, 0, 0);
    wrefresh(wp);

    refresh();
    napms(500);
}


int main(int argc, char *argv[], char *envp[])
{
    char buf[100];
    FILE *fp;

    srand(time((time_t *)0));

    fp = fopen(LISTFILE, "r");
    if (!fp)
	barf(1, "can't open %s: %s", LISTFILE, strerror(errno));

    while (!sigflag)
    {
	initscr();
	refresh();
	rewind(fp);
	incurses=1;

	while (!sigflag && fgets(buf, sizeof buf, fp))
	    doit(buf);
	sleep(2);

	endwin();
	incurses=0;
    }

    return 0;
}
