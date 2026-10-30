/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)caption:include/openlook/CaptionP.h	1.18"
#endif

/* 
** CaptionP.h - Private definitions for Open Look Caption Widget
*/

#include <Xol/ManagerP.h>	/* include superclasses' header */
#include <Xol/Caption.h>
#include <Xol/Olg.h>

#ifndef _CaptionP_h
#define _CaptionP_h

typedef struct {
	int make_compiler_happy;
	} CaptionClassPart;

/* Full class record declaration */
typedef struct _CaptionClassRec {
    CoreClassPart	core_class;
    CompositeClassPart	composite_class;
    ConstraintClassPart	constraint_class;
    ManagerClassPart	manager_class;
    CaptionClassPart	caption_class;
} CaptionClassRec;

extern CaptionClassRec captionClassRec;

/* New fields for the Caption widget record */

typedef struct {
	/* resources */
	Pixel		fontcolor;
	XFontStruct	*font;
	String		label;
	OlDefine	position;
	OlDefine	alignment;
	Dimension	space;
	OlMnemonic	mnemonic;

	/* internal data */
	GC         	normal_GC; 	/* GC for text			*/
	GC         	hilite_GC; 	/* GC for highlighted text	*/
	Dimension	caption_len;	/* len of text in chars		*/
	Dimension	caption_width;	/* width of text in pixels	*/
	Dimension	caption_height;	/* height of caption in pixels	*/
	Position	caption_x;	/* where text is placed		*/
	Position	caption_y;	/* where text is placed		*/
	OlgAttrs	*pAttrs;	/* drawing attributes */
	unsigned char	dyn_flags;

} CaptionPart;

/*
** Full instance record declaration
*/

typedef struct _CaptionRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    ManagerPart		manager;
    CaptionPart		caption;
} CaptionRec;

/* dynamics resource bit masks */
#define OL_B_CAPTION_FONTCOLOR	(1 << 0)
#endif /* _CaptionP_h */
