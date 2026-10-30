/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)olclib:FMcomm.h	1.9"

#ifndef _FMcomm_h
#define _FMcomm_h

#define QEMPTY		0
#define GOTREQUEST	1

/*
 * public request types
 */

#define FM_ACTIVATE	1
#define FM_BROWSE	2

/*
 * private request types
 */

#define FM_COPY		3
#define FM_MOVE		4
#define FM_DELETE	5
#define FM_MKDIR 	6
#define FM_RMTMPDIR 	7
#define FM_RMDIR 	8
#define FM_OVERWRITE 	9
#define FM_ENDOVER	10
#define FM_REFRESH	11
#define FM_CANTACCESS	12
#define FM_LINK		13

/*
 * reply types
 */

#define FM_CANCEL	9
#define FM_ACCEPT      10
#define FM_INVALID     11

typedef struct
   {
   int serial;
   char * name;
   int    wingroup;
   char * directory;
   char * pattern;
   char * label;
   char * sysname;
   char * nodename;
   char * reserved;
   unsigned short uid;
   unsigned short gid;
   } FM_Request;

typedef struct
   {
   int serial;
   char * path;
   char * sysname;
   char * nodename;
   char * reserved;
   } FM_Reply;

extern Atom	_OL_FM_QUEUE;
extern Atom	_OL_FM_REPLY;

extern void ClearFMQueue();
extern int  EnqueueFMRequest();
extern int  DequeueFMRequest();
extern int  SendFMReply();
extern int  AcceptFMReply();

#endif
