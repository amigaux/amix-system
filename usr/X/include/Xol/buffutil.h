/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)olmisc:buffutil.h	1.3"
#endif

/*
 * buffutil.h
 *
 */

#ifndef _buffutil_h
#define _buffutil_h

#include <stdio.h>

	/* shouldn't check for SEEK_SET because it defined in unistd.h  */
	/* in SVR3.2 and X11/Xos.h will include this header file if USG */
#ifndef SVR4				/* AG */
#ifndef __STDC__
#ifndef __cplusplus
#ifndef c_plusplus
#define memmove(dest, src, n)  bcopy((char*)src, (char*)dest, (int)n)
#endif
#endif
#endif
#endif /* SVR4 via AG */
typedef char BufferElement;

#define Bufferof(type) \
   struct \
      { \
      int    size; \
      int    used; \
      int    esize; \
      type * p; \
      }

typedef struct _Buffer
   {
   int    size;
   int    used;
   int    esize;
   BufferElement * p;
   } Buffer;

#define LNMIN       8
#define LNINCRE    24

#define BufferFilled(b)  (b-> size == b-> used)
#define BufferLeft(b)    (b-> size - b-> used)
#define BufferEmpty(b)   (b-> used == 0)

extern Buffer * AllocateBuffer();
extern void     GrowBuffer();
extern Buffer * CopyBuffer();
extern void     FreeBuffer();
extern int      InsertIntoBuffer();
extern int      ReadFileIntoBuffer();
extern int      ReadStringIntoBuffer();

extern Buffer * stropen();
extern int      strgetc();
extern void     strclose();

#endif
