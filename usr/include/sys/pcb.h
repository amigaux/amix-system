/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)head.sys:sys/pcb.h	11.5"

#ifndef _SYS_PCB_H
#define _SYS_PCB_H
/*
 * MC68020 process control block
 */

/*
 * Process Control Block (PCB) 
 */

typedef struct ipcb
	{
	psw_t psw;		/* initial PSW */
	unsigned short pcwords[2]; /* initial program counter (PC) */
	unsigned short spwords[2]; /* initial stack pointer (SP) */
} ipcb_t;

typedef struct pcb
	{
	int regsave[16];        /* registers USP/D0-D7/A0-A6 save area */
	psw_t psw;		/* PSW */
	unsigned short pcMSW;   /* PC MSW */
	unsigned int pcLSW:16;	/* PC LSW */
	unsigned int format:4;  /* Format word */
	unsigned int vector:12; /* Vector # */
	unsigned int ssw;       /* Special Status Word */
	unsigned int pipe;      /* Pipe B, Pipe C */
	unsigned int datafault; /* Data Cycle Fault Address */
	unsigned int fill[4];
	unsigned int pcfault;   /* Stage B address (scanpc) */
	unsigned int fill2[13];
} pcb_t;

/* Warning: GETPSW refers to the longword which includes both the psw */
/* and part of the pc, so DON'T WRITE TO IT except by specifiying one */
/* of the particular bitfields.  -=] Ford [=- */
#define GETPSW(pcb) (*((struct psw *)&(pcb).psw))
#define GETPC(pcb) (*((unsigned long *)&(pcb).pcMSW))

/*
 * Symbolic locations of registers in pcb relative to regsave[0]
 * Usage: u.u_ar0->regsave[K_XX]
 */

#define K_A7    0
#define K_D0    1
#define K_D1    2
#define K_D2    3
#define K_D3    4
#define K_D4    5
#define K_D5    6
#define K_D6    7
#define K_D7    8
#define K_A0    9
#define K_A1    10
#define K_A2    11
#define K_A3    12
#define K_A4    13
#define K_A5    14
#define K_A6    15
#define	K_PS	16	/* pseudo reg. for sig.c */
#define K_FP    K_A6
#define K_SP    K_A7

/* Special Status Word bits */
#define	SSW_FC	0x8000	/* fault on stage C of the instruction pipe */
#define	SSW_FB	0x4000	/* fault on stage B of the instruction pipe */
#define	SSW_RC	0x2000	/* Rerun flag for stage C of the instruction pipe */
#define	SSW_RB	0x1000	/* Rerun flag for stage B of the instruction pipe */
#define	SSW_DF	0x0100	/* Fault/rerun flag for data cycle */
#define	SSW_RM	0x0080	/* Read-modify-write on data cycle */
#define	SSW_RW	0x0040	/* read/write for data cycle (1=read,0=write) */
#define	SSW_SIZ 0x0030	/* Size code for data cycle */
#define	SSW_FU	0x0007	/* Function codes for data cycle */

#endif /* _SYS_PCB_H */
