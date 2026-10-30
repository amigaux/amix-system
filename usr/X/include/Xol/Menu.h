/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)menu:include/Xol/Menu.h	1.9"
#endif

#ifndef _Ol_Menu_h
#define _Ol_Menu_h

/*************************************************************************
 *
 * Description:
 *		This is the "public" include file for the Menu Widget.
 *	This menu widget belongs to the OPEN LOOK (Tm - AT&T) Toolkit.
 *
 *****************************file*header********************************/

#include <X11/Shell.h>

extern WidgetClass			menuShellWidgetClass;
typedef struct _MenuShellClassRec *	MenuShellWidgetClass;
typedef struct _MenuShellRec *		MenuShellWidget;

#endif /* _Ol_Menu_h */
