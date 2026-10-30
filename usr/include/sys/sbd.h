/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_SBD_H
#define _SYS_SBD_H

#ident	"@(#)head.sys:sys/sbd.h	11.5"
/*
 *  Amiga A2500 and A3000 System Data Definitions
 */

/*
 *	Mainstore Memory Space
 */
#define	SPMEM		0x00000000L
extern unsigned long MAINSTORE, VSIZOFMEM; /* setup in amiga/kernel/support.c */

#define KERNELIO(a)	(a)		 /* physical io matches logical */

/*
 *  User address space offsets
 *
 *****************************  NOTE - NOTE  *********************************
 *
 *	 ANY CHANGES THE THE FOLLOWING DEFINES, NEED TO BE REFLECTED IN
 *	    EITHER ml/misc.s, OR ml/ttrap.s, OR BOTH.
 */

#define UVBASE		((unsigned)0x80000000L)
#define UVTEXT		((unsigned)0x80800000L)
#define UVSTKBASE	((unsigned)0xc0800000L)
#define UVUBLK		((unsigned)0xc0000000L)
#define UVSTACK		((unsigned)0xc0020000L)
#define UVSHM		((unsigned)0xc1000000L)
#define UVEND		((unsigned)0xF1000000L)

#endif	/* _SYS_SBD_H */
