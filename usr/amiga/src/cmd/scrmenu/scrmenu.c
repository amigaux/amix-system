#

#define	INKERNEL
#include	<stdio.h>
#include	<sys/types.h>
#include	<osfcn.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<sys/mman.h>
#include	<stdarg.h>
#include	<sys/screen.h>
#include	<sys/signal.h>
#include	<sys/fault.h>
#include	<sys/syscall.h>
#include	<sys/procfs.h>
#include	<ctype.h>
#include	<pwd.h>

#include	"scrmenu.h"

#define	CSI		0x9b

#define	NUMINEVENTS	10	/* # of input events to simultaneously read */
#define	FONT		"/usr/amiga/lib/font/newtopaz11"
#define	POS_X_SCREENLIST	15
#define	POS_Y_SCREENLIST	7
#define	SDESCRSIZE		50
#define	MAXCACHEDNAMES		20
#define	MOUSEMOVETHRESH		30

	int		screenfd;
static	int		bid;
static	unsigned char	*bpaddr;
/*static*/ short	xsize,ysize;
struct	bitmap		bm;
struct 	groupinfo	gi;
struct	groupinfo	oldgi;
int			selectedscreen	= 0;
int			nf0screens = 0;
struct	font		*fp;
int			mouseposincr = 0;

struct	font		*openfont(char *);

void		printfat(int,int,char *,...);
void		process_char(unsigned char);
static	int	eioctl(int screenfd,int cmd,void *arg);
static	void	clear_bmap();
static	void	setpixel(int,int);
static	void	clrpixel(int,int);

static struct scolor scolor[] = {
		{  0,   0,   0,   0},
		{255, 255, 255, 255}
	};

char screenname[SCRNAMESIZE] = "Virtual Screen Manager";

main() {

	if ((fp = openfont(FONT)) == (struct font *)NULL) {
		fprintf(stderr,"ERROR -- Can't open font: %s!\n",FONT);
		exit(1);
	}
	init_display();
	init_proc();
        printfat(1,18,"Use the arrow keys to select a screen.");
        printfat(2,15,"Press RETURN to display the selected screen.");
	update_procinfo();
	update_menu();
	process_events();
	exit(0);
}



struct font *
openfont(char *fn) {
	register int ffd;
	struct stat sbuf;

	if ((ffd = open(fn,0)) == ERROR)
		return((struct font *)NULL);
	if (fstat(ffd,&sbuf) == ERROR) {
		close(ffd);
		return((struct font *)NULL);
	}
	if ((fp = malloc(sbuf.st_size)) == NULL) {
		close(ffd);
		return(NULL);
	}
	if (read(ffd,fp,sbuf.st_size) != sbuf.st_size) {
		close(ffd);
		free(fp);
		return(NULL);
	}
	close(ffd);
	return(fp);
}


do_mouse_event(struct inputevent *ip) {
	register int newscr;

	switch(ip->class) {
		case IECLASS_MOUSEMOTION:
			mouseposincr += (signed char)(ip->code);
			while (mouseposincr < -MOUSEMOVETHRESH/2) {
				newscr = selectedscreen - 1;
				if (newscr >= 0 && gi.scr[newscr].group != 0)
					movearrow(newscr,FALSE);
				mouseposincr += MOUSEMOVETHRESH;
			}
			while (mouseposincr > MOUSEMOVETHRESH/2) {
				newscr = selectedscreen + 1;
				if (newscr < MAXSCREENS && gi.scr[newscr].group != 0)
					movearrow(newscr,FALSE);
				mouseposincr -= MOUSEMOVETHRESH;
			}
			break;
		case IECLASS_MOUSEBUTTON:
			/*if (ip->code & 0x1) we now allow both down and up button */
				process_char('\r');
			break;
	}
}


process_events() {
	register int nbytes,updateflag;
	register struct inputevent *ip;
	struct inputevent ies[NUMINEVENTS];

	eioctl(screenfd,SIOCSELBMAP,(char *)SELBMAP_VBWAIT);
	while ((nbytes = read(screenfd,ies,sizeof ies)) > 0) {
		updateflag = FALSE;
		for (ip=ies;ip<(struct inputevent *)((int)ies+nbytes);ip++) {
			switch(ip->type) {
				case IETYPE_MENU:
					updateflag = TRUE;
					break;
				case IETYPE_ASCII:
					process_char(ip->code);
					break;
				case IETYPE_MOUSE:
					do_mouse_event(ip);
					break;
				/*case IETYPE_RAW:*/
				default:
					break;
			}
		}
		if (updateflag) {
			update_procinfo();
			update_menu();
		}
	}
}


void
process_char(register unsigned char c) {
	static unsigned char lastc = 0;
	register int curgrp,newscr;

	if (lastc == CSI) {
		switch (c) {
			case 'A':
				newscr = selectedscreen;
				curgrp = gi.scr[newscr].group;
				while (newscr >= 0 && gi.scr[newscr].group != 0 && gi.scr[newscr].group == curgrp)
					newscr--;
				curgrp = gi.scr[newscr].group;
				while (newscr >= 0 && gi.scr[newscr].group != 0 && gi.scr[newscr].group == curgrp)
					newscr--;
				newscr++;
				movearrow(newscr,FALSE);
				mouseposincr = 0;
				break;
			case 'B':
				newscr = selectedscreen;
				curgrp = gi.scr[newscr].group;
				while (newscr < MAXSCREENS && gi.scr[newscr].group != 0  && gi.scr[newscr].group == curgrp)
					newscr++;
				if (newscr >= MAXSCREENS || gi.scr[newscr].group == 0)
					newscr = selectedscreen;
				movearrow(newscr,FALSE);
				mouseposincr = 0;
				break;
			case 'C':
				newscr = selectedscreen;
				curgrp = gi.scr[newscr].group;
				newscr++;
				if (newscr >= MAXSCREENS || gi.scr[newscr].group == 0 || gi.scr[newscr].group != curgrp)
					newscr--;
				movearrow(newscr,FALSE);
				mouseposincr = 0;
				break;
			case 'D':
				newscr = selectedscreen;
				curgrp = gi.scr[newscr].group;
				newscr--;
				if (newscr < 0 || gi.scr[newscr].group == 0 || gi.scr[newscr].group != curgrp)
					newscr++;
				movearrow(newscr,FALSE);
				mouseposincr = 0;
				break;
			default:
				goto lab1;
		}
	} else {
lab1:
		switch (c) {
			case '\n':
			case '\r':
				(void)ioctl(screenfd,SIOCACTIVATESCREEN,(char *)gi.scr[selectedscreen].id);
				break;
			case 'q':
				/*exit(0)*/;
		}
	}
	lastc = c;

}


same_groupinfo(struct groupinfo *ogip,struct groupinfo *ngip) {
	register int i;

	for (i=0;i<sizeof ngip->scr/sizeof ngip->scr[0];i++) {
		if (ogip->scr[i].group != ngip->scr[i].group)
			return(FALSE);
		if (strncmp(ogip->scr[i].name,ngip->scr[i].name,sizeof ngip->scr[i].name) != 0)
			return(FALSE);
	}
	return(TRUE);
}


drawarrow(register int x,register int y,int setflg) {
	register int i;
	
	for (i=x-10;i<=x;i++)
		if (setflg)
			setpixel(i,y);
		else
			clrpixel(i,y);
	if (setflg) {
		setpixel(x-1,y-1);
		setpixel(x-2,y-2);
		setpixel(x-1,y+1);
		setpixel(x-2,y+2);
	} else {
		clrpixel(x-1,y-1);
		clrpixel(x-2,y-2);
		clrpixel(x-1,y+1);
		clrpixel(x-2,y+2);
	}
}


movearrow(register int w,int updatedesc) {
	register int x,y;
	static lastarrowy = -1;
	int x0,x1;

	x = (POS_X_SCREENLIST) * fp->f_width - 3 ;
	if (lastarrowy != -1)
		drawarrow(x,lastarrowy,0);
	selectedscreen = w;
	y = (POS_Y_SCREENLIST+selectedscreen-nf0screens+1) * fp->f_height + (fp->f_height>>1);
	drawarrow(x,y,1);
	lastarrowy = y;
/*
	if (updatedesc)
		printfat(1,24,"%d",gi.scr[selectedscreen].group);
*/
}



update_selected(struct groupinfo *ogip,struct groupinfo *ngip) {
	register int i,j;

	i = 0;  j = 0;
	while ((i<sizeof ngip->scr/sizeof ngip->scr[0]) && (j<sizeof ogip->scr/sizeof ogip->scr[0])) {
		if (j >= sizeof ogip->scr/sizeof ogip->scr[0])
			if (ngip->scr[i].group != 0) {
				movearrow(i,TRUE);
				return;
			} else {
				movearrow(1,TRUE);
				return;
			}
		if (ngip->scr[i].group == 0) {
			i++;
			continue;
		}
		if (ogip->scr[j].group == 0) {
			j++;
			continue;
		}
		if (ngip->scr[i].group == ogip->scr[j].group) {
			if (strncmp(ogip->scr[i].name,ngip->scr[i].name,sizeof ngip->scr[i].name) != 0) {
				movearrow(i,TRUE);
				return;
			}
			i++;
			j++;
			continue;
		}
		if (ngip->scr[i].group < ogip->scr[j].group) {
			movearrow(i,TRUE);
			return;
		}
		if (ngip->scr[i].group > ogip->scr[j].group) {
			j++;
			continue;
		}
	}
	/*movearrow(1,TRUE);*/
}


struct CACHEDNAME {
	int	cn_uid;
	char	*cn_name;
};
static	struct CACHEDNAME cachednames[MAXCACHEDNAMES];
static	int	ncachednames = 0;
static char *
getusername(int uid) {
	register int i;
	struct passwd *pwdp;
	char *cp;
	extern void *getmem(int);

	for (i=0;i<ncachednames;i++)
		if (cachednames[i].cn_uid == uid)
			return(cachednames[i].cn_name);
	pwdp = getpwuid(uid);
	if (pwdp == NULL)
		cp = "Unknown";
	else
		cp = pwdp->pw_name;
	if (ncachednames >= MAXCACHEDNAMES)
		return(cp);
	cachednames[ncachednames].cn_uid = uid;
	cachednames[ncachednames].cn_name = getmem(strlen(cp)+1);
	strcpy(cachednames[ncachednames].cn_name,cp);
	ncachednames++;
	return(cp);
}


#define	 addtodbuf(c)	{if (dpos < SDESCRSIZE) descbuf[dpos++] = (c);}
void
prtbydesc(char *d,int vpos) {
	extern prpsinfo_t *getprocbytty(char *);
	prpsinfo_t *pp;
	char *fp,*cp;
	char descbuf[SDESCRSIZE+1];
	int fldwidth;
	int dpos = 0;
	char *awaitloginstr = NULL;

	pp = getprocbytty(d);
	for (fp=d;*fp != '\0' && *fp != ':';fp++);
	if (fp != '\0')
		*fp++ = '\0';
	if (pp == NULL || strncmp("/etc/getty",pp->pr_psargs,10) == 0 || strncmp("/etc/getscr",pp->pr_psargs,11) == 0)
		awaitloginstr = "Awaiting Login";
	
			/* If no format, just print ps args */
	if (*fp == '\0') {
		printfat(vpos,POS_X_SCREENLIST+9,"%-*.*s",
				SDESCRSIZE,SDESCRSIZE,
				awaitloginstr ? awaitloginstr : pp->pr_psargs);
		return;
	}
	while (*fp != '\0') {
		if (*fp == '%') {
			if (isdigit(*++fp)) {
				fldwidth = 0;
				while (isdigit(*fp))
					fldwidth = 10*fldwidth + (*fp++ - '0');
			} else
				fldwidth = -1;
			if (*fp == '%') {
				addtodbuf(*fp++);
				continue;
			}
			if (*fp == 'u') {
				if (awaitloginstr)
					cp = "-";
				else
					cp = getusername(pp->pr_uid);	
			} else if (*fp == 'p') {
				int i;
				if (awaitloginstr)
					cp = awaitloginstr;
				else {
					cp = pp->pr_psargs;
					i = strlen(cp);
					if (cp[i-1] == ' ')
						cp[i-1] = '\0';
				}
			} else if (*fp == 't') {
				cp = d;
				if (strncmp(cp,"term/",5) == 0)
					cp += 5;
			} else
				continue;
			fp++;
			while (*cp != '\0' && fldwidth-- != 0) {
				addtodbuf(*cp);
				cp++;
			}
			while (fldwidth-- > 0)
				addtodbuf(' ');
		}
		addtodbuf(*fp++);
	}
	descbuf[dpos] = '\0';
	printfat(vpos,POS_X_SCREENLIST+9,"%-*.*s",SDESCRSIZE,SDESCRSIZE,descbuf);
}


update_menu() {
	register int i,j,lstgrp;
	static int firsttime = TRUE;
	int gotnonf0 = FALSE;
	static int lastlines = -1;

	j = POS_Y_SCREENLIST;
	eioctl(screenfd,SIOCGETGROUPINFO,&gi);

	if (!firsttime && 0/*same_groupinfo(&oldgi,&gi)*/)
		return;
	
	lstgrp = 0;
	nf0screens = 0;
	for (i=0;i<sizeof gi.scr/sizeof gi.scr[0];i++) {
		if (gi.scr[i].group != 0) {
			gotnonf0 = TRUE;
			if (gi.scr[i].group != lstgrp)
				printfat(j,POS_X_SCREENLIST,"Alt-F%d = ",
						gi.scr[i].group);
			else
				printfat(j,POS_X_SCREENLIST,"         ");
			if (strncmp(gi.scr[i].name,"T:",2) == 0) {
				prtbydesc(gi.scr[i].name+2,j++);
			} else {
				printfat(j++,POS_X_SCREENLIST+9,"%-*.*s",
						SDESCRSIZE,SDESCRSIZE,
						gi.scr[i].name);
			}
		} else if (!gotnonf0)
			nf0screens++;
		lstgrp = gi.scr[i].group;
	}
	if (firsttime)
		movearrow(1,TRUE);
	update_selected(&oldgi,&gi);
	firsttime = FALSE;
	oldgi = gi;
		/* Clean rest of screen */
	i = lastlines;  lastlines = j;
	while (j < i)
		printfat(j++,POS_X_SCREENLIST,"         %*s",SDESCRSIZE,"");
}



init_display() {
	register int i;
	register int bp;
	struct scrtype scrtype;
	struct bmap bmap;

	if ((screenfd = open("/dev/screen",O_RDWR)) == ERROR) {
		fprintf(stderr,"ERROR -- Can't get screen!\7\n");
		exit(1);
	}

		/* Allocate initial displays */
	scrtype.dispx = 640;
	scrtype.dispy = 400;
	scrtype.dispz = 1;
	scrtype.flags = SF_MORERES;
	scrtype.modes = SM_NONHEDLEY;
	scrtype.type = 0;
	(void)eioctl(screenfd,SIOCSETTYPE,&scrtype);
	(void)eioctl(screenfd,SIOCGETTYPE,&scrtype);
	xsize = scrtype.dispx;
	ysize = scrtype.dispy;
	bmap.flags = 0;
	bmap.bmapx = xsize;
	bmap.bmapy = ysize;
	bmap.bmapz = 1;
	bid = eioctl(screenfd,SIOCALLOCBMAP,&bmap);
	eioctl(screenfd,SIOCSETCMAP,scolor);

	eioctl(screenfd,SIOCSELBMAP,(void *)bid);
/*
	eioctl(screenfd,SIOCFRONT,0);
	eioctl(screenfd,SIOCACTIVATE,0);
*/
	eioctl(screenfd,SIOCSETGROUP,0);
	bpaddr = (unsigned char *)mmap((caddr_t)0,(xsize*ysize)>>3,
			PROT_READ|PROT_WRITE,MAP_SHARED,
			screenfd,BPADDR(bid,0,0));
	if ((int)bpaddr == ERROR) {
		fprintf(stderr,"ERROR -- mmap failure!\7\n");
		exit(1);
	}
	clear_bmap();
	/*setgroup will be here*/
	eioctl(screenfd,SIOCSETNAME,screenname);
	eioctl(screenfd,SIOCSETINPUTMODE,(void *)SIM_MENU);

	bm.flags = 0;
	bm.width = xsize;
	bm.height = ysize;
	bm.depth = 1;
	bm.offset = 0;
	bm.bpl[0] = bpaddr;
}




static int
eioctl(int screenfd,int cmd,void *arg) {
	register int rv;

	if ((rv = ioctl(screenfd,cmd,arg)) == ERROR) {
		fprintf(stderr,"ERROR -- screen ioctl failed %x, errno=%s!\7\n",cmd,strerror(errno));
		exit(1);
	}
	return(rv);
}


static void
clear_bmap() {
	register unsigned char *cp;
	register int i;

	cp = bpaddr;
	for (i=0;i<(xsize*ysize)>>3;i++)
		*cp++ = 0;
}





static void
setpixel(int x,int y) {
	register int bn;
	unsigned char *cp;

	bn = x + y * xsize;
	cp = bpaddr + (bn>>3);
	*cp |= 0x80>>(bn&0x7);
}



static void
clrpixel(int x,int y) {
	register int bn;
	unsigned char *cp;

	bn = x + y * xsize;
	cp = bpaddr + (bn>>3);
	*cp &= ~(0x80>>(bn&0x7));
}


putstrat(int line,int col,register char *str) {
	register int x,y;

	y = (line+1) * fp->f_height;
	x = col * fp->f_width;
	while (*str) {
		scrcon_putch(&bm,x,y,fp,*str++,0);
		x += fp->f_width;
	}
}

void
printfat(int line,int col,char *fmt,...) {
	va_list ap;
	char strbuf[256];

	va_start(ap,fmt);
	(void)vsprintf(strbuf,fmt,ap);
	va_end(ap);
	putstrat(line,col,strbuf);
}
