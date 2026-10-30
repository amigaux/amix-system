/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)bboard:include/Xol/BulletinBP.h	1.7"
#endif

#ifndef _BulletinBP_h
#define _BulletinBP_h

#include	<Xol/ManagerP.h>	/* include superclasses' header */
#include	<Xol/BulletinBo.h>

typedef struct
{
	int keep_compiler_happy;   /* No new procedures */
} BulletinBoardClassPart;


typedef struct _BulletinBoardClassRec
{
   CoreClassPart		core_class;
   CompositeClassPart		composite_class;
   ConstraintClassPart		constraint_class;
   ManagerClassPart		manager_class;
   BulletinBoardClassPart	bulletin_class;
} BulletinBoardClassRec;

extern BulletinBoardClassRec bulletinBoardClassRec;

/* New fields for the Bulletin Board widget record */

typedef struct
{
   OlDefine	layout;
} BulletinBoardPart;


/*
** Full instance record declaration
*/

typedef struct _BulletinBoardRec
{
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    ManagerPart		manager;
    BulletinBoardPart	bulletin;
} BulletinBoardRec;

#endif
