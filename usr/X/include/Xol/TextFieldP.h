/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)textfield:include/Xol/TextFieldP.h	1.16"
#endif

/*
 * TextFieldP.h
 *
 */

#ifndef _TextFieldP_h
#define _TextFieldP_h

#include <Xol/ManagerP.h>        /* include superclasses' header */
#include <Xol/TextField.h>
#include <Xol/Olg.h>

/* dynamic resource bit masks */
#define OL_B_TEXTFIELD_BG	(1 << 0)
#define OL_B_TEXTFIELD_FG	(1 << 1)

typedef enum { POLL_LEFT, POLL_RIGHT, POLL_OFF } PollMode;

typedef struct {
	int keep_compiler_happy;   /* No new procedures */
} TextFieldClassPart;

typedef struct _TextFieldClassRec {
	CoreClassPart         core_class;
	CompositeClassPart    composite_class;
	ConstraintClassPart   constraint_class;
	ManagerClassPart      manager_class;
	TextFieldClassPart    textfield_class;
} TextFieldClassRec;

typedef struct {
	char *          string;
	Widget          textWidget;
	int             initialDelay;
	int             repeatRate;
	int             maximumSize;
	int             charsVisible;
	XtCallbackList  verification;
	Dimension       arrowWidth;
	Dimension       arrowHeight;
	XFontStruct *   font;
	char            l_on;
	char            r_on;
	unsigned char	dyn_flags;
	Boolean		insertTab;
	PollMode        polling;
	OlgAttrs	*pAttrs;	/* drawing attributes */
	Pixel		foreground;
} TextFieldPart;

typedef struct _TextFieldRec {
	CorePart        core;
	CompositePart   composite;
	ConstraintPart  constraint;
	ManagerPart     manager;
	TextFieldPart   textField;
} TextFieldRec;

extern TextFieldClassRec textFieldClassRec;

#endif
