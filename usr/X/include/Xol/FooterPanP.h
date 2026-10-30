/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)panel:include/Xol/FooterPanP.h	1.4"
#endif
/*
 FooterPanP.h (C hdr file)
	Acc: 602455388 Thu Feb  2 15:43:08 1989
	Mod: 601689973 Tue Jan 24 19:06:13 1989
	Sta: 601689973 Tue Jan 24 19:06:13 1989
	Owner: 4777
	Group: 1985
	Permissions: 666
*/
/*
	START USER STAMP AREA
*/
/*
	END USER STAMP AREA
*/
/* 
 * FooterPanP.h - Private definitions for FooterPanel widget
 * 
 */

#ifndef _FooterPanP_h
#define _FooterPanP_h

#include <Xol/ManagerP.h>		/* include supclasses' header */
#include <Xol/FooterPane.h>		/* include public header */

/***********************************************************************
 *
 * Class structure
 *
 */

/* New fields for the FooterPanel widget class record */
typedef struct {
	int no_class_fields;		/* make compiler happy */
} FooterPanelClassPart;

/* Full class record declaration */
typedef struct _FooterPanelClassRec {
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	ManagerClassPart	manager_class;
	FooterPanelClassPart	footer_panel_class;
} FooterPanelClassRec;

extern FooterPanelClassRec footerPanelClassRec;


/***********************************************************************
 *
 * Instance (widget) structure
 *
 */

/* New fields for the FooterPanel widget record */
typedef struct {
	int no_class_fields;		/* make compiler happy */
} FooterPanelPart;


/* Full instance record declaration */
typedef struct _FooterPanelRec {
	CorePart	core;
	CompositePart	composite;
	ConstraintPart	constraint;
	ManagerPart	manager;
	FooterPanelPart	footer_panel;
} FooterPanelRec;

/***********************************************************************
 *
 * Constants / Macros
 */

#endif /* _FooterPanP_h */
