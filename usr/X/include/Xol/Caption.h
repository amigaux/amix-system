/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)caption:include/openlook/Caption.h	1.9"
#endif

#ifndef _Caption_h
#define _Caption_h

#include <Xol/Manager.h>	/* include superclasses' header */

/***********************************************************************
**
** Open Look Caption Widget
**
************************************************************************/

/*

Name		Type		Default		Meaning
---		----		-------		-------
XtNforeground	XtRPixel	Black		Foreground pixel
XtNfont		XtRFontStruct	Fixed		Font for caption
XtNlabel	XtRString	NULL		text of caption
XtNposition	int		OL_LEFT		Where caption goes
XtNalignment	int		OL_CENTER 	Caption position fine-tuning
XtNcaptionWidth	Dimension	none		Width of caption
XtNspace	Dimension	4		spacing

*/
 
/* Class record constants */

extern WidgetClass captionWidgetClass;

typedef struct _CaptionClassRec *CaptionWidgetClass;
typedef struct _CaptionRec      *CaptionWidget;

#endif /* _Caption_h */
