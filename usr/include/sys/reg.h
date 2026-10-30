/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_REG_H
#define _SYS_REG_H

#ident	"@(#)head.sys:sys/reg.h	11.2"
/*
 * Location of the users' stored registers relative to R0.
 * Usage is u.u_ar0[XX].
 *
 * NOTE: all of these are valid for user-level registers.
 *	 only PC and PS are valid during fault/exception handling
 */

#define	D0	0
#define	D1	1
#define	D2	2
#define	D3	3
#define	D4	4
#define	D5	5
#define	D6	6
#define	D7	7
#define	A0	8
#define	A1	9
#define	A2	10
#define	A3	11
#define	A4	12
#define	A5	13
#define	A6	14
#define	A7	15
#define	FP	A6	/* FP == A6  */
#define	SP	A7
#define	PC	16
#define	PS	17	

#endif	/* _SYS_REG_H */
