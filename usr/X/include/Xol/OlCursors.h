/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)olmisc:OlCursors.h	1.2"
#endif

/*
 * OlCursors.h
 *
 */

#ifndef _OlCursors_h
#define _OlCursors_h

	/* OLBegin/OLEndFunctionPrototypeBlock are defined in OpenLook.h */
OLBeginFunctionPrototypeBlock

extern void   _GetBandWXColors OL_ARGS((
	Screen *	screen,
	XColor *	black,
	XColor *	white
));
extern Cursor _CreateCursorFromFiles OL_ARGS((
	Screen *	screen,
	char *		sourcefile,
	char *		maskfile
));
extern Cursor _CreateCursorFromData OL_ARGS((
	Screen *	screen,
	unsigned char *	sbits,
	unsigned char *	mbits,
	int		w,
	int		h,
	int 		xhot,
	int 		yhot
));
extern Cursor GetOlMoveCursor OL_ARGS((
	Screen *	screen
));
extern Cursor GetOlDuplicateCursor OL_ARGS((
	Screen *	screen
));
extern Cursor GetOlBusyCursor OL_ARGS((
	Screen *	screen
));
extern Cursor GetOlPanCursor OL_ARGS((
	Screen *	screen
));
extern Cursor GetOlQuestionCursor OL_ARGS((
	Screen *	screen
));
extern Cursor GetOlTargetCursor OL_ARGS((
	Screen *	screen
));
extern Cursor GetOlStandardCursor OL_ARGS((
	Screen *	screen
));
extern Pixmap OlGet50PercentGrey OL_ARGS((
	Screen *	screen
));
extern Pixmap OlGet75PercentGrey OL_ARGS((
	Screen *	screen
));

OLEndFunctionPrototypeBlock

#endif
