/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)rubbertile:RubberTilP.h	1.3"
#endif

#ifndef _RUBBERTILP_H
#define _RUBBERTILP_H

#include "Xol/ManagerP.h"
#include "Xol/RubberTile.h"

/*
 * Constraint record:
 */

typedef struct	_RubberTileConstraintRec {
	/*
	 * Public:
	 */
	String			ref_name;
	Widget			ref_widget;
	Dimension		space;
	Dimension		weight;

	/*
	 * Private:
	 */
	Dimension		set_size;
}			RubberTileConstraintRec;

/*
 * Class strucure:
 */

typedef struct _RubberTileClassPart {
	int			no_class_fields;
} RubberTileClassPart;

typedef struct _RubberTileClassRec {
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	ManagerClassPart	manager_class;
	RubberTileClassPart	rubber_tile_class;
}			RubberTileClassRec;

extern RubberTileClassRec	rubberTileClassRec;

/*
 * Instance structure:
 */

typedef struct _RubberTilePart {
	/*
	 * Public:
	 */
	OlDefine		orientation;

	/*
	 * Private:
	 */
	Widget *		managed_children;
	Cardinal		num_managed_children;
}			RubberTilePart;

typedef struct _RubberTileRec {
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	ManagerPart		manager;
	RubberTilePart		rubber_tile;
}			RubberTileRec;

#endif /* _RUBBERTILP_H */
