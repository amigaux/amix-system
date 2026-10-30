/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)changebar:ChangeBar.h	1.5"
#endif

#ifndef _OL_CHANGEBAR_H
#define _OL_CHANGEBAR_H

#include "X11/Intrinsic.h"
#include "Xol/OpenLook.h"

#define _OlChangeBarWidth(CB)	((CB)->width)
#define _OlChangeBarHeight(CB)	((CB)->height)
#define _OlChangeBarSpan(CB)	((CB)->width + (CB)->pad)

/*
 * Size/spacing of a change bar, in points
 */
#define CHANGE_BAR_WIDTH	3
#define CHANGE_BAR_HEIGHT	18
#define CHANGE_BAR_PAD		7

/*
 * Pick numbers not taken by the other (official) OlDefines
 */
#define OL_DIM				1000
#define OL_NORMAL			1001

#define OL_PROPAGATE_TO_CONTROL_AREA	0x0001
#define OL_PROPAGATE_TO_CATEGORY	0x0002
#define OL_PROPAGATE \
		(OL_PROPAGATE_TO_CONTROL_AREA|OL_PROPAGATE_TO_CATEGORY)

extern char		XtNallowChangeBars [];
extern char		XtNchangeBar	   [];

extern char		XtCAllowChangeBars [];
extern char		XtCChangeBar	   [];

typedef struct ChangeBar {
	GC			normal_GC; /* GC for regular change bar	*/
	GC			dim_GC;	   /* GC for dim change bar	*/
	Dimension		width;	   /* width of change bar	*/
	Dimension		height;	   /* height of change bar	*/
	Dimension		pad;	   /* padding next to change bar*/
}			ChangeBar;

/*
 * External functions:
 */

extern ChangeBar *	_OlCreateChangeBar OL_ARGS((
	Widget	w
));
extern void		_OlDestroyChangeBar OL_ARGS((
	Widget			w,
	ChangeBar *		cb
));
extern void		_OlSetChangeBarState OL_ARGS((
	Widget			w,
	OlDefine		state,
	unsigned int		propagate
));
extern void		_OlFlatSetChangeBarState OL_ARGS((
	Widget			w,
	Cardinal		indx,
	OlDefine		state,
	unsigned int		propagate
));
extern void		_OlDrawChangeBar OL_ARGS((
	Widget			w,
	ChangeBar *		cb,
	OlDefine		state,
	Boolean			expose,
	Position		x,
	Position		y,
	Region			region
));
extern void		_OlGetChangeBarGCs OL_ARGS((
	Widget			w,
	ChangeBar *		cb
));
extern void		_OlFreeChangeBarGCs OL_ARGS((
	Widget			w,
	ChangeBar *		cb
));
extern Pixel		_OlContrastingColor OL_ARGS((
	Widget			w,
	Pixel			pixel,
	int			percent
));

#endif
