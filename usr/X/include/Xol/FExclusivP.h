/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)flat:FExclusivP.h	1.18"
#endif

#ifndef _OL_FEXCLUSIVP_H
#define _OL_FEXCLUSIVP_H

/*
 ************************************************************************	
 * Description:
 *	This is the flat exclusive container's private header file.
 ************************************************************************	
 */

#include <Xol/FlatP.h>
#include <Xol/FExclusive.h>

/*
 ************************************************************************	
 * Define Widget Class Part and Class Rec
 ************************************************************************	
 */

				/* Define new fields for the class part	*/

typedef struct {
    Boolean	allow_class_default;	/* is default item allowed ?	*/
} FlatExclusivesClassPart;

				/* Full class record declaration 	*/

typedef struct _FlatExclusivesClassRec {
    CoreClassPart		core_class;
    PrimitiveClassPart		primitive_class;
    FlatClassPart		flat_class;
    FlatExclusivesClassPart	exclusives_class;
} FlatExclusivesClassRec;

				/* External class record declaration	*/

extern FlatExclusivesClassRec		flatExclusivesClassRec;

/*
 ************************************************************************	
 * Define Widget Instance Structure
 ************************************************************************	
 */

				/* Define Expanded sub-object instance	*/

typedef struct {
	Boolean		set;		/* is this item set ?		*/
	Boolean		is_default;	/* is this item the default ?	*/
	XtPointer	client_data;	/* for callbacks		*/
	XtCallbackProc	select_proc;	/* select callback		*/
	XtCallbackProc	unselect_proc;	/* unselect callback		*/
} FlatExclusivesItemPart;

			/* Item's Full Instance record declaration	*/

typedef struct {
	FlatItemPart		flat;
	FlatExclusivesItemPart	exclusives;
} FlatExclusivesItemRec, *FlatExclusivesItem;

			/* Define new fields for the instance part	*/

typedef struct {
						/* Public Resources	*/

	Boolean		exclusive_settings;/* are settings exclusive ?	*/
	Boolean		none_set;	/* does a sub-object always have
					 * to be set ?			*/
	
						/* Private Resources	*/

	Boolean		menu_descendant; /* is the shell a menu ?	*/
	Boolean		allow_instance_default;/* can there be a default
					 * sub-object for this instance	*/
	Boolean		dim;		/* is the setting dimmed ?	*/
	Boolean		has_default;	/* some sub-object is a default	*/
	Cardinal	current_item;	/* the currect item		*/
	Cardinal	set_item;	/* previously selected item	*/
	Cardinal	default_item;	/* the default item		*/
	Cardinal	preview_item;	/* preview widget sub-object	*/
	Widget		preview;	/* widget to preview in		*/
	FlatExclusivesItemPart item_part;/* sub-object data for this part*/
	XtCallbackList	post_select;	/* call after select/unselect	*/
} FlatExclusivesPart;

				/* Full instance record declaration	*/

typedef struct _FlatExclusivesRec {
    CorePart		core;
    PrimitivePart	primitive;
    FlatPart		flat;
    FlatExclusivesPart	exclusives;
} FlatExclusivesRec;

#endif /* _OL_FEXCLUSIVP_H */
