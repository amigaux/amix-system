/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)flat:FlatP.h	1.22"
#endif

#ifndef _OL_FLATP_H
#define _OL_FLATP_H

/*
 ************************************************************************	
 * Description:
 *	This is the flat container's private header file.
 ************************************************************************	
 */

#include <Xol/PrimitiveP.h>
#include <Xol/Flat.h>
#include <Xol/Olg.h>

/*
 ************************************************************************	
 * Define structures and names/types needed to support flat widgets
 ************************************************************************	
 */

typedef Boolean	(*OlFlatReqRscPredicateFunc) OL_ARGS((
	Widget, 	/* w;		flattened widget id	*/
	Cardinal,	/* offset;	resource offset		*/
	String		/* name		require resource name	*/
));

typedef struct {
	String				name;		/* Resource name*/
	OlFlatReqRscPredicateFunc	predicate;	/* Use it ?	*/
	Cardinal			rsc_index;	/* private	*/
} OlFlatReqRsc, *OlFlatReqRscList;

			/* Define a structure that's used to extract
			 * information from the application's list and
			 * store indexes of required resources.		*/
typedef struct {
	Cardinal *		xlist;	/* resource extractor list	*/
	Cardinal		num_xlist; /* number of xlist elements	*/
	Cardinal *		rlist;/* required resource extractor list*/
	Cardinal		num_rlist; /* number of rlist elements	*/
	char *			rdata;	/* place to hold data		*/
} OlFlatResourceInfo;

			/* Create a structure for holding transient
			 * geometry information				*/

typedef struct {
	Cardinal	rows;		/* number of rows		*/
	Cardinal	cols;		/* number of columns		*/
	Position	x_offset;	/* x coordinate of first item	*/
	Position	y_offset;	/* y coordinate of first item	*/
	Dimension	bounding_width;	/* width tightly bounding items	*/
	Dimension	bounding_height;/* height tightly bounding items*/
	Dimension *	col_widths;	/* array of column widths	*/
	Dimension *	row_heights;	/* array of row heights		*/
} OlFlatGeometry;

			/* Define a structure used to Draw Sub-objects	*/

typedef struct {
	Drawable	drawable;	/* place to draw	*/
	Screen *	screen;		/* drawable's screen	*/
	Pixel		background;	/* drawable's backgrnd	*/
	Position	x;		/* item's X coordinate	*/
	Position	y;		/* item's Y coordinate	*/
	Dimension	width;		/* item's width		*/
	Dimension	height;		/* item's height	*/
	Boolean		clipped;	/* is it clipped ?	*/
	Dimension	clip_width;	/* clip width		*/
	Dimension	clip_height;	/* clip height		*/
} OlFlatDrawInfo;

				/* Define a structure to cache information
				 * on a per screen basis		*/

typedef struct _OlFlatScreenCache {
	Screen *		screen;		/* screen id		*/
	struct _OlFlatScreenCache * next;	/* next node in list	*/
	Cardinal		count;		/* reference count	*/
	XFontStruct *		font;		/* OPEN LOOK Font	*/
	Pixmap			stipple;	/* Stippled pixmap	*/
	GC			default_gc;	/* default GC		*/
	GC			scratch_gc;	/* scratch GC		*/
	OlgAttrs *		alt_attrs;	/* scratch attributes	*/
	Pixel			alt_bg;		/* scratch attrs bg	*/
	Pixel			alt_fg;		/* scratch attrs fg	*/
	Dimension		stroke_width;	/* brush stroke width	*/
	Dimension		abbrev_width;	/* points cvted to pixels*/
	Dimension		abbrev_height;	/* points cvted to pixels*/
	Dimension		menu_mark;	/* points cvted to pixels*/
	Dimension		oblong_radius;	/* points cvted to pixels*/
	Dimension		check_width;	/* points cvted to pixels*/
	Dimension		check_height;	/* points cvted to pixels*/
} OlFlatScreenCache, *OlFlatScreenCacheList;

#define OL_JUST_LOOKING	1		/* OlFlatScreenManager flag	*/
#define OL_ADD_REF	2		/* OlFlatScreenManager flag	*/
#define OL_DELETE_REF	3		/* OlFlatScreenManager flag	*/

/*
 ************************************************************************	
 * Define Expanded Sub-object Instance Structure
 ************************************************************************	
 */

typedef struct {
	OlDefine	label_justify;	/* justification of the labels	*/
	Boolean		managed;	/* is this item managed ?	*/
	Boolean		mapped_when_managed; /* mapped when managed ?	*/
	Boolean		sensitive;	/* is this item sensitive ?	*/
	Boolean		ancestor_sensitive; /* ancestor sensitive ?	*/
	Boolean		label_tile;	/* tile the label ?		*/
	Boolean		traversal_on;	/* is this item traversable ?	*/
	OlMnemonic	mnemonic;	/* mnemonic			*/
	Pixmap		background_pixmap; /* Background pixmap		*/
	Pixel		background_pixel;/* Background color		*/
	Pixel		foreground;	/* foreground			*/
	Pixel		font_color;	/* font color			*/
	Pixel		input_focus_color;
	XtPointer	user_data;	/* application data hook	*/
	String		accelerator;	/* accelerator binding		*/
	String		accelerator_text; /* accelerator display string	*/
	String		label;		/* textual label		*/
	XImage *	label_image;	/* Image label			*/
	XFontStruct *	font;		/* font struct			*/
	Cardinal	item_index;	/* the index of this item	*/
} FlatItemPart;

			/* Item's Full Instance record declaration	*/

typedef struct {
	FlatItemPart	flat;
} FlatItemRec, *FlatItem;

/*
 ************************************************************************	
 * Define Widget Instance Structure
 ************************************************************************	
 */
			/* Define new fields for the instance part	*/

typedef struct _FlatPart {
						/* layout resources	*/

	Dimension	h_pad;		/* horizontal margin padding	*/
	Dimension	h_space;	/* internal horizontal padding	*/
	Dimension	item_max_height;/* item's maximum allow. height	*/
	Dimension	item_max_width;	/* item's maximum allow. width	*/
	Dimension	item_min_height;/* item's minimum allow. height	*/
	Dimension	item_min_width;	/* item's minimum allow. width	*/
	Dimension	overlap;	/* overlap of each item		*/
	Dimension	v_pad;		/* vertical margin padding	*/
	Dimension	v_space;	/* internal vertical padding	*/
	int		gravity;	/* gravity for sub-object group	*/
	int		item_gravity;	/* gravity for each sub-object	*/
	int		measure;	/* layout related dimension	*/
	OlDefine	layout_height;	/* height boundary constraint	*/
	OlDefine	layout_type;	/* type of desired layout	*/
	OlDefine	layout_width;	/* width boundary constraint	*/
	OlDefine	same_height;	/* forcing of sub-object height	*/
	OlDefine	same_width;	/* forcing of sub-object width	*/

					/* Sub-object related resources	*/

	XtPointer	items;		/* sub-object list		*/
	String *	item_fields;	/* array of fields in item list	*/
	Cardinal	num_item_fields;/* Number of fields per item	*/
	Cardinal	num_items;	/* number of sub-object items	*/
	Cardinal	focus_item;	/* item with focus		*/
	Cardinal	last_focus_item;/* last item to have focus	*/
	OlFlatResourceInfo resource_info; /* resource information	*/
	OlFlatGeometry	item_geom;	/* calculated item geometry	*/
	FlatItemPart	item_part;	/* default item attributes	*/
	Boolean		items_touched;	/* application touched items	*/
	unsigned char	flags;		/* flags used to maintain state	*/
	String *	acc_text;	/* list of accelerator text	*/
	GC		label_gc;
	GC		inverse_gc;
	OlgAttrs *	pAttrs;
#define OL_B_FLAT_RELAYOUT_HINT		(1<<0)	/* need a relayout	*/
#define OL_B_FLAT_ACCELERATOR_TEXT	(1<<1)	/* internal acc. text.	*/
} FlatPart;

				/* Full instance record declaration	*/

typedef struct _FlatRec {
    CorePart		core;
    PrimitivePart	primitive;
    FlatPart		flat;
} FlatRec;

			/* Define new function pointers for the flat
			 * widget class.				*/

typedef void	(*OlFlatAnalyzeItemsProc)OL_ARGS((
	Widget,		/* w;		container widget id		*/
	ArgList,	/* args;	args that created items		*/
	Cardinal *	/* num_args;	number of args			*/
));

typedef void	(*OlFlatDrawItemProc) OL_ARGS((
	Widget,		/* w;		container widget id		*/
	FlatItem,	/* item;	expanded item			*/
	OlFlatDrawInfo * /* draw_info;	Information used in drawing	*/
));

typedef void	(*OlFlatExpandItemProc) OL_ARGS((
	Widget,		/* w;		flat widget container		*/
	FlatItem	/* item;	expanded item			*/
));

typedef void (*OlFlatGetDrawInfoProc) OL_ARGS((
	Widget,		/* w;		Flat widget type		*/
	Cardinal,	/* item_index;	Item to get drawing info. for	*/
	OlFlatDrawInfo * /* draw_info;	Information used in drawing	*/
));

typedef Cardinal	(*OlFlatGetIndexFunc) OL_ARGS((
	Widget,		/* w;			Flat widget type	*/
	Position,	/* x;			X source location	*/
	Position,	/* y;			Y source location	*/
	Boolean		/* ignore_sensitivity;	don't look at sensitivity*/
));

typedef Boolean	(*OlFlatItemAcceptFocusFunc) OL_ARGS((
	Widget,		/* w		container widget id		*/
	FlatItem,	/* item;	item to accept focus		*/
	Time		/* time		time when request was made	*/
));

typedef Boolean	(*OlFlatItemActivateFunc) OL_ARGS((
	Widget,		/* w;		container widget id		*/
	FlatItem,	/* item;	item to activate		*/
	OlVirtualName,	/* type;	activation type			*/
	XtPointer	/* data;	activation data			*/
));

typedef void	(*OlFlatItemDimensionsProc) OL_ARGS((
	Widget,		/* w;		Flat widget container id	*/
	FlatItem,	/* item;	expanded item			*/
	Dimension *,	/* width;	returned width			*/
	Dimension *	/* height;	returned height			*/
));

typedef void	(*OlFlatItemGetValuesProc) OL_ARGS((
	Widget,		/* widget;	flat widget container		*/
	FlatItem,	/* item;	expanded item			*/
	ArgList,	/* args;	item Args			*/
	Cardinal *	/* num_args;	num item Args			*/
));

typedef void	(*OlFlatItemHighlightProc) OL_ARGS((
	Widget,		/* w;		flat widget id			*/
	FlatItem,	/* item;	item being affected		*/
	OlDefine	/* type;	highlight type			*/
));

typedef void	(*OlFlatItemInitializeProc) OL_ARGS((
	Widget,		/* w;		Flat widget container id	*/
	FlatItem,	/* request;	expanded requested item		*/
	FlatItem,	/* new;		expanded new item		*/
	ArgList,	/* args;	args that created items		*/
	Cardinal *	/* num_args;	number of args			*/
));

typedef Boolean	(*OlFlatItemSetValuesFunc) OL_ARGS((
	Widget,		/* w;		flat widget container		*/
	FlatItem,	/* current;	item application has		*/
	FlatItem,	/* request;	item application wants		*/
	FlatItem,	/* new;		item application gets		*/
	ArgList,	/* args;	item Args			*/
	Cardinal *	/* num_args;	num item Args			*/
));

typedef void	(*OlFlatItemsTouchedProc) OL_ARGS((
	Widget,		/* current;	current flat widget or NULL	*/
	Widget,		/* request;	request flat widget		*/
	Widget,		/* new;		new flat widget			*/
	ArgList,	/* args;	item Args			*/
	Cardinal *	/* num_args;	num item Args			*/
));

typedef void	(*OlFlatLayoutProc) OL_ARGS((
	Widget,		/* w;		Flat widget container id	*/
	Dimension *,	/* return_width;  returned container width	*/
	Dimension *	/* return_height; returned container height	*/
));

typedef Cardinal	(*OlFlatTraverseItemsFunc) OL_ARGS((
	Widget,		/* w;		flat widget container		*/
	Cardinal,	/* item_index;	item index to start at		*/
	OlVirtualName,	/* direction;	direction to move		*/
	Time		/* time;	the time			*/
));

			/* Define inheritance procedures for the class	*/

#define XtInheritFlatDrawItem		((OlFlatDrawItemProc) _XtInherit)
#define XtInheritFlatGetDrawInfo	((OlFlatGetDrawInfoProc)_XtInherit)
#define XtInheritFlatGetIndex		((OlFlatGetIndexFunc) _XtInherit)
#define XtInheritFlatItemAcceptFocus	((OlFlatItemAcceptFocusFunc) _XtInherit)
#define XtInheritFlatItemActivate	((OlFlatItemActivateFunc) _XtInherit)
#define XtInheritFlatItemHighlight	((OlFlatItemHighlightProc) _XtInherit)
#define XtInheritFlatItemDimensions	((OlFlatItemDimensionsProc) _XtInherit)
#define XtInheritFlatLayout		((OlFlatLayoutProc) _XtInherit)
#define XtInheritFlatLayoutClass	((WidgetClass)_XtInherit)
#define OlThisFlatClass			((WidgetClass)OL_IGNORE)
#define XtInheritFlatTraverseItems	((OlFlatTraverseItemsFunc) _XtInherit)

/*
 ************************************************************************	
 * Declare external routines and macros used by the flat widgets.
 * External routines exist for chained class procedures only.
 * Macros exist for non-chained class procedures, they provide a
 * way of checking the number of parameters when using a pointers
 * to functions.
 ************************************************************************	
 */

OLBeginFunctionPrototypeBlock

extern void
_OlDoGravity OL_ARGS((
	int,		/* gravity;	the desired gravity		*/
	Dimension,	/* c_width;	container's width		*/
	Dimension,	/* c_height;	container's height		*/
	Dimension,	/* width;	interior item width		*/
	Dimension,	/* height;	interior item height		*/
	Position *,	/* x;		returned x offset		*/
	Position *	/* y;		returned y offset		*/
));

extern void
_OlFlatAddConverters OL_NO_ARGS();

extern Boolean
_OlFlatCheckItems OL_ARGS((
	WidgetClass,	/* wc;		subclass's id			*/
	Widget,		/* current;	current flat widget or NULL	*/
	Widget,		/* request;	request flat widget		*/
	Widget,		/* new;		new flat widget			*/
	ArgList,	/* args;	args that created items		*/
	Cardinal *	/* num_args;	number of args			*/
));

extern void
_OlFlatExpandItem OL_ARGS((
	Widget,		/* w;		flat widget container		*/
	Cardinal,	/* item_index;	item to be expanded		*/
	FlatItem	/* item;	memory to expand into		*/
));

extern void
_OlFlatInitializeItems OL_ARGS((
	Widget,		/* current;	current flat widget or NULL	*/
	Widget,		/* request;	request flat widget		*/
	Widget,		/* new;		new flat widget			*/
	ArgList,	/* args;	container Args			*/
	Cardinal *	/* num_args;	num container Args		*/
));

extern void
_OlFlatPreviewItem OL_ARGS((
	Widget,		/* w;		container widget id		*/
	Cardinal,	/* item_index;	Item to draw			*/
	Widget,		/* preview;	destination preview widget	*/
	Cardinal	/* preview_index; sub-object within preview widget*/
));

extern void
_OlFlatRefreshItem OL_ARGS((
	Widget,		/* w;		container widget id		*/
	Cardinal,	/* item_index;	Item to draw			*/
	Boolean		/* clear_area;	should the area be cleared?	*/
));

extern OlFlatScreenCache *
_OlFlatScreenManager OL_ARGS((
	Widget,		/* w;		any widget id			*/
	Cardinal,	/* point_size;	point size of information	*/
	OlDefine	/* flag;	OlFlatScreenManager flag	*/
));

extern void
_OlFlatSetupLabelSize OL_ARGS((
	Widget,		/* w;		Widget making request		*/
	FlatItem,	/* item_rec;	expanded item			*/
	XtPointer *,	/* ppLbl;	pointer to label structure	*/
	void (**)()	/* ppSizeProc;	pointer to size procedure	*/
));

extern void
_OlFlatSetupAttributes OL_ARGS((
	Widget,		/* w;		flat widget container		*/
	FlatItem,	/* item_rec;	expanded item			*/
	OlFlatDrawInfo *, /* di;	drawing information		*/
	OlgAttrs **,	/* ppAttrs;	returned pointer to attributes	*/
	XtPointer *,	/* ppLbl;	returned pointer to label	*/
	void (**)()	/* ppDrawProc;    returned ptr to lbl drawer */
));

OLEndFunctionPrototypeBlock

#define OL_FLATCLASS(w,field) \
			(((FlatWidgetClass)XtClass(w))->flat_class.field)

#define _OlFlatDrawItem(w,ei,d)		(*OL_FLATCLASS(w,draw_item))(w,ei,d)
#define _OlFlatGetDrawInfo(w,i,d)	(*OL_FLATCLASS(w,get_draw_info))(w,i,d)
#define _OlFlatGetIndex(w,x,y,f)	(*OL_FLATCLASS(w,get_index))(w,x,y,f)
#define _OlFlatItemActivate(w,i,t,d) \
				(*OL_FLATCLASS(w,item_activate))(w,i,t,d)
#define _OlFlatItemAcceptFocus(w,i,t) \
				(*OL_FLATCLASS(w,item_accept_focus))(w,i,t)
#define _OlFlatItemDimensions(w,ei,iw,ih) \
			(*OL_FLATCLASS(w,item_dimensions))(w,ei,iw,ih)
#define _OlFlatItemHighlight(w,i,t)   (*OL_FLATCLASS(w,item_highlight))(w,i,t)
#define _OlFlatLayout(w,x,y)	      (*OL_FLATCLASS(w,layout))(w,x,y)
#define _OlFlatTraverseItems(w,i,d,t) (*OL_FLATCLASS(w,traverse_items))(w,i,d,t)

/*
 ************************************************************************	
 * Define Widget Class Part and Class Rec
 ************************************************************************	
 */

				/* Define new fields for the class part	*/

typedef struct _FlatClassPart {
    XtResourceList		item_resources;	/* sub-object resources	*/
    Cardinal			num_item_resources;/* number of item rsc*/
    OlFlatReqRscList		required_resources; /* manditory item rscs*/
    Cardinal			num_required_resources;
    XrmQuarkList		quarked_items;	/* quarked names	*/
    Cardinal			part_size;	/* this item's part size*/
    Cardinal			part_offset;	/* default part in widget*/
    Cardinal			part_in_rec_offset;/* part's record offset*/
    Cardinal			rec_size;	/* Full Record size	*/
    Boolean			transparent_bg;	/* inherit parent's bg ?*/
    OlFlatAnalyzeItemsProc	analyze_items;	/* check all items	*/
    OlFlatDrawItemProc		draw_item;	/* subclass's draw proc.*/
    OlFlatExpandItemProc	expand_item;	/* supplemental expansion*/
    OlFlatGetDrawInfoProc	get_draw_info;	/* returns drawing info	*/
    OlFlatGetIndexFunc		get_index;	/* gets an item's index	*/
    OlFlatItemAcceptFocusFunc	item_accept_focus; /* sets focus to items*/
    OlFlatItemActivateFunc	item_activate;	/* activates an item	*/
    OlFlatItemDimensionsProc	item_dimensions;/* item's width/height	*/
    OlFlatItemGetValuesProc	item_get_values;/* queries an item	*/
    OlFlatItemHighlightProc	item_highlight;	/* highlights an item	*/
    OlFlatItemInitializeProc	item_initialize;/* checking a new item	*/
    OlFlatItemSetValuesFunc	item_set_values;/* updates an item	*/
    OlFlatItemsTouchedProc	items_touched;	/* for new lists	*/
    WidgetClass			layout_class;	/* Flat to do layout	*/
    OlFlatLayoutProc		layout;		/* sub-object layout	*/
    OlFlatTraverseItemsFunc	traverse_items;	/* traverses items	*/
    XtPointer			reserved2;	/* GeometryManager	*/
    XtPointer			reserved1;	/* ChangedManaged	*/
    XtPointer			extension;	/* future extensions	*/
} FlatClassPart;

				/* Full class record declaration 	*/

typedef struct _FlatClassRec {
    CoreClassPart	core_class;
    PrimitiveClassPart	primitive_class;
    FlatClassPart	flat_class;
} FlatClassRec;

				/* External class record declaration	*/

extern FlatClassRec		flatClassRec;

#endif /* _OL_FLATP_H */
