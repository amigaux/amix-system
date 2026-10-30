/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_UTSNAME_H
#define _SYS_UTSNAME_H

#ident	"@(#)head.sys:sys/utsname.h	11.14"

/*
 * If you are compiling the kernel, the value used in initializing
 * the utsname structure in the master.d/kernel file better be the
 * same as SYS_NMLN.
 */
#if (defined (_POSIX_SOURCE) || defined(_XOPEN_SOURCE)) && !defined(_KERNEL)

#define _SYS_NMLN	257	/* 4.0 size of utsname elements.*/
				/* Must be at least 257 to 	*/
				/* support Internet hostnames.  */


struct utsname {
	char	sysname[_SYS_NMLN];
	char	nodename[_SYS_NMLN];
	char	release[_SYS_NMLN];
	char	version[_SYS_NMLN];
	char	machine[_SYS_NMLN];
};

#else /*!defined(POSIX_SOURCE) && !defined(_XOPEN_SOURCE) || defined(_KERNEL)*/

#define SYS_NMLN	257	/* 4.0 size of utsname elements.*/
				/* Must be at least 257 to 	*/
				/* support Internet hostnames.  */

struct utsname {
	char	sysname[SYS_NMLN];
	char	nodename[SYS_NMLN];
	char	release[SYS_NMLN];
	char	version[SYS_NMLN];
	char	machine[SYS_NMLN];
};

#endif /* defined (_POSIX_SOURCE) || defined(_XOPEN_SOURCE) */

#if !defined(_POSIX_SOURCE) 
extern struct utsname utsname;
#endif /* !defined(_POSIX_SOURCE) */

#if !defined(_KERNEL)
#if defined(__STDC__)

static int uname(struct utsname *);

int nuname(struct utsname *);
#else

static int uname();

int nuname();
#endif	/* (__STDC__) */
#endif	/* !(KERNEL) */

#if !defined(_KERNEL)
static int
uname(buf)
struct utsname *buf;
{
	int ret;

	ret = nuname(buf);
	return ret;
}
#endif /* !defined(_KERNEL) */

#endif	/* _SYS_UTSNAME_H */
