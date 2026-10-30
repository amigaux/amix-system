/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)primitive:PrimitiveP.h	1.11"
#endif

/***********************************************************************
 *
 * Primitive Widget Private Data
 *
 ***********************************************************************/

#ifndef _OlPrimitiveP_h
#define _OlPrimitiveP_h

#include <X11/CoreP.h>
#include <Xol/Primitive.h>

/***********************************************************************
 *
 * Class record
 *
 ***********************************************************************/

/* New fields for the PrimitiveWidget class record */
typedef struct {
    XtPointer			reserved1;	/* obsolete can_accept_focus */
    OlHighlightProc		highlight_handler;
    OlTraversalFunc		traversal_handler;
    OlRegisterFocusFunc		register_focus;
    OlActivateFunc		activate;
    OlEventHandlerList		event_procs;
    Cardinal			num_event_procs;
    XtVersionType		version;
    XtPointer			extension;
    _OlDynData			dyn_data;
    OlTransparentProc		transparent_proc;
} PrimitiveClassPart;

/* Full class record declaration */
typedef struct _PrimitiveClassRec {
    CoreClassPart	core_class;
    PrimitiveClassPart	primitive_class;
} PrimitiveClassRec;

extern PrimitiveClassRec	primitiveClassRec;


/***********************************************************************
 *
 * Instance record
 *
 ***********************************************************************/

/* New fields for the PrimitiveWidget record */
typedef struct _PrimitivePart {
    /* Resource-related data */
    XtPointer		user_data;
    Widget     		reference_widget;
    String		accelerator;
    String		accelerator_text;
    String		reference_name;
    XtCallbackList	consume_event;
    XFontStruct		*font;
    Pixel		input_focus_color;
    Pixel		font_color;
    Pixel		foreground;
    OlMnemonic		mnemonic;
    Boolean		traversal_on;

    /* Non-resource-related data */
    Boolean		has_focus;
    unsigned char	dyn_flags;
} PrimitivePart;


/* Full instance record declaration */
typedef struct _PrimitiveRec {
    CorePart		core;
    PrimitivePart	primitive;
} PrimitiveRec;


/***********************************************************************
 *
 * Constants
 *
 ***********************************************************************/

#define _OlIsPrimitive(w) (XtIsSubclass((w), primitiveWidgetClass))
#define _OL_IS_PRIMITIVE _OlIsPrimitive

/* dynamic resources bit masks */
#define OL_B_PRIMITIVE_BG		(1 << 0)
#define OL_B_PRIMITIVE_FG		(1 << 1)
#define OL_B_PRIMITIVE_FONTCOLOR	(1 << 2)
#define OL_B_PRIMITIVE_FOCUSCOLOR	(1 << 3)
#define OL_B_PRIMITIVE_BORDERCOLOR	(1 << 4)

#endif /* _OlPrimitiveP_h */
