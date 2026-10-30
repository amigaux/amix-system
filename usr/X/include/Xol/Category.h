/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)category:Category.h	1.4"
#endif

#if	!defined(_CATEGORY_H)
#define _CATEGORY_H

#include "Xol/Manager.h"

typedef struct _OlCategoryNewPage {
	Widget			new;
	Widget			old;
	Boolean			apply_all;
}			OlCategoryNewPage;

extern char		XtNavailableWhenUnmanaged [];
extern char		XtNcategoryLabel          [];
extern char		XtNcategoryFont           [];
extern char		XtNchanged                [];
extern char		XtNleftFoot               [];
extern char		XtNnewPage                [];
extern char		XtNpageLabel              [];
extern char		XtNpageHeight             [];
extern char		XtNpageWidth              [];
extern char		XtNqueryChild             [];
extern char		XtNrightFoot              [];
extern char		XtNshowFooter             [];

extern char		XtCAvailableWhenUnmanaged [];
extern char		XtCCategoryLabel          [];
extern char		XtCCategoryFont           [];
extern char		XtCChanged                [];
extern char		XtCLeftFoot               [];
extern char		XtCNewPage                [];
extern char		XtCPageLabel              [];
extern char		XtCPageWidth              [];
extern char		XtCPageHeight             [];
extern char		XtCQueryChild             [];
extern char		XtCRightFoot              [];
extern char		XtCShowFooter             [];

extern WidgetClass	categoryWidgetClass;

typedef struct _CategoryClassRec *	CategoryWidgetClass;
typedef struct _CategoryRec *		CategoryWidget;
typedef struct _CategoryConstraintRec *	CategoryConstraints;

/*
 * Used to change the property sheet. Returns True if the
 * Apply All button should be displayed.
 */
extern Boolean		OlCategorySetPage OL_ARGS((
	CategoryWidget		w,
	Widget			child
));

#endif /* _CATEGORY_H */
