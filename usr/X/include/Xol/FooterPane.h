/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)panel:include/Xol/FooterPane.h	1.4"
#endif
/*
 FooterPane.h (C hdr file)
	Acc: 602455391 Thu Feb  2 15:43:11 1989
	Mod: 601690014 Tue Jan 24 19:06:54 1989
	Sta: 601690014 Tue Jan 24 19:06:54 1989
	Owner: 4777
	Group: 1985
	Permissions: 666
*/
/*
	START USER STAMP AREA
*/
/*
	END USER STAMP AREA
*/

#ifndef _FooterPane_h
#define _FooterPane_h

#include <Xol/Manager.h>	/* include superclasses' header */

extern WidgetClass footerPanelWidgetClass;

typedef struct _FooterPanelClassRec *FooterPanelWidgetClass;
typedef struct _FooterPanelRec      *FooterPanelWidget;

#endif /* _FooterPane_h */
