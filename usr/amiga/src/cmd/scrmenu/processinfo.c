#

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/signal.h>
#include	<sys/fault.h>
#include	<sys/syscall.h>
#include	<sys/procfs.h>
#include	<dirent.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<sys/errno.h>
#include	<sys/fcntl.h>
#include	<sys/mkdev.h>

#include	"scrmenu.h"

#define	SSLEEP	1		/* awaiting an event */
#define	SRUN	2		/* running */
#define	SZOMB	3		/* process terminated but not waited for */
#define	SSTOP	4		/* process stopped by debugger */
#define	SIDL	5		/* intermediate state in process creation */
#define	SONPROC	6		/* process is being run on a processor */
#define SXBRK	7		/* process being xswapped */

#define		PROCDIR	"/proc"
#define		NUMTTYS	16

#define		TIMECMP(t1,t2)	((t1.tv_sec - t2.tv_sec != 0) ? (t1.tv_sec - t2.tv_sec) : (t1.tv_nsec - t2.tv_nsec))

struct proclist {
	prpsinfo_t	pinfo;
	int		usedflag;
	struct proclist	*flink;
	struct proclist	*blink;
};
	
struct	proclist	*nonttyproc = NULL;
struct	proclist	*ttyproc[NUMTTYS];

void *
getmem(register int size) {
	register void *m;

	if ((m = malloc(size)) == NULL) {
		fprintf(stderr,"ERROR -- Can't allocate %d bytes!\n",size);
		exit(1);
	}
	return(m);
}


void
init_proc() {
	register int i;

	if (chdir(PROCDIR) == -1) {
		fprintf(stderr,"ERROR -- Can't change to %s directory!\n",PROCDIR);
		exit(1);
	}
	for (i=0;i<NUMTTYS;i++)
		ttyproc[i] = NULL;
}


static int
isnonttyproc(char *pn) {
	register struct proclist *pp;
	register int pid;

	pid = atoi(pn);
	pp = nonttyproc;
	while (pp != NULL) {
		if (pid == pp->pinfo.pr_pid) {
			pp->usedflag = TRUE;
			return(TRUE);
		}
		pp = pp->flink;
	}
	return(FALSE);
}


static void
checksetttyproc(register struct proclist *pp) {
	register int ttyn;

	ttyn = minor(pp->pinfo.pr_lttydev);
	if (ttyn >= NUMTTYS)
		return;
	if (ttyproc[ttyn] == NULL) {
		ttyproc[ttyn] = pp;
		return;
	}
	if (pp->pinfo.pr_state == SZOMB || pp->pinfo.pr_state == SIDL)
		return;
	if (TIMECMP(pp->pinfo.pr_start,ttyproc[ttyn]->pinfo.pr_start) > 0)
		ttyproc[ttyn] = pp;
}


static void
addtononttyproc(register struct proclist *pp) {

	pp->flink = nonttyproc;
	pp->blink = NULL;
	if (nonttyproc != NULL)
		nonttyproc->blink = pp;
	nonttyproc = pp;
}


static void
checkproc(char *pn) {
	register int pfd;
	extern int errno;
	register struct proclist *pp;
	
	pp = getmem(sizeof *pp);
retry:
	if ((pfd = open(pn,O_RDONLY)) < 0)
		return;
	if (ioctl(pfd,PIOCPSINFO,&(pp->pinfo)) < 0) {
		if (errno == EAGAIN) {
			close(pfd);
			goto retry;
		} else {
			close(pfd);
			return;
		}
	}
	close(pfd);
	pp->usedflag = TRUE;
	if (pp->pinfo.pr_lttydev != PRNODEV && major(pp->pinfo.pr_lttydev) == 0)
		checksetttyproc(pp);
	else
		addtononttyproc(pp);
}


static void
emptyttylist(void) {
	register struct proclist *pp,*opp;
	register int i;

	for (i=0;i<NUMTTYS;i++) {
		if (ttyproc[i])
			free(ttyproc[i]);
		ttyproc[i] = NULL;
	}
}



static void
cleannonttylist(void) {
	register struct proclist *pp,*nxtpp;
	
	pp = nonttyproc;
	while (pp != NULL) {
		if (!(pp->usedflag)) {
			if (pp->blink != NULL)
				(pp->blink)->flink = pp->flink;
			else
				nonttyproc = pp->flink;
			if (pp->flink != NULL)
				(pp->flink)->blink = pp->blink;
			nxtpp = pp->flink;
			free(pp);
			pp = nxtpp;
		} else
			pp = pp->flink;
	}
}


update_procinfo() {
	register int i;
	register struct proclist *pp;
	DIR *dp;
	struct dirent *dep;

	emptyttylist();
	pp = nonttyproc;
	while (pp != NULL) {
		pp->usedflag = 0;
		pp = pp->flink;
	}
	if ((dp = opendir(PROCDIR)) == NULL) {
		fprintf(stderr,"ERROR -- Can't open process dir!\n");
		exit(1);
	}
	while ((dep = readdir(dp)) != NULL) {
		if (!isdigit(*(dep->d_name)))
			continue;
		if (isnonttyproc(dep->d_name))
			continue;
		checkproc(dep->d_name);
	}
	closedir(dp);
	cleannonttylist();
}


prpsinfo_t *
getprocbytty(char *tn) {
	register int ttyn;

	if (strncmp(tn,"console",7) == 0)
		ttyn = 0;
	else if (strncmp(tn,"term/con",8) == 0)
		ttyn = atoi(tn+8);
	if (ttyn > NUMTTYS)
		return(NULL);
	if (ttyproc[ttyn] == NULL)
		return(NULL);
	return(&(ttyproc[ttyn]->pinfo));
		
}
