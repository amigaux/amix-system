/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_PSW_H
#define _SYS_PSW_H

#ident	"@(#)head.sys:sys/psw.h	11.5"

/*
 * PSW structure.
 */

struct psw {		/* program status word */
		unsigned int    TE      :2;     /* Trace enable.          */
		unsigned int    CM      :1;     /* Current mode. (S-bit)  */
		unsigned int    MIS     :1;     /* Master/Interrupt state */
		unsigned int    rsvd    :1;
		unsigned int    IPL     :3;     /* Interrupt priority.    */
		unsigned int    NZVC    :8;     /* X- Neg - Zero - Ovr - Ca  */
};
typedef unsigned short psw_t;

#define PSWSTRUCT(pswt) (*((struct psw *)&(pswt)))
#define PSWT(pswstruct) (*((psw_t *)&(pswstruct)))

/*
 * MC68020 XNZVC codes
 */
#define PS_C		1
#define PS_V		2
#define PS_Z		4
#define PS_N		8
#define PS_X		16

/*
 * MC68020 mode fields (S-bit polarity)
 */
#define PS_USER         0
#define PS_KERNEL       1
#define PS_TRACE	2

/*
 * MC68020 psw masks for "short" corresponding to psw_t 
*/
#define	PS_IPL		0x700		/* interrupt priority level */
#define	PS_CUR		0x2000		/* current mode. Supervisor if set */

/*	The following is the initial psw for process 0.
*/
#define IPSW    {0,1,0,0,7,0}


/*	The following psw is used by machdep.c/sendsig.
*/

#define SIGPSW    {0,0,0,0,0,0}

extern struct psw sendsig_psw;
extern struct psw p0init_psw;

#endif	/* _SYS_PSW_H */
