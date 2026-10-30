/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_TUNEABLE_H
#define _SYS_TUNEABLE_H

#ident	"@(#)head.sys:sys/tuneable.h	11.8"

typedef struct tune {
	int	t_gpgslo;	/* If freemem < t_getpgslow, then start	*/
				/* to steal pages from processes.	*/
	int	t_pad[7];	/* Padding for driver compatibility.    */
	int	t_fsflushr;	/* The rate at which fsflush is run in	*/
				/* seconds.				*/
	int	t_minarmem;	/* The minimum available resident (not	*/
				/* swappable) memory to maintain in 	*/
				/* order to avoid deadlock.  In pages.	*/
	int	t_minasmem;	/* The minimum available swappable	*/
				/* memory to maintain in order to avoid	*/
				/* deadlock.  In pages.			*/
	int	t_flckrec;	/* max number of active frlocks */
} tune_t;

extern tune_t	tune;

/*
 * The following is the default value for t_gpgsmsk.  It cannot be
 * defined in /etc/master or /stand/system due to limitations of the
 * config program.
 */

#define	GETPGSMSK	PG_REF|PG_NDREF

#endif	/* _SYS_TUNEABLE_H */
