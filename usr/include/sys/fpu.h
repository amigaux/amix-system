/* @(#)fpu.h    6.1      */

/* The following is information specific to the M68881 floating point chip.
** WARNING: If this file is changed and your kernel selects M68881 support,
**          you are advised to build the kernel again from scratch, as this
**          file is included by file "user.h", which is used everywhere in
**          the kernel!.
*/

#ifndef _SYS_FPU_H
#define _SYS_FPU_H
/* bit masks for various exceptions in M68881 %status, %control registers */
#define EXC_MASK        0xff00
#define EXC_INX1        0x0100
#define EXC_INX2        0x0200
#define EXC_DZ          0x0400
#define EXC_UNFL        0x0800
#define EXC_OVFL        0x1000
#define EXC_OPER        0x2000
#define EXC_SNAN        0x4000
#define EXC_BSUN        0x8000

/* bit masks for condition codes in M68881's %status register */
#define FPCC_NAN        0x01000000
#define FPCC_I          0x02000000
#define FPCC_Z          0x04000000
#define FPCC_N          0x08000000

/* bit sets for rounding mode and precision in 881's %control register */
#define RM_MASK         0x30
#define RM_NEAR         0x00
#define RM_ZERO         0x10
#define RM_NGINF        0x20
#define RM_PSINF        0x30
#define RP_MASK         0xc0
#define RP_X            0x00
#define RP_S            0x40
#define RP_D            0x80

        typedef long freg[3];   /* 4*3 bytes or 96 bits per FPi */

        typedef struct {        /* M68881 programmer's model */
               freg  fpu_reg[8];    /* holds fp0..fp7 */
               long  fpu_control,
                     fpu_status,
                     fpu_iaddr;     /* 32-bit 881 system registers */
               } fpu_t;

/* The following are masks used in looking at the "ustate" status word
** in the "sys_info" structure.  They indicate the condition of 881
** info. inside the u-vector.
*/
#define UFPRWRT 1               /* Flags the need to update the 881 programmer's
                                ** model from the u-vector values before going 
                                ** through normal context switch processing.
                                */
#define FSAVEMAX	54	/* Max. busy save frame for 68882 (68881=46) */
        typedef struct {
               long  ustate;    /* Flag word to remember condition of 881
                                ** info inside the u-vector.
                                ** (offset 0) */
               fpu_t regs;      /* holds fp0..fp7, control, status, iaddr 
                                ** (offset 4 ) */
               int fsave[FSAVEMAX];/* area where latest FSAVE info (881 internal
                                ** state info) is kept.  Put last because if
                                ** it is trashed, it is least likely to contain
                                ** useful info.
                                ** (offset 112) */
               } fpu_info;

/* state frame formats for 68881 and 68882 */
#define FIDLE881 0x180000	/* 68881 idle frame format */
#define FIDLE882 0x380000	/* 68882 idle frame format */
#define BUSY881 0xb40000	/* 68881 busy frame format */
#define BUSY882 0xd40000	/* 68882 busy frame format */
#define FRMTMASK 0xff0000	/* format byte mask */

/* indices into idle state frame */
#define FORMAT 0 	/* index of format word */
#define	EXOP881	2	/* index of exception op in 68881 */
#define	EXOP882	10	/* index of exception op in 68882 */
#define EXOPSIZE 12	/* number of bytes in exception operand */
#define	BIUFLG881 6	/* index of BIU flags in 68881 */
#define	BIUFLG882 14	/* index of BIU flags in 68882 */

extern	int	fpu_present;

#endif	/* _SYS_FPU_H */
