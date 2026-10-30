/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)abbrevstack:include/Xol/AbbrevStac.h	1.8"
#endif

#ifndef _Ol_AbbrevStac_h
#define _Ol_AbbrevStac_h

/*
 *************************************************************************
 *
 * Description:
 *		This is the "public" include file for the Abbreviated
 *	Button Stack Widget.
 *
 ******************************file*header********************************
 */

#include <Xol/AbbrevMenu.h>		/* include superclasses' header */

extern WidgetClass				abbrevStackWidgetClass;
typedef struct _AbbrevMenuButtonClassRec *	AbbrevStackWidgetClass;
typedef struct _AbbrevMenuButtonRec *		AbbrevStackWidget;

#endif /* _Ol_AbbrevStac_h */
