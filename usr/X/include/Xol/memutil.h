/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)olmisc:memutil.h	1.1"
#endif

/*
 * memutil.h
 *
 */

#ifndef _memutil_h
#define _memutil_h

#ifdef DEBUG
#   define PRINTF(x) (void) fprintf x
#   define REGISTER_MALLOC(p) (void) fprintf \
       (stderr, "%x malloced around %s at %d.\n", p, __FILE__, __LINE__);
#else
#   define PRINTF(x)
#   define REGISTER_MALLOC(p) 
#endif

#define FREE(p)         _Free(p, __FILE__, __LINE__)
#define CALLOC(n,s)     _Calloc(n, s, __FILE__, __LINE__)
#define MALLOC(p)       _Malloc(p, __FILE__, __LINE__)
#define REALLOC(p,s)    _Realloc(p, s, __FILE__, __LINE__)

extern void      _Free();
extern char *    _Calloc();
extern char *    _Malloc();
extern char *    _Realloc();

#endif
