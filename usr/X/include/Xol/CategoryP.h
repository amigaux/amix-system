/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)category:CategoryP.h	1.5"
#endif

#if	!defined(_CATEGORYP_H)
#define _CATEGORYP_H

#include "Xol/Olg.h"
#include "Xol/ManagerP.h"
#include "Xol/Category.h"
#include "Xol/ChangeBar.h"

/*
 * Space (in points) between
 *
 *	- top of CATEGORY label, AbbrevMenuButton widget, page label
 *	- left edge of CATEGORY label and left edge of Category widget
 *	- CATEGORY and AbbrevMenuButton widget
 *	- AbbrevMenuButton widget and page label
 *	- bottom of CATEGORY label, AbbrevMenuButton widget, page label
 *	- footer and edge of Category widget
 *	- left and right footers (1/2 that distance, actually)
 *	- left edge and change bar
 */
#define CATEGORY_TOP_MARGIN		10
#define CATEGORY_LEFT_MARGIN		10
#define CATEGORY_SPACE1			10
#define CATEGORY_SPACE2			10
#define CATEGORY_BOTTOM_MARGIN		10
#define CATEGORY_FOOTER_EDGE		10
#define CATEGORY_INTER_FOOTER	 	4
#define CATEGORY_FOOTER_HEIGHT		22
#define CATEGORY_CHANGE_BAR_SPACE	4

/*
 * Constraint record:
 */
typedef struct	_CategoryConstraintRec {
	/*
	 * Public:
	 */
	String			page_label;
	int			gravity;
	Boolean			_default;
	Boolean			changed;
	Boolean			available_when_unmanaged;
	Boolean			query_child;

	/*
	 * Private:
	 */
	Boolean			window_gravity_set;
}			CategoryConstraintRec;

/*
 * Class strucure:
 */

typedef struct _CategoryClassPart {
	int			no_class_fields;
}			CategoryClassPart;

typedef struct _CategoryClassRec {
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	ManagerClassPart	manager_class;
	CategoryClassPart	category_class;
}			CategoryClassRec;

extern CategoryClassRec categoryClassRec;

/*
 * Instance structure:
 */

typedef struct MenuItem {
	String			label;
	Boolean			set;
	XtPointer		user_data;
}			MenuItem;

typedef struct _CategoryPart {
	/*
	 * Public:
	 */
	struct {
		OlDefine		width;
		OlDefine		height;
	}			layout;
	struct {
		Dimension		width;
		Dimension		height;
	}			page;
	String			category_label;
	XFontStruct *		category_font;
	XFontStruct *		font;
	Pixel			font_color;
	Boolean			show_footer;
	String			left_foot;
	String			right_foot;
	XtCallbackList		new_page;
	Widget			lower_control_area;

	/*
	 * Private:
	 */
	Widget			page_choice;
	Widget			next_page;
	Widget			delayed_set_page_child;
	MenuItem *		page_list;
	Cardinal		first_page_child;
	Cardinal		page_list_size;
	Cardinal		current_page;
	GC			category_font_GC;
	GC			font_GC;
	OlgAttrs *		attrs;
	ChangeBar *		cb;
	XtOrderProc		insert_position;
	unsigned char		flags;
	unsigned char		dynamics;
}			CategoryPart;

#define _CATEGORY_INTERNAL_CHILD	0x001

#define _CATEGORY_B_DYNAMIC_BACKGROUND	0x001
#define _CATEGORY_B_DYNAMIC_FONTCOLOR	0x002

typedef struct _CategoryRec {
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	ManagerPart		manager;
	CategoryPart		category;
}			CategoryRec;

#endif
