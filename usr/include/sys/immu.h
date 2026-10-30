/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_IMMU_H
#define _SYS_IMMU_H

#ident	"@(#)head.sys:sys/immu.h	11.18"

/*
 * Page Table Entry Definitions
 */

typedef union pte {    /*  page table entry  */

/*                                                */
/*    	                   DEBUG                  */
/*    	                   only                   */
/*  +---------------------+---+---+-+-+-+-+-+     */
/*  |        pfn          |tag|   |m|r|w| |p|     */
/*  +---------------------+---+---+-+-+-+-+-+     */
/*             21           3   3  1 1 1 1 1      */
/*                                                */
	struct {
		uint pg_pfn	: 21,	/* Physical page frame number */
		     pg_tag	:  3,	/* Unused software bits. */
				:  3,	/* Reserved by hardware.	*/
		     pg_mod	:  1,	/* Page has been modified */
		     pg_ref	:  1,	/* Page has been referenced */
		     pg_w	:  1,	/* Fault on write */
				:  1,   /* Must be zero.                */
		     pg_pres	:  1;	/* Page is present in memory */
	} pgm;

	uint	pg_pte;		/* Full page descriptor (table) entry */
} pte_t;
#define pg_v	pg_pres		/* present bit = valid bit */

/*
 *	Page Table
 */

#define NPGPT		64	/* Nbr of pages per page table (seg). */

typedef struct ptbl {
	int page[NPGPT];
} ptbl_t;

/* Page table entry dependent constants */

#define	NBPP		2048		/* Number of bytes per page */
#define	NBPPT		256		/* Number of bytes per page table */
#define	BPTSHFT		8 		/* LOG2(NBPPT) if exact */
#define	NPTPP		8		/* Nbr of page tables per page.	*/
#define	NPTPPSHFT	3		/* Shift for NPTPP. */

#define PNUMSHFT	11		/* Shift for page number from addr. */
#define PNUMMASK	0x3F		/* Mask for page number in segment. */
#define POFFMASK        0x7FF		/* Mask for offset into page. */
#define	PNDXMASK	0x7FFFF		/* Mask for page index into section.*/
#define PGFNMASK	0x1FFFFF	/* Mask page frame nbr after shift. */

/* Page descriptor (table) entry field masks */

#define PG_ADDR		0xFFFFF800	/* physical page address */
#ifdef DEBUG
#define PG_LOCK		0x00000700	/* mapping SOFTLOCK count (software) */
#endif
#define PG_M		0x00000010	/* modify bit */
#define PG_REF		0x00000008	/* reference bit */
#define PG_W		0x00000004	/* fault on write bit */
#define PG_V		0x00000001	/* page present bit */

/* The page number within a section. */

#define pgndx(x)	(((x) >> PNUMSHFT) & PNDXMASK)

/* Round up page table address */

#define ptround(p)	((int *) (((int)p + PTSIZE-1) & ~(PTSIZE-1)))

/* Round down page table address */

#define pttrunc(p)	((int *) ((int)p & ~(PTSIZE-1)))

/* Page tables (64 entries == 256 bytes) to pages. */

#define	pttopgs(x)	((x + NPTPP - 1) >> NPTPPSHFT)
#define	pttob(x)	((x) << BPTSHFT)
#define	btopt(x)	(((x) + NBPPT - 1) >> BPTSHFT)

/* Form page table entry from modes and page frame number */

#define	mkpte(mode,pfn)	(mode | ((pfn) << PNUMSHFT))

/*	The following macros are used to set/check the values
 *	of the bits in a page descrptor (table) entry 
 *
 *	Atomic instruction is available to clear the present bit,
 *	other bits are set or cleared in a word operation.
 */

#define PG_ISVALID(pte) 	((pte)->pgm.pg_v)
#define PG_SETVALID(pte)	((pte)->pg_pte |= PG_V)
#define PG_CLRVALID(pte)	((pte)->pg_pte &= ~PG_V)

#define PG_SETNDREF(pte)	((pte)->pg_pte |= PG_NDREF)
#define PG_CLRNDREF(pte)	((pte)->pg_pte &= ~PG_NDREF)

#ifdef DEBUG

/*
 * Macros to keep track of SOFTLOCKs on a mapping.
 */

#define PG_ISLOCKED(pte)	((pte)->pgm.pg_tag)
#define PG_SETLOCK(pte)  	{ \
	if ((pte)->pgm.pg_tag == 7) \
		cmn_err(CE_PANIC, "Too many SOFTLOCKs on mapping !\n"); \
	++(pte)->pgm.pg_tag; \
}
#define PG_CLRLOCK(pte)  	{ \
	if ((pte)->pgm.pg_tag == 0) \
		cmn_err(CE_PANIC, "SOFTUNLOCK on an unlocked mapping !\n"); \
	--(pte)->pgm.pg_tag; \
}

#else

#define PG_ISLOCKED(pte)
#define PG_SETLOCK(pte)
#define PG_CLRLOCK(pte)

#endif

#define PG_SETMOD(pte)   	((pte)->pg_pte |= PG_M)	
#define PG_CLRMOD(pte)   	((pte)->pg_pte &= ~PG_M)	

#define PG_SETW(pte)     	((pte)->pg_pte |= PG_W)
#define PG_CLRW(pte)     	((pte)->pg_pte &= ~PG_W)

#define PG_SETREF(pte)    	((pte)->pg_pte |= PG_REF)
#define PG_CLRREF(pte)    	((pte)->pg_pte &= ~PG_REF)

/*
 * Segment Descriptor (Table) Entry Definitions
 */

typedef struct sde {    /*  segment descriptor (table) entry  */
/*                                                                            */
/*  +--------------+------+--------+----+ +--------------------------------+  */
/*  |    len       | ring |  prot  |type| |             address            |  */
/*  +--------------+------+--------+----+ +--------------------------------+  */
/*        16         6        8       2                    32                 */
/*                                                                            */
/*					  +--------------------------+-+-+-+  */
/*			   (V0):	  |                          |N|W|S|  */
/*					  +--------------------------+-+-+-+  */
/*						       29             1 1 1   */
/*                                                                            */
	uint seg_len    : 16,   /* Segment length in pages and L/U bit */
	     seg_ring   : 6,    /* Segment RAL/WAL (Not used) */
	     seg_prot   : 8,    /* Segment protection: SG,S,G,CI,L,M,U,WP */
	     seg_type   : 2;    /* Segment descriptor type */
	union {
		paddr_t address; /* Address of PT or physical segment (cont.) */
		struct {
			uint		: 29,	/* Not used */
			     wanted	:  1,	/* "N" bit  */
			     shmswap	:  1,	/* "W" bit  */
			     sanity	:  1;	/* "S" bit  */
		} V0;
		struct sde *sdeptr; /* pointer to next level sde_t tables */
	} wd2;
} sde_t;

/* segment protection bits */

#define SG_bit  0x80    /* Shared global */
#define S_bit   0x40    /* Superivsor only access */
#define G_bit   0x20    /* Gates allowed */
#define CI_bit  0x10    /* Inhibit page table cacheing */
#define L_bit   0x08    /* Lock in page cache */
#define M_bit   0x04    /* Modified bit */
#define U_bit   0x02    /* Used/Referenced bit */
#define WP_bit  0x01    /* segment is write protected */

#define SDE_invalid     0
#define SDE_contiguous  1       /* really early termination */
#define SDE_paged       2       /* valid, 4 byte descriptor at next level */
#define SDE_toplevel    3       /* valid, 8 byte descriptor at next level */

#define	SEG_RW_UBLK	(S_bit)		/* ublock can't be shared global */
#define SEG_RW_SYS      (SG_bit|S_bit)   /* supervisor r/w only */
#define SEG_RO_SYS      (SG_bit|S_bit|WP_bit)   /* supervisor read only */
#define SEG_RO          WP_bit          /* readonly (Super and User) */
#define SEG_RW          0x00            /* r/w (anyone) */

#define SDE_SOFT        7

#define SD_ISVALID(sde) 	((sde)->seg_type != SDE_invalid)
/*#define SD_INDIRECT(sde)	((sde)->seg_flags & SDE_I_bit)*/
#define SD_ISCONTIG(sde)	((sde)->seg_type == SDE_contiguous)
/*#define SD_ISTRAP(sde)  	((sde)->seg_flags & SDE_T_bit)*/
/*#define SD_SEGBYTES(sde)	(int)((((sde)->seg_len) + 1) << 3)*/
/*#define SD_LASTPG(sde)  	(((sde)->seg_len) >> 8)*/
#define SD_U_ACCESS(sde)	((((sde)->seg_prot) & S_bit) == 0)
#define SD_K_ACCESS(sde)	(((sde)->seg_prot) & S_bit)
#define SD_SEGINDX(addr)	((int)addr & MSK_IDXSEG)
#define SD_CLRVALID(sde)	((sde)->seg_type = SDE_invalid)

/*	Segment descriptor (table) dependent constants.	*/

#define NBPS		0x20000 /* Number of bytes per segment.  */
#define NPPS		64	/* Number of pages per segment.  */
#define PPSSHFT		6	/* Shift for pages per segment.  */
#define SOFFMASK	0x1FFFF	/* Mask for offset into segment. */
#define SEGOFF(x)	((uint)(x) & SOFFMASK)
#define	BPSDESHFT	3	/* Shift for bytes per sde.      */
#define	NBPSDT		64	/* Segment table size in bytes.  */
#define	BPSDTSHFT	6	/* Shift for bytes per seg tbl.  */
#define	NSDTPP		32	/* Number of seg tbls per page.  */
#define	SDTPPSHFT	5	/* Shift for seg tbls per page.  */

#define	sdetob(x)	((x) << BPSDESHFT)
#define	btosde(x)	((x) >> BPSDESHFT)
#define	sdttopgs(x)	(((x) + NSDTPP - 1) >> SDTPPSHFT)
#define	sdttob(x)	((x) << BPSDTSHFT)
#define	btosdt(x)	(((x) + NBPSDT - 1) >> BPSDTSHFT)

/*
 * Memory Management Unit Definitions
 */

/*    	                                                     */
/*  Virtual Address:                                         */
/*    	                                                     */
/*  +-----------+---------------+----------+--------------+  */
/*  |  section  |    segment    |   page   |    offset    |  */
/*  +-----------+---------------+----------+--------------+  */
/*        2            13             6           11         */
/*    	                                                     */

typedef struct _VAR {
	uint v_sid	:  2,	/*  section number      */
	     v_ssl	: 13,	/*  segment number      */
	     v_psl	:  6,	/*  page number         */
	     v_byte	: 11;	/*  offset within page  */
} VAR;

/*  masks to extract portions of a VAR  */
#define SECNMASK	0x3	/* Mask for section number after shift. */
#define SECNSHFT	30	/* Shift for section number from address. */
#define SEGNMASK	0x1FFF	/* Mask for segment number after shift. */
#define SEGNSHFT	17	/* Shift for segment number from address. */

#define SECNUM(x)   (((uint)(x) >> SECNSHFT) & SECNMASK)
#define SEGNUM(x)   (((uint)(x) >> SEGNSHFT) & SEGNMASK)
#define PAGNUM(x)   (((uint)(x) >> PNUMSHFT) & PNUMMASK)
#define PAGOFF(x)   (((uint)(x)) & POFFMASK)

#define MSK_IDXSEG  0x1ffff  /*  lower 17 bits == PSL || BYTE  */

/*  access execution level */

/*#define	XLVL_K		0*/
/*#define	XLVL_U		3*/

/*  PMMU Status register bits (returned by PTEST) */

#define F_BUS_ERROR     0x8000
#define F_LIMIT         0x4000
#define F_SUPER_ONLY    0x2000
#define F_ACCESS_LEVEL  0x1000
#define F_PWRITE        0x0800
#define F_INVALID       0x0400
#define F_MODIFIED      0x0200
#define F_GATE          0x0100
#define F_SHARED        0x0080
#define F_NUM_LEVELS    0x000F  /* mask */

/* Virtual start address of sections */

#define VSECT0		0x00000000
#define VSECT1		0x40000000
#define VSECT2		0x80000000
#define VSECT3		0xC0000000

/* #define SRAMBSHIFT	10 */
#define MAXSDTSEG	8192	/* Maxmum number of segment tbl entries */

#define OFF 0
#define ON  1

#define flushaddr(vaddr)	flushmmu((vaddr), 1)

/*	The following variables describe the memory managed by
**	the kernel.  This includes all memory above the kernel
**	itself.
*/
extern int	kpbase;		/* The address of the start of	*/
				/* the first physical page of	*/
				/* memory above the kernel.	*/
				/* Physical memory from here to	*/
				/* the end of physical memory	*/
				/* is represented in the pfdat.	*/
extern int	syssegs[];	/* Start of the system segment	*/
				/* from which kernel space is	*/
				/* allocated.  The actual value	*/
				/* is defined in the vuifile.	*/
extern int	win_ublk[];	/* A window into which a	*/
				/* u-block can be mapped.	*/
extern pte_t	*kptbl;		/* Kernel page table.  Used to	*/
				/* map sysseg.			*/
extern int	maxmem;		/* Maximum available free	*/
				/* memory.			*/
extern int	freemem;	/* Current free memory.		*/
extern int	availrmem;	/* Available resident (not	*/
				/* swapable) memory in pages.	*/
extern int	availsmem;	/* Available swapable memory in	*/
				/* pages.			*/

/*	Conversion macros
*/

/*	Get page number from system virtual address.  */

#define	svtop(x)	((uint)(x) >> PNUMSHFT) 

/*	Get system virtual address from page number.  */

#define	ptosv(x)	((uint)(x) << PNUMSHFT)


/*	These macros are used to map between kernel virtual
**	and physical address.  Note that all of physical
**	memory is mapped into kernel virtual address space
**	in segment zero at the actual physical address of
**	the start of memory which is MAINSTORE.
*/

extern paddr_t	svirtophys(/* va */);
extern paddr_t  uvirtophys(/* va , as */);

#define phystokv(paddr) (paddr)

/*	Between kernel virtual address and physical page frame number.
*/

#define phystopfn(paddr)	((u_int)(paddr) >> PNUMSHFT)
#define pfntophys(pfn)  	((pfn) << PNUMSHFT)
#define kvtopfn(vaddr)  	(kvtophys(vaddr) >> PNUMSHFT)
#define pfntokv(pfn)    	((pfn) << PNUMSHFT)

/*	Between kernel virtual addresses and the kernel
**	segment table entry.
*/

#define kvtokstbl(vaddr) (&(st_top1[SEGNUM(vaddr)]))

extern	sde_t	*st_top1;	/* pointer to SCN1 segment descriptor
				 * table, see ml/pstart.c
				 */

/*	Between kernel virtual addresses and the kernel page
**	table.
*/

#define	kvtokptbl(x)	(&kptbl[pgndx((uint)(x) - (uint)syssegs)])

/*	Test whether a virtual address is in the kernel dynamically
**	allocated area.
*/

#define	iskvir(va)	((SECNUM(va) == SCN1)  &&  \
			 (uint)va >= (uint)syssegs)

/*
 * vatosde(v)
 * returns the segment descriptor entry location of the virtual address v.
 */

extern sde_t *vatosde();

/*
 * uvatosde(v)
 * returns the segment descriptor entry location
 * of the USER virtual address v.
 */
extern sde_t *uvatosde();

/*
 * pte_t *
 * svtopte(v)
 * returns the pte entry location of v.
 *
 * This macro works only with paged virtual address.
 */

#define svtopte(v) ((pte_t *)(phystokv(vatosde(v)->wd2.address)) + PAGNUM(SEGOFF(v)))

/*
 * pte_t *
 * uvtopte(v)
 * returns the pte entry location of USER virtual address v.
 *
 * This macro works only with paged virtual address.
 */

#define uvtopte(v) ((pte_t *)(phystokv(uvatosde(v)->wd2.address)) + PAGNUM(SEGOFF(v)))

/*
 * vatopte(va)
 * returns the page descriptor entry location of the virtual address va.
 */

#define	vatopte(va, sde)	((pte_t *)((sde)->wd2.address) + PAGNUM(va))

/*
 * svtopfn(v)
 */

#define svtopfn(v) (PAGNUM(svirtophys(v)))

/*	Page frame number to kernel pte.
*/

#define	pfntokptbl(p)	(kvtokptbl(pfntokv(p)))

/* flags used in ptmemall() call
*/

#define PHYSCONTIG 02
#define NOSLEEP    01

/* Section Id used in flushsect() and loadmmu() */

#define SCN0	0
#define SCN1	1
#define SCN2	2
#define SCN3	3

#define	SDE_ALIGN	16	/* segments must align to this boundary */
#define	PTE_ALIGN	16	/* page tables must align to this boundary */

extern long tc_on;
extern long tt0_on;
extern long tt1_on;

#ifdef __GNUC__
#define pmmu_on() asm volatile("pmove tc_on,%tc")
#else
asm void 
pmmu_on()
{
#if MC68030_ONLY
	pmove	tt0_on,%tt0
	pmove	tt1_on,%tt1
#endif
	pmove	tc_on,%tc
}
#endif /* GNUC */

/* loadmmu(urp)	
 * sde_t *urp;	 pointer to user map root pointer
 *
 * Load the user mode root pointer
 * using the passed pointer and flush the TLB.
 */
#ifdef __GNUC__
static inline void loadmmu(paddr_t urp)
{asm volatile(" mov.l	%0,userroot+4
	mov.l	&userroot,%%a1
	pmove	(%%a1),%%crp
	pflusha" : : "a" (urp) : "%a1");
}
#else
asm	void
loadmmu(urp)
{
%	mem	urp;
	mov.l	urp,userroot+4
	mov.l	&userroot,%a1
	pmove	(%a1),%crp
	pflusha
%error	# force a compiler diagnostic if the parameters dont match
}
#endif /* GNUC */

/* loadkrp(krp)	
 * sde_t *krp;	 pointer to kernel map root pointer 
 *
 * Load the kernel mode root pointer
 * using the passed pointers and flush the TLB.
*/
#ifdef __GNUC__
static inline void loadkrp(void *krp)
{asm volatile("pmove	(%0),%%srp
	pflusha" : : "a" (krp));
}
#else
asm	void
loadkrp(krp)
{
%	mem	krp;
	mov.l	krp,%a1
	pmove	(%a1),%srp
	pflusha
%error	# force a compiler diagnostic if the parameters dont match
}
#endif /* GNUC */

#ifdef __GNUC__
#define pflusha() asm("pflusha")
#else
asm 	void
pflusha()
{
	pflusha
}
#endif /* GNUC */

#ifdef _KERNEL

#if defined(__STDC__)
extern sde_t *vatosde(caddr_t);
extern void flushmmu(caddr_t, int);
#else
extern sde_t *vatosde();
extern void flushmmu();
#endif	/* __STDC__ */

#endif	/* _KERNEL */

#endif	/* _SYS_IMMU_H */
