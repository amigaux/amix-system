/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 		PROPRIETARY NOTICE (Combined)
 * 
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 * 
 * 
 * 
 * 		Copyright Notice 
 * 
 * Notice of copyright on this source code product does not indicate 
 * publication.
 * 
 * 	(c) 1986,1987,1988,1989  Sun Microsystems, Inc
 * 	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
 * 	          All rights reserved.
 *  
 */

#ifndef _SYS_VMSYSTM_H
#define _SYS_VMSYSTM_H

#ident	"@(#)head.sys:sys/vmsystm.h	1.8"

/*
 * Miscellaneous virtual memory subsystem variables and structures.
 */

#ifdef _KERNEL
extern int	freemem;		/* remaining blocks of free memory */
extern int	avefree;		/* moving average of remaining free blocks */
extern int	avefree30;		/* 30 sec (avefree is 5 sec) moving average */
extern int	deficit;		/* estimate of needs of new swapped in procs */
extern int	nscan;			/* number of scans in last second */
extern int	desscan;		/* desired pages scanned per second */

/* writable copies of tunables */
extern int	maxpgio;		/* max paging i/o per sec before start swaps */
extern int	lotsfree;		/* max free before clock freezes */
extern int	minfree;		/* minimum free pages before swapping begins */
extern int	desfree;		/* no of pages to try to keep free via daemon */
#endif

#ifdef _KERNEL

#if defined(__STDC__)
extern void vmtotal(void);
extern int valid_va_range(addr_t *, u_int *, u_int, int);
extern int valid_usr_range(addr_t, size_t);
extern int useracc(caddr_t, uint, int);
extern int page_deladd(int, int, rval_t *);
extern void map_addr(addr_t *, u_int, off_t, int);
#else
extern void vmtotal();
extern int valid_va_range();
extern int valid_usr_range();
extern int useracc();
extern int page_deladd();
extern void map_addr();
#endif	/* __STDC__ */

#endif	/* _KERNEL */

#endif	/* _SYS_VMSYSTM_H */
