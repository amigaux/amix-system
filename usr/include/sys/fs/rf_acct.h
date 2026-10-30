/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_FS_RF_ACCT_H
#define _SYS_FS_RF_ACCT_H

#ident	"@(#)head.sys:sys/fs/rf_acct.h	1.9"

#include <sys/types.h>
#include <sys/sysinfo.h>

/*
 * Accounting structure for RFS client caching.
 */
typedef struct rfc_info {
	ulong rfci_pmread;	/* read cache miss pages */
	ulong rfci_pmwrite;	/* write cache miss pages */
	ulong rfci_ptread;	/* read pages sought in cache */
	ulong rfci_ptwrite;	/* write pages sought in cache */
	ulong rfci_pabort;	/* pages aborted from cache */
	ulong rfci_snd_dis;	/* cache disable messages sent */
	ulong rfci_rcv_dis;	/* cache disable messages received */
	ulong rfci_snd_msg;	/* total messages sent */
	ulong rfci_rcv_msg;	/* total messages received */
	ulong rfci_vc_hit;	/* vattr cache hits */
	ulong rfci_vc_miss;	/* vattr cache misses */
	ulong rfci_acr_hit;	/* access cache read permission hits */
	ulong rfci_acr_miss;	/* access cache read permission misses */
	ulong rfci_acw_hit;	/* access cache write permission hits */
	ulong rfci_acw_miss;	/* access cache write permission misses */
	ulong rfci_acx_hit;	/* access cache exec permission hits */
	ulong rfci_acx_miss;	/* access cache exec permission misses */
	ulong rfci_dis_data;	/* on client, messages incurred for files
				 * in resources mounted without caching */
} rfc_info_t;

typedef struct rf_srv_info {
			/* ELEMENT FOR sar -Du */
	time_t	rfsi_serve;	/* ticks in rfs server since boot */
			/* ELEMENTS FOR sar -S */
	ulong	rfsi_nservers;	/* sum of all servers since boot */
	ulong	rfsi_srv_que;	/* sum of server queue length since boot */
	ulong	rfsi_srv_occ;	/* seconds server queue found occupied */
	ulong	rfsi_rcv_que;	/* sum of server work list length since boot */
	ulong	rfsi_rcv_occ;	/* seconds server work list found occupied */
} rf_srv_info_t;

#if defined(_KERNEL)

extern rfc_info_t	rfc_info;
extern fsinfo_t		rfcl_fsinfo;
extern fsinfo_t		rfsr_fsinfo;
extern rf_srv_info_t	rf_srv_info;

extern time_t		*rfsi_servep;	/* SVID compliance hack for sar(1) */

extern	int minserve;			/* tunable: server low water mark */
extern	int maxserve;			/* tunable: server high water mark */
extern	int rf_nservers;		/* total servers in system */
extern	int n_idleserver;		/* idle servers in system */
extern	int rf_n_sr_msgs;		/* rcvds in server work list */

extern void rf_clock();

#endif

#endif /* _SYS_FS_RF_ACCT_H */
