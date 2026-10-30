/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)scrollwin:include/Xol/ScrolledWP.h	1.17"
#endif

/*
 * ScrolledWi.h
 *
 */

#ifndef _ScrolledWP_h
#define _ScrolledWP_h

#include <Xol/ManagerP.h>
#include <Xol/ScrolledWi.h>

typedef struct { int foo;} ScrolledWindowClassPart;

typedef struct _ScrolledWindowClassRec
   {
   CoreClassPart                 core_class;
   CompositeClassPart            composite_class;
   ConstraintClassPart           constraint_class;
   ManagerClassPart              manager_class;
   ScrolledWindowClassPart       scrolled_window_class;
   } ScrolledWindowClassRec;

extern ScrolledWindowClassRec scrolledWindowClassRec;

typedef void (*PFV)();

typedef struct _ScrolledWindowPart
   {
   /* resources */
   int                   current_page;
   OlDefine              show_page;
   int                   h_granularity;
   int                   h_initial_delay;
   Widget                h_menu_pane;
   int                   h_repeat_rate;
   XtCallbackList        h_slider_moved;
   int                   v_granularity;
   int                   v_initial_delay;
   Widget                v_menu_pane;
   int                   v_repeat_rate;
   XtCallbackList        v_slider_moved;
   PFV                   compute_geometries;
   Boolean               recompute_view_height;
   Boolean               recompute_view_width;
   Dimension             view_height;
   Dimension             view_width;
   Boolean               force_hsb;
   Boolean               force_vsb;
   int                   align_horizontal;
   int                   align_vertical;
   Boolean               vAutoScroll;
   Boolean               hAutoScroll;
   int                   init_x;
   int                   init_y;
   Pixel                 foreground;

   /* private state */
   BulletinBoardWidget   bboard;
   ScrollbarWidget       hsb;
   ScrollbarWidget       vsb;
   Widget                bb_child;
   Dimension             vborder;
   Dimension             hborder;
   Dimension             child_width;
   Dimension             child_bwidth;
   Dimension             child_height;
   OlgAttrs *		 attr_normal;
   } ScrolledWindowPart;

typedef struct _ScrolledWindowRec 
   {
   CorePart            core;
   CompositePart       composite;
   ConstraintPart      constraint;
   ManagerPart         manager;
   ScrolledWindowPart  scrolled_window;
   } ScrolledWindowRec;

#endif
