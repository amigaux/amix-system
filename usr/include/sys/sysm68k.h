#ifdef m68k

/*
 * Commands for sysm68k() system call
 */

#define _m68k_SETNAME	56	/* set the nodename */
#define	_m68k_BIURW	92	/* read/write BIU flags field of 
				 * 68881 idle state frame
				 */
#define _m68k_EXOP	93	/* read exception operand field of 68881/68882
				 * idle state frame
				 */
#define	_m68k_AB_STKFRM 94	/* provides access to specific fields in 68020/030
				 * A and B format stack frames
				 */
#define	_m68k_LIMUSER	95	/*
				 * Access to the limited licensing controler
				 */

/*
**	Suboptions for _m68k_LIMUSER
*/
#define EUA_GET_LIM	0	/* Fetch the system user limit */
#define EUA_GET_CUR	1	/* Fetch the current number of users */
#define EUA_REM_USER	3	/* For Internal use only */
#define EUA_ADD_USER	4	/* Register the calling process as LOGIN */
#define EUA_UUCP	5	/* Register the current process as UUCP */
#define EUA_FORK	6	/* For Internal use only */
#define	EUA_GET_LIC	7	/* return the license bits for this process */

#define _m68k_KERNADDR	99	/* physical address of relocated kernel */

/* field indicators for AB_STKFRM command	*/
#define	SSW	0xa	/* Special Status Word	*/
#define	DOB	0x18	/* Data Output Buffer	*/
#define	DIB	0x2c	/* Data Input Buffer	*/
#define	M68KFMT 1	/* index into u.u_sigsave[] of int containing format */

/*
 *	Constants used in sysm68k() BIURW system call processing
 */

#define	EXC_PEND  0x8000000	/* Position of Floating Point
				   Exception Pending Bit in BIU flags	*/


#ifdef _KERNEL

/*
 *	Signal handling constants for 68020
 */

/* The following masks are used to identify the 68020 deferable
 * and non-deferable signals when they are checked in os/sig.c: fsig()
 */

#define	SNODEFER	0x7f8	/* SIGILL thru SIGSEGV must be handled
				   before process execution can be continued,
				   SIGKILL is included since it always kills
				   the process */ 
#define	SDEFER	~SNODEFER	/* All of the other signals */

/* constants used for u_sigflag */

#define USTKCLEAR	0x80000000 /* rte with a 4 word stack frame in ttrap */
#define USTKRESTORE	0x40000000 /* rte with stack frame in u.u_sigsave[]  */ 

extern	char	framesz[];

#else /* not _KERNEL */

extern int sysm68k(int command, ...);

#endif /* _KERNEL */

#endif /* m68k */
