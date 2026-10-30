/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)rubbertile:RubberTile.h	1.1"
#endif

#ifndef _RUBBERTILE_H
#define _RUBBERTILE_H

#include "Xol/Manager.h"

extern char			XtNrefName   [];
extern char			XtNrefWidget [];
extern char			XtNweight    [];

extern char			XtCRefName   [];
extern char			XtCRefWidget [];
extern char			XtCWeight    [];

extern WidgetClass			rubberTileWidgetClass;

typedef struct _RubberTileClassRec *		RubberTileWidgetClass;
typedef struct _RubberTileRec *			RubberTileWidget;
typedef struct _RubberTileConstraintRec *	RubberTileConstraints;

#endif /* _RUBBERTILE_H */
