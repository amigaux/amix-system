/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)flat:FCheckBoxP.h	1.4"
#endif

#ifndef _OL_FCHECKBOXP_H
#define _OL_FCHECKBOXP_H

/*
 ************************************************************************	
 * Description:
 *	This is the flat checkbox container's private header file.
 ************************************************************************	
 */

#include <Xol/FExclusivP.h>
#include <Xol/FCheckBox.h>

/*
 ************************************************************************	
 * Define Widget Class Part and Class Rec
 ************************************************************************	
 */

				/* Define new fields for the class part	*/

typedef struct {
    int		no_class_fields;
} FlatCheckBoxClassPart;

				/* Full class record declaration 	*/

typedef struct _FlatCheckBoxClassRec {
    CoreClassPart		core_class;
    PrimitiveClassPart		primitive_class;
    FlatClassPart		flat_class;
    FlatExclusivesClassPart	exclusives_class;
    FlatCheckBoxClassPart	checkbox_class;
} FlatCheckBoxClassRec;

				/* External class record declaration	*/

extern FlatCheckBoxClassRec	flatCheckBoxClassRec;

/*
 ************************************************************************	
 * Define Widget Instance Structure
 ************************************************************************	
 */
				/* Define Expanded sub-object instance	*/

typedef struct {
	OlDefine	position;	/* position of check box	*/
} FlatCheckBoxItemPart;

			/* Item's Full Instance record declaration	*/

typedef struct {
	FlatItemPart		flat;
	FlatExclusivesItemPart	exclusives;
	FlatCheckBoxItemPart	checkbox;
} FlatCheckBoxItemRec, *FlatCheckBoxItem;

			/* Define new fields for the instance part	*/
typedef struct {
			/* Must finish this in next load	*/

	FlatCheckBoxItemPart	item_part;
} FlatCheckBoxPart;

				/* Full instance record declaration	*/

typedef struct _FlatCheckBoxRec {
    CorePart		core;
    PrimitivePart	primitive;
    FlatPart		flat;
    FlatExclusivesPart	exclusives;
    FlatCheckBoxPart	checkbox;
} FlatCheckBoxRec;

#endif /* _OL_FCHECKBOXP_H */
