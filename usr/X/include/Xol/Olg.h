/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)olg:Olg.h	1.6"
#endif

#ifndef _OLG_H_
#define _OLG_H_

typedef struct {
    unsigned char	width;	/* width of image */
    unsigned char	height;	/* height of image */
    unsigned char	*pData;	/* pointer to coordinate data */
} _OlgDesc;

typedef struct _OlgDevice {
    Screen	*scr;		/* display structure ptr */

    GC		whiteGC;	/* white pixel of screen */
    GC		blackGC;	/* black pixel of screen */
    GC		grayGC;		/* gray pixel (solid) */
    GC		busyGC;		/* black with 15% stipple */
    GC		lightGrayGC;	/* white with 50% stipple */
    GC		dimGrayGC;	/* black with 50% stipple */
    GC		scratchGC;	/* a GC for others to play with */

    unsigned	horizontalStroke;	/* width of horizontal stroke */
    unsigned	verticalStroke;		/* width of vertical stroke */

    char	*descPath;		/* base name of description files */

    Pixmap	busyStipple;
    Pixmap	inactiveStipple;

    /* Push Pin pixmaps */
    Pixmap	pushpin2D;
    Dimension	widthPushpin2D;
    Dimension	heightPushpin2D;

    Pixmap	pushpin3D;
    Dimension	widthPushpin3D;
    Dimension	heightPushpin3D;

    /* check box pixmaps */
    Pixmap	checks;
    Dimension	widthChecks;
    Dimension	heightChecks;
    Dimension	widthCheckBox;
    Dimension	heightCheckBox;

    /* miscellaneous arrows */
    _OlgDesc	arrowUp;
    _OlgDesc	arrowDown;
    _OlgDesc	arrowLeft;
    _OlgDesc	arrowRight;
    _OlgDesc	arrowText;

    /* Corner for oblong buttons */
    _OlgDesc	oblongB2UL;	/* 2-D corners */
    _OlgDesc	oblongB2UR;
    _OlgDesc	oblongB2LL;
    _OlgDesc	oblongB2LR;
    _OlgDesc	oblongB3UL;	/* 3-D corners */
    _OlgDesc	oblongB3UR;
    _OlgDesc	oblongB3LL;
    _OlgDesc	oblongB3LR;
    _OlgDesc	oblongDefUL;	/* default ring (2- and 3-D) */
    _OlgDesc	oblongDefUR;
    _OlgDesc	oblongDefLL;
    _OlgDesc	oblongDefLR;

    /* Small-radius rounded Corners for more rectangular things like
     * the abbreviated menu button
     */
    _OlgDesc	rect2UL;	/* 2-D */
    _OlgDesc	rect2UR;
    _OlgDesc	rect2LL;
    _OlgDesc	rect2LR;
    _OlgDesc	rect3UL;	/* 3-D */
    _OlgDesc	rect3UR;
    _OlgDesc	rect3LL;
    _OlgDesc	rect3LR;

    _OlgDesc	sliderVUL;	/* vertical slider */
    _OlgDesc	sliderVUR;
    _OlgDesc	sliderVLL;
    _OlgDesc	sliderVLR;
    _OlgDesc	sliderHUL;	/* horizontal slider */
    _OlgDesc	sliderHUR;
    _OlgDesc	sliderHLL;
    _OlgDesc	sliderHLR;

    /* metrics used to determine the sizes of various elements */

    char	lblOrigY;	/* amount from top of oblong default ring
				 * to offset label.
				 */
    char	lblCornerY;	/* amount from bottom of oblong default ring
				 * to offset label.
				 */

    char	rect2OrigX;	/* offsets of interior of small ring (2-D) */
    char	rect2OrigY;
    char	rect2CornerX;
    char	rect2CornerY;

    char	rect3OrigX;	/* offsets of interior of small ring (3-D) */
    char	rect3OrigY;
    char	rect3CornerX;
    char	rect3CornerY;

    unsigned char	downMenuMWidth;	/* down menu mark dimensions */
    unsigned char	downMenuMHeight;
    unsigned char	rightMenuMWidth;	/* right menu mark */
    unsigned char	rightMenuMHeight;
    unsigned char	menuMPad;	/* space between menu mark and lbl */

    struct _OlgDevice	*next;	/* ptr to next device struct */
    char	scale;		/* rendering size */
} _OlgDevice;

typedef union {
    Pixel	pixel;
    Pixmap	pixmap;
} OlgBG;

typedef struct _OlgAttrs {
    int		refCnt;		/* reference count */
    struct _OlgAttrs	*next;	/* pointer to next struct in list */

    Pixel	fg;		/* foreground pixel */
    OlgBG	bg;		/* background pixel or Pixmap */
    GC		bg0;		/* bg1, only lighter */
    GC		bg1;		/* User specified background */
    GC		bg2;		/* bg1, only a little darker */
    GC		bg3;		/* bg1, only much darker */
    _OlgDevice	*pDev;		/* pointer to device specific data */
    unsigned char	flags;
} OlgAttrs;

typedef struct	{
    char	*label;
    GC		normalGC;
    GC		inverseGC;
    XFontStruct	*font;
    char	*accelerator;
    char	mnemonic;
    unsigned char	justification;
    unsigned char	flags;
} OlgTextLbl;

typedef struct {
    union {
	Pixmap	pixmap;
	XImage	*image;
    } label;
    GC		normalGC;
    Pixel	stippleColor;
    unsigned char	justification;
    unsigned char	type;
    unsigned char	flags;
} OlgPixmapLbl;    

extern	unsigned	_olgIs3d;

/* attributes defines */
#define OLG_BGPIXMAP	1
#define OLG_ALLOCBG0	2
#define OLG_ALLOCBG1	4
#define OLG_ALLOCBG2	8
#define OLG_ALLOCBG3	16

/* Scrollbar defines */
#define SB_POSITION	1
#define SB_BEGIN_ANCHOR	2
#define SB_END_ANCHOR	4
#define SB_PREV_ARROW	8
#define SB_NEXT_ARROW	16
#define SB_DRAG		32

/* Slider defines */
#define SL_POSITION	1
#define SL_DRAG		2
#define SL_BEGIN_ANCHOR	4
#define SL_END_ANCHOR	8

/* Oblong button defines */
#define OB_SELECTED	1
#define OB_DEFAULT	2
#define OB_BUSY		4
#define OB_INSENSITIVE	8
#define OB_MENUITEM	16
#define OB_MENUMARK	32
#define OB_MENU_R	OB_MENUMARK
#define OB_MENU_D	(OB_MENUMARK | 64)

/* Rectangular button defines */
#define RB_SELECTED	1
#define RB_DEFAULT	2
#define RB_DIM		4
#define RB_INSENSITIVE	8
#define RB_NOFRAME	16

/* Label defines */
#define TL_LEFT_JUSTIFY		0
#define TL_CENTER_JUSTIFY	1
#define TL_RIGHT_JUSTIFY 	2
#define TL_SELECTED		1
#define TL_POPUP		2

/* Image label defines */
#define PL_IMAGE	0
#define PL_PIXMAP	1
#define PL_BITMAP	2
#define PL_TILED	1
#define PL_INSENSITIVE	2

/* Abbreviated menu button defines */
#define AM_NORMAL	0
#define AM_SELECTED	1

/* Menu mark defines */
#define MM_DOWN		1
#define MM_CENTER	2
#define MM_INVERT	4

/* Pushpin defines */
#define PP_OUT		0
#define PP_IN		1
#define PP_DEFAULT	2

/* Checkbox defines */
#define	CB_CHECKED	1
#define CB_DIM		2

/* Rounded box defines */
#define RB_UP		1
#define RB_OMIT_LCAP	2
#define RB_OMIT_BCAP	4
#define RB_MONO		8

/* Filled Rounded box defines */
#define FB_UP		1
#define FB_BUSY		2
#define FB_OMIT_RCAP	4
#define FB_OMIT_TCAP	8
#define FB_OMIT_LCAP	16
#define FB_OMIT_BCAP	32

/* Bitmap type defines */
#define OLG_PUSHPIN_2D	0
#define OLG_PUSHPIN_3D	1
#define OLG_CHECKS	2

/* Miscellaneous Macros */
#define OlgGetBg1GC(p)	((p) ? (p)->bg1 : (GC) 0)
#define OlgGetBg2GC(p)	((p) ? (p)->bg2 : (GC) 0)
#define OlgGetBg3GC(p)	((p) ? (p)->bg3 : (GC) 0)
#define OlgGetFgGC(p)	((p) ? (p)->bg2 : (GC) 0)
#define OlgGetBrightGC(p)	((p) ? (p)->bg0 : (GC) 0)
#define OlgGetDarkGC(p)	((p) ? (p)->pDev->blackGC : (GC) 0)
#define OlgGetScratchGC(p)	((p) ? (p)->pDev->scratchGC : (GC) 0)
#define OlgGetHorizontalStroke(p)	((p) ? (p)->pDev->horizontalStroke:0)
#define OlgGetVerticalStroke(p)	((p) ? (p)->pDev->verticalStroke : 0)
#define OlgGetInactiveStipple(p)	((p) ? (p)->pDev->inactiveStipple : (Pixmap) 0)
#define OlgGetScreen(p)	((p) ? (p)->pDev->scr : (Screen *) 0)

#define OlgIs3d()	_olgIs3d

extern OlgAttrs *OlgCreateAttrs ();
extern void OlgDestroyAttrs ();
extern void OlgSetStyle3D ();

extern void OlgDrawScrollbar ();
extern void OlgUpdateScrollbar ();
extern void OlgDrawAnchor ();
extern void OlgDrawSlider ();
extern void OlgUpdateSlider ();
extern void OlgDrawOblongButton ();
extern unsigned OlgDrawMenuMark ();
extern void OlgDrawAbbrevMenuB ();
extern void OlgDrawPushPin ();
extern void OlgDrawChiseledBox ();
extern void OlgDrawBox ();
extern void OlgDrawRBox ();
extern void OlgDrawFilledRBox ();

extern void OlgSizeScrollbarAnchor ();
extern void OlgSizeScrollbarElevator ();
extern void OlgSizeSlider ();
extern void OlgSizeSliderAnchor ();
extern void OlgSizeSliderElevator ();
extern void OlgSizeOblongButton ();
extern void OlgSizeRectButton ();
extern void OlgSizeAbbrevMenuB ();
extern void OlgSizePushPin ();

extern void OlgDrawTextLabel ();
extern void OlgSizeTextLabel ();
extern void OlgDrawPixmapLabel ();
extern void OlgSizePixmapLabel ();

extern _OlgDevice *_OlgGetDeviceData ();
extern void _OlgGetBitmaps ();

#endif /* _OLG_H_ */
