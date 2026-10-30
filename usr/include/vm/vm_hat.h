/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 		PROPRIETARY NOTICE (Combined)
 * 
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 * 
 * 
 * 
 * 		Copyright Notice 
 * 
 * Notice of copyright on this source code product does not indicate 
 * publication.
 * 
 * 	(c) 1986,1987,1988,1989  Sun Microsystems, Inc
 * 	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
 * 	          All rights reserved.
 *  
 */

#ifndef _VM_VM_HAT_H
#define _VM_VM_HAT_H

#ident	"@(#)kernel:vm/vm_hat.h	1.17"

#include "sys/immu.h"
#include "vm/page.h"

/*
 * VM - Hardware Address Translation management.
 *
 * This file describes the contents of the machine specific
 * hat data structures and the machine specific hat procedures.
 * The machine independent interface is described in <vm/hat.h>.
 */

/*
 * The hat structure is the machine dependent hardware address translation
 * structure kept in the address space structure to show the translation.
 */
typedef struct hat {
	sde_t	*hat_sram;	/* pointer to top level of tree */
	sde_t	hat_top[7];	/* Top level descriptors for 68020 */
} hat_t;

/*
 * The page table data (ptdat) structure is a machine dependent hardware
 * address translation structure that keeps information about a page table.
 * This struct is not kept for the kernel page tables.
 *
 * Active page tables:
 *
 * pt_as      - address space the page table belongs to.
 * pt_secseg  - section/segment the page table is mapping.
 * pt_inuse   - number of active pte's in the page table.
 * pt_keepcnt - number of pte's SOFTLOCK'd in page table.
 *
 * Free page tables:
 *
 * pt_addr    - physical addr of the page table.
 * pt_pp      - pp of the page that contains page table.
 *
 */
typedef struct ptdat {
	union {
		struct {
			struct as *ptas;
			ushort ptsecseg;
			u_char ptinuse;
			u_char ptkeepcnt;
		} pt_active;
		struct {
			pte_t	*ptaddr;
			page_t	*ptpp;
		} pt_free;
	} pt_use;
	struct ptdat *pt_prev;	/* prev ptdat in the list. */
	struct ptdat *pt_next;	/* next ptdat in the list. */
} ptdat_t;

#define	pt_as    	pt_use.pt_active.ptas
#define	pt_secseg	pt_use.pt_active.ptsecseg
#define	pt_inuse 	pt_use.pt_active.ptinuse
#define	pt_keepcnt	pt_use.pt_active.ptkeepcnt
#define	pt_addr  	pt_use.pt_free.ptaddr
#define	pt_pp    	pt_use.pt_free.ptpp

/*
 * Flags to indicate the section of the virtual address space with respect
 * to the hat structure contained in the as structure for a process.
 */
#define HAT_SCN0	0
#define HAT_SCN1	1
#define HAT_SCN2	2
#define HAT_SCN3	3

/*
 * Flags to pass to hat_ptalloc().
 *
 * NOTE: HAT_NOSLEEP and HAT_CANWAIT must match up with P_NOSLEEP
 *       and P_CANWAIT (respectively) in vm/page.h .
 */
#define	HAT_NOSLEEP	0	/* return immediately if no memory.          */
#define	HAT_CANWAIT	1	/* wait if no memory currently available.    */
#define HAT_NOSTEAL	2	/* don't steal a page tbl from another proc. */

/*
 * Page table flag bits
 */
#define	PT_RESERVE	0x100

#define	p_ptbits	p_offset
#define	p_ptdats	p_mapping
#define	p_sdtbits	p_mapping

#ifdef _KERNEL

/*
 * Round up the p_ubptbl address, that was given, to the next 16 byte
 * boundary and convert it to a physical address.
 *
 * paddr_t
 * phys_ubptbl(pp->p_ubptbl)
 * caddr_t virtaddr;
 * {
 *	virtaddr = (virtaddr + 0x0F) & ~0x0F;
 *	return( kvtophys(virtaddr) );
 * }
 */
#if defined(lint)

#define phys_ubptbl(va)	((paddr_t)kvtophys((caddr_t)\
	((((u_long)(va)) + 0x0F) & ~0x0F)))

#else

/*
	WARNING:  This routine is not correct.  It needs to convert the
		rounded address to a physical address.
*/
#ifdef __GNUC__
#define phys_ubptbl(va) ((paddr_t)(((unsigned long)(va)+0xf)&0xfffffff0))
#else
asm paddr_t
phys_ubptbl(va)
{
%	mem	va;
	mov.l	va,%d0
	add.l	&0x0f,%d0
	and.l	&0xfffffff0,%d0
}
#endif /* GNUC */

#endif

extern sde_t dflt_sdt_p;
extern sde_t mmu_invalid;

#endif /* _KERNEL */

#endif	/* _VM_VM_HAT_H */
