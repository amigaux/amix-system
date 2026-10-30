/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)statictext:include/Xol/StaticTexP.h	1.7.2.5"
#endif

/*************************************<+>*************************************
 *****************************************************************************
 **
 **   File:        StaticTexP.h    
 **
 **   Project:     X Widgets
 **
 **   Description: Private include file for StaticText class
 **
 *****************************************************************************
 **   
 **   Copyright (c) 1988 by Hewlett-Packard Company
 **   Copyright (c) 1988 by the Massachusetts Institute of Technology
 **   
 *****************************************************************************
 *************************************<+>*************************************/

#include <Xol/PrimitiveP.h>	/* include superclasses's header */
#include <Xol/StaticText.h>

/********************************************
 *
 *   No new fields need to be defined
 *   for the StaticText widget class record
 *
 ********************************************/
typedef struct 
{
	int	make_compiler_happy;
} StaticTextClassPart;

/****************************************************
 *
 * Full class record declaration for StaticText class
 *
 ****************************************************/
typedef struct _StaticTextClassRec {
	CoreClassPart      	core_class;
	PrimitiveClassPart	primitive_class;
	StaticTextClassPart	statictext_class;
} StaticTextClassRec;

/********************************************
 *
 * New fields needed for instance record
 *
 ********************************************/
typedef struct _StaticTextPart {
	/*
	 * "Public" members (Can be set by resource manager).
	 */
	char       	*input_string;	/* String sent to this widget. */
	OlDefine	alignment;	/* Alignment within the box */
	Boolean    	wrap;		/* Controls wrapping on spaces */
	Boolean		strip;		/* Controls stripping of blanks */
	int	       	gravity;	/* Controls extra space in window */
	int     	line_space;	/* Ratio of font height as dead space
					   between lines.  Can be less than zero
					   but not less than -1.0  */
	Dimension 	internal_height; /* Space from text to top and bottom highlights */
	Dimension 	internal_width; /* Space from left and right side highlights */
	Boolean		recompute_size;

/*
** Fields taken from XwPrimitive
*/

	Dimension	highlight_thickness;

/*
 * "Private" fields, used by internal widget code.
 */

	GC         	normal_GC; 	/* GC for text			*/
	GC         	hilite_GC; 	/* GC for highlighted text	*/
	GC         	cursor_GC; 	/* GC for x-or'ed cursor	*/
	XRectangle 	TextRect; 	/* The bounding box of the text, or clip rectangle of the window; whichever is smaller. */
	char       	*output_string; /* input_string after formatting*/
	char		*selection_start;	/* ptr to start of selection*/
	char		*selection_end;	/* ptr to end of selection	*/
	char		*oldsel_start;	/* old start of selection	*/
	char		*oldsel_end;	/* old end of selection		*/
	int		selection_mode;	/* chars, words, lines...	*/
	Time            time;           /* time of last key or button action*/ 
	Position        ev_x, ev_y;     /* coords for key or button action*/
	Position        old_x, old_y;   /* previous key or button coords*/
	char		**line_table;	/* ptrs to start of each line	*/
	int		line_count;	/* count of line_table		*/
	char		*clip_contents;	/* contents of clipboard	*/

} StaticTextPart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/
typedef struct _StaticTextRec {
	CorePart      	core;
	PrimitivePart	primitive;
	StaticTextPart	static_text;
} StaticTextRec;

