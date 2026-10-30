/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)popupwindo:include/Xol/PopupWindP.h	1.7"
#endif

#ifndef _PopupWindowShellP_h
#define _PopupWindowShellP_h
#include	<X11/ShellP.h>
#include	<Xol/PopupWindo.h>

typedef struct
{
	int keep_compiler_happy;   /* No new procedures */
} PopupWindowShellClassPart;

typedef struct _PropWindowClassRec {
  	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ShellClassPart		shell_class;
	WMShellClassPart	wm_shell_class;
	VendorShellClassPart	vendor_shell_class;
	TransientShellClassPart		transient_shell_class;
	PopupWindowShellClassPart	popup_shell_class;
} PopupWindowShellClassRec;

extern PopupWindowShellClassRec popupWindowShellClassRec;

/* New fields for the Open Look Popup Window widget */

typedef struct {
	Widget		upperControlArea;
	Widget		lowerControlArea;
	Widget		footerPanel;
	Boolean		propchange;
	XtCallbackList	apply;
	XtCallbackList	setDefaults;
	XtCallbackList	reset;
	XtCallbackList	resetFactory;
	XtCallbackList	verify;
	Widget		menu;
} PopupWindowShellPart;

typedef struct _PopupWindowShellRec
{
	CorePart 	core;
	CompositePart 	composite;
	ShellPart 	shell;
	WMShellPart	wm;
	VendorShellPart	vendor;
	TransientShellPart	transient;
	PopupWindowShellPart	popupwindow;
} PopupWindowShellRec;

#endif
