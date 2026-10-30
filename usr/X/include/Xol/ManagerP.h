/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)manager:ManagerP.h	1.13"
#endif

/***********************************************************************
 *
 * Manager Widget Private Data
 *
 ***********************************************************************/

#ifndef _OlManagerP_h
#define _OlManagerP_h


#include <X11/ConstrainP.h>
#include <Xol/Manager.h>

/***********************************************************************
 *
 * Class record
 *
 ***********************************************************************/

/* New fields for the ManagerWidget class record */
typedef struct {
    OlHighlightProc	highlight_handler;
    XtPointer		reserved1;	/* obsolete insert_descendant */
    XtPointer		reserved2;	/* obsolete delete_descendant */
    OlTraversalFunc	traversal_handler;
    OlActivateFunc	activate;
    OlEventHandlerList	event_procs;
    Cardinal		num_event_procs;
    OlRegisterFocusFunc	register_focus;
    XtPointer		reserved4;
    XtVersionType	version;
    XtPointer		extension;
    _OlDynData		dyn_data;
    OlTransparentProc	transparent_proc;
} ManagerClassPart;

/* Full class record declaration */
typedef struct _ManagerClassRec {
    CoreClassPart	core_class;
    CompositeClassPart	composite_class;
    ConstraintClassPart	constraint_class;
    ManagerClassPart	manager_class;
} ManagerClassRec;

extern ManagerClassRec	managerClassRec;

/***********************************************************************
 *
 * Instance record
 *
 ***********************************************************************/

/* New fields for the ManagerWidget record */
typedef struct _ManagerPart {
    /* Resource-related data */
    XtPointer		user_data;
    XtCallbackList	consume_event;
    Pixel		input_focus_color;
    String		reference_name;
    Widget		reference_widget;
    Boolean		traversal_on;

    /* Non-resource-related data */
    Boolean		has_focus;
    unsigned char	dyn_flags;
} ManagerPart;


/* Full instance record declaration */
typedef struct _ManagerRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    ManagerPart		manager;
} ManagerRec;


/***********************************************************************
 *
 * Constants
 *
 ***********************************************************************/

#define MGRPART(w)		( &(((ManagerWidget)(w))->manager) )
#define MGRCLASSPART(wc)	( &(((ManagerWidgetClass)(wc))->manager_class) )
#define _OlIsManager(w)		XtIsSubclass((w), managerWidgetClass)
#define _OL_IS_MANAGER		_OlIsManager

/* dynamic resources bit values */
#define OL_B_MANAGER_BG			(1 << 0)
#define OL_B_MANAGER_FOCUSCOLOR		(1 << 1)
#define OL_B_MANAGER_BORDERCOLOR	(1 << 2)

#endif /* _OlManagerP_h */
