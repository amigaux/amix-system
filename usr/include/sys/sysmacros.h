/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_SYSMACROS_H
#define _SYS_SYSMACROS_H

#ident	"@(#)head.sys:sys/sysmacros.h	11.14"

#include <sys/param.h>

/*
 * Some macros for units conversion
 */

/* Core clicks to segments and vice versa */

#define ctos(x)		(((x) + (NCPS-1)) >> CPSSHIFT)
#define	ctost(x)	((x) >> CPSSHIFT)
#define	stoc(x)		((x) * NCPS)

/* byte address to segment and vice versa  */
#define sgnum(x)	(((unsigned)(x) >> 17) & 0x1fff)
#define stob(x)		((((unsigned)(x)) & 0x1fff) << 17)
#define	btos(x)		((unsigned)(x) >> 17)
#define secnum(x)	((unsigned)(x) >> 30)

/* Core clicks to immu max offset and vice versa */
#define ctomo(x)        ((x)-1)
#define motoc(x)        ((x)+1)
#define motob(x)        (NBPC*motoc(x)-1)

/*
 * Disk blocks (sectors) and bytes.
 */
#define	dtob(DD)	((DD) << SCTRSHFT)
#define	btod(BB)	(((BB) + NBPSCTR - 1) >> SCTRSHFT)
#define	btodt(BB)	((BB) >> SCTRSHFT)

/*
 * Disk blocks (sectors) and pages.
 */
#define NDPP		4	/* Number of disk blocks per page */
#define DPPSHFT		2	/* Shift for disk blocks per page. */
#define	ptod(PP)	((PP) << DPPSHFT)
#define	dtop(DD)	(((DD) + NDPP - 1) >> DPPSHFT)
#define dtopt(DD)	((DD) >> DPPSHFT)

/* clicks to bytes */
#ifdef BPCSHIFT
#define	ctob(x)	((x)<<BPCSHIFT)
#else
#define	ctob(x)	((x)*NBPC)
#endif

/* bytes to clicks */
#ifdef BPCSHIFT
#define	btoc(x)	(((unsigned)(x)+(NBPC-1))>>BPCSHIFT)
#define	btoct(x)	((unsigned)(x)>>BPCSHIFT)
#else
#define	btoc(x)	(((unsigned)(x)+(NBPC-1))/NBPC)
#define	btoct(x)	((unsigned)(x)/NBPC)
#endif

/* common macros */

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) < (b) ? (b) : (a))


/* WARNING: The device number macros defined here should not be used by device 
** drivers or user software. Device drivers should use the device functions
** defined in the DDI/DKI interface (see also ddi.h). Application software should 
** make use of the library routines available in makedev(3). A set of new device 
** macros are provided to operate on the expanded device number format supported
** in SVR4. Macro versions of the DDI device functions are provided for use by
** kernel proper routines only. Macro routines bmajor(), major(), minor(),
** emajor(), eminor(), and makedev() will be removed or their definitions 
** changed at the next major release following SVR4.
*/

/*
 *  In Amiga Unix, there are no "old" device numbers, as we have no "old"
 *  drivers with which to be binary compatible.  Also, there are no
 *  "internal" major numbers, as we don't run on a silly archetecture.
 */

#define L_BITSMAJOR	14	/* # of major device bits */
#define L_BITSMINOR	18	/* # of minor device bits */
#define L_MAXMAJ	0xff
#define L_MAXMIN	0x3ffff

/* major part of device number */
#define	major(x)	(int)((unsigned)((x)>>L_BITSMINOR) & L_MAXMAJ)

/* minor part of device number */
#define	minor(x)	(int)((x) & L_MAXMIN)

#ifdef _KERNEL

/* major part of device number */
#define	getmajor(x)	major(x)
#define	bmajor(x)	major(x)

/* minor part of device number */
#define	getminor(x)	minor(x)

#endif	/* _KERNEL */



/* make a device number */

#define makedevice(x,y)	(unsigned long)(((x)<<L_BITSMINOR) | ((y)&L_MAXMIN))
#define	makedev(x,y)	makedevice(x,y)

#define emajor(x)	getmajor(x)
#define getemajor(x)	getmajor(x)

#define eminor(x)	getminor(x)
#define geteminor(x)	getminor(x)


#define cmpdev(x)	(x)
#define expdev(x)	(x)


/*
 *  Evaluate to true if the process is an RFS server.
 */
#define	RF_SERVER()	(u.u_procp->p_sysid != 0)

/* machine dependent operations - defined for RFS and STREAMS */

#ifdef	pdp11
#define	SALIGN(p)		(char *)(((int)p+1) & ~1)
#define	IALIGN(p)		(char *)(((int)p+1) & ~1)
#define LALIGN(p)		(char *)(((int)p+1) & ~3)
#endif
#ifdef	vax
#define	SALIGN(p)		(char *)(((int)p+1) & ~1)
#define	IALIGN(p)		(char *)(((int)p+3) & ~3)
#define	LALIGN(p)		(char *)(((int)p+3) & ~3)
#endif
#ifdef	u3b2
#define	SALIGN(p)		(char *)(((int)p+1) & ~1)
#define	IALIGN(p)		(char *)(((int)p+3) & ~3)
#define	LALIGN(p)		(char *)(((int)p+3) & ~3)
#endif
#ifdef	m68k
#define	SALIGN(p)		(char *)(((int)p+1) & ~1)
#define	IALIGN(p)		(char *)(((int)p+1) & ~1)
#define	LALIGN(p)		(char *)(((int)p+1) & ~1)
/*
 *	XDR canonical form assumes all data types are represented
 * in a multiple of 4 byte blocks.
*/
#define XDRPAD(p)		(char *)(((int)(p)+3) & ~3)
#endif

#define SNEXT(p)		(char *)((int)p + sizeof (short))
#define INEXT(p)		(char *)((int)p + sizeof (int))
#define LNEXT(p)		(char *)((int)p + sizeof (long))

/*
 * Macros for counting and rounding.
 */
#define howmany(x, y)	(((x)+((y)-1))/(y))
#define roundup(x, y)	((((x)+((y)-1))/(y))*(y))

#endif	/* _SYS_SYSMACROS_H */
