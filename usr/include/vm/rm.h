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

#ifndef _VM_RM_H
#define _VM_RM_H

#ident	"@(#)kernel:vm/rm.h	1.8"

/*
 * VM - Resource Management.
 */

#ifdef DEBUG

struct	page *rm_allocpage(/* seg, addr, len, flags */);

#else

#define rm_allocpage(seg, addr, len, flags) \
	(struct page *) (page_get((u_int) (len), (u_int) (flags)))

#endif

void	rm_outofanon();
void	rm_outofhat();
size_t	rm_asrss(/* as */);
size_t	rm_assize(/* as */);

/*
 * rm_allocpage() request flags.
 */
#ifndef P_NOSLEEP
#define	P_NOSLEEP	0x0000
#define	P_CANWAIT	0x0001
#define	P_PHYSCONTIG	0x0002
#endif

#endif	/* _VM_RM_H */
