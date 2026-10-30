/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)flat:FNonexcluP.h	1.3"
#endif

#ifndef _OL_FNONEXCLUP_H
#define _OL_FNONEXCLUP_H

/*
 ************************************************************************	
 * Description:
 *	This is the flat non-exclusive container's private header file.
 ************************************************************************	
 */

#include <Xol/FExclusivP.h>
#include <Xol/FNonexclus.h>

/*
 ************************************************************************	
 * Define Widget Class Part and Class Rec
 ************************************************************************	
 */

				/* Define new fields for the class part	*/

typedef struct {
    int		no_class_fields;
} FlatNonexclusivesClassPart;

				/* Full class record declaration 	*/

typedef struct _FlatNonexclusivesClassRec {
    CoreClassPart		core_class;
    PrimitiveClassPart		primitive_class;
    FlatClassPart		flat_class;
    FlatExclusivesClassPart	exclusives_class;
    FlatNonexclusivesClassPart	nonexclusives_class;
} FlatNonexclusivesClassRec;

				/* External class record declaration	*/

extern FlatNonexclusivesClassRec	flatNonexclusivesClassRec;

/*
 ************************************************************************	
 * Define Widget Instance Structure
 ************************************************************************	
 */

typedef struct {
	int		no_instance_fields;
} FlatNonexclusivesPart;

				/* Full instance record declaration	*/

typedef struct _FlatNonexclusivesRec {
    CorePart			core;
    PrimitivePart		primitive;
    FlatPart			flat;
    FlatExclusivesPart		exclusives;
    FlatNonexclusivesPart	nonexclusives;
} FlatNonexclusivesRec;

#endif /* _OL_FNONEXCLUP_H */
