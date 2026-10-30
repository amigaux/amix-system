/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)scrollbar:include/openlook/ScrollP.h	1.21"
#endif

/*
 * ScrollbarP.h - Private definitions for Scrollbar widget
 *
 */

#ifndef	_ScrollbarP_h
#define	_ScrollbarP_h

/***********************************************************************
 *
 * Scroll Widget Private Data
 *
 ***********************************************************************/

#include <Xol/Scrollbar.h>
#include <Xol/PrimitiveP.h>	/* include superclasses's header */

#include <Xol/Olg.h>

/* Types of scrollbar */
/* The 2 LSBs is the number of parts in the elevator */
#define SB_REGULAR	3
#define SB_MINREG	7
#define SB_ABBREVIATED	2
#define SB_MINIMUM	6
#define EPARTMASK	0x3

/* Types of operation */
#define ANCHOR         1
#define DIR_INC        2
#define PAGE           4
#define ELEV_OP		8
#define DRAG_OP		16
#define KBD_OP		32	/* mouseless operation indicator */
#define ANCHOR_TOP     (ANCHOR)
#define ANCHOR_BOT     (ANCHOR | DIR_INC)
#define PAGE_DEC       (PAGE)
#define PAGE_INC       (PAGE | DIR_INC)
#define GRAN_DEC       (ELEV_OP)
#define GRAN_INC       (ELEV_OP | DIR_INC)
#define DRAG_ELEV      (ELEV_OP | DRAG_OP)
#define NOOP            255

/* New fields for the Scroll widget class record */
typedef	struct {int empty;} ScrollbarClassPart;

/* Full	class record declaration */
typedef	struct _ScrollbarClassRec {
	CoreClassPart		core_class;
	PrimitiveClassPart	primitive_class;
	ScrollbarClassPart	scroll_class;
} ScrollbarClassRec;

extern ScrollbarClassRec scrollbarClassRec;

/* New fields for the Scroll widget record */
typedef	struct {
	/* Public */
	int	sliderMin;		/* min slider in user scale */
	int	sliderMax;		/* max slider in user scale */
	int	sliderValue;		/* slider value	in user	scale */
	int	granularity;		/* granularity in user scale */
	int	scale;
	int	proportionLength;
	int	currentPage;
	int	repeatRate;
	int	initialDelay;
	XtCallbackList	sliderMoved;
	OlDefine orientation;
	OlDefine showPage;
	OlDefine dragtype;
	OlDefine stoppos;

	/* Private */
	GC	textGC;		/* gc for page indicator */
	OlgAttrs *pAttrs;	/* drawing attributes */
	XtIntervalId timerid;
	Widget	pane;
	Widget	popup;
	Widget	page_ind;
	Position dragbase;	/* starting pos. of ptr while dragging */
	Position absx;		/* abs x pos. used for displaying page ind */
	Position absy;		/* abs y pos. used for displaying page ind */
	Position sliderPValue;	/* slider value	in pixel */
	Position indPos;		/* indicator position in pixel */
	Position indLen;		/* indicator length in pixel */
	int	previous;	/* used	by "previous" menu button */
	int	XorY;		/* used	by menu	callback, set in menu()	*/
	Position  offset;		/* x offset of anchors and elevator */
	Boolean	warp_pointer;
	unsigned char type;	/* regular, abbreviated, or minimum */
	unsigned char opcode;	/* operation code while select is pressed */

	unsigned char anchwidth;/* anchor width	*/
	unsigned char anchlen;	/* anchor length */
	unsigned char elevwidth;	/* elevator width */
	unsigned char elevheight;	/* length of elevator	*/
/* stuff for mouseless operation */
	Widget here_to_lt_btn;		/* widget id for Here to Left/Top */
	Widget lt_to_here_btn;		/* widget id for Left/Top to Here */
} ScrollbarPart;

/****************************************************************
 *
 * Full	instance record	declaration
 *
 ****************************************************************/

typedef	struct _ScrollbarRec {
	CorePart	core;
	PrimitivePart	primitive;
	ScrollbarPart	scroll;
} ScrollbarRec;

#endif
