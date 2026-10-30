/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)bboard:include/Xol/BulletinBo.h	1.5"
#endif

#ifndef _BulletinBo_h
#define _BulletinBo_h

#include <Xol/Manager.h>	/* include superclasses' header */

/*
Name		Type		Default		Meaning
---		----		-------		-------
XtNlayout	BulletinLayout	OL_MINIMIZE	Type of layout
*/

typedef struct _BulletinBoardClassRec *BulletinBoardWidgetClass;
typedef struct _BulletinBoardRec      *BulletinBoardWidget;

extern WidgetClass bulletinBoardWidgetClass;

#endif /* _Bulletin_h */
