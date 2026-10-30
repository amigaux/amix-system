/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_REGSET_H
#define _SYS_REGSET_H

#ident	"@(#)head.sys:sys/regset.h	1.1"

/* General register access (m68k) */

typedef	int	greg_t;
#define	NGREG	18
typedef	greg_t	gregset_t[NGREG];

#define	R_D0	0
#define	R_D1	1
#define	R_D2	2
#define	R_D3	3
#define	R_D4	4
#define	R_D5	5
#define	R_D6	6
#define	R_D7	7
#define	R_A0	8
#define	R_A1	9
#define	R_A2	10
#define	R_A3	11
#define	R_A4	12
#define	R_A5	13
#define	R_A6	14
#define	R_FP	14
#define	R_A7	15
#define	R_SP	15
#define	R_PC	16
#define	R_PS	17

/* Floating-point register access (m68k FPU) */

typedef	struct fpregset {
	int	f_pcr;
	int	f_psr;
	int	f_fpiaddr;
	int	f_fpregs[8][3];
} fpregset_t;

#endif	/* _SYS_REGSET_H */
