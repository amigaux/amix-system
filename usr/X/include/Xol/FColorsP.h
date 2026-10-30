/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)flat:FColorsP.h	1.1"
#endif

#ifndef _OL_FCOLORSP_H
#define _OL_FCOLORSP_H

#include <Xol/FExclusivP.h>
#include <Xol/FColors.h>

/*
 * Class structure:
 */

typedef struct _FlatColorsClassPart {
	int			unused;
}			FlatColorsClassPart;

typedef struct _FlatColorsClassRec {
	CoreClassPart		core_class;
	PrimitiveClassPart	primitive_class;
	FlatClassPart		flat_class;
	FlatExclusivesClassPart	exclusives_class;
	FlatColorsClassPart	colors_class;
}			FlatColorsClassRec;

extern FlatColorsClassRec flatColorsClassRec;

/*
 * Item structure:
 */

typedef struct _FlatColorsItemPart {
	int			unused;
}			FlatColorsItemPart;

typedef struct _FlatColorsItemRec {
	FlatItemPart		flat;
	FlatExclusivesItemPart	exclusives;
	FlatColorsItemPart	colors;
}			FlatColorsItemRec, *FlatColorsItem;

/*
 * Instance structure:
 */

typedef struct _FlatColorsPart {
	FlatColorsItemPart	item_part;
}			FlatColorsPart;

typedef struct _FlatColorsRec {
	CorePart		core;
	PrimitivePart		primitive;
	FlatPart		flat;
	FlatExclusivesPart	exclusives;
	FlatColorsPart		colors;
}			FlatColorsRec;

#endif /* _OL_FCOLORSP_H */
