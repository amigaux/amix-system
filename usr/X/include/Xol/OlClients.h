/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)misc:include/Xol/OlClients.h	1.16"
#endif

#ifndef __OlClients_h__
#define __OlClients_h__


typedef struct {
	long	flags;
	enum {
	MENU_FULL = 0,
	MENU_LIMITED = 1,
	MENU_NONE = 2
	} menu_type;
	long	pushpin_initial_state;	/* as for WM_PUSHPIN_STATE */
} WMDecorationHints;

#define DELIMITER	'\n'

#define WMDecorationHeader	(1L<<0)		/* has title bar */
#define WMDecorationPushpin	(1L<<2)		/* has push pin */
#define WMDecorationCloseButton	(1L<<3)		/* has shine mark */
#define WMDecorationResizeable	(1L<<5)		/* has grow corners */


#define WMWindowNotBusy		0
#define WMWindowIsBusy		1


#define WMPushpinIsOut		0
#define WMPushpinIsIn		1


typedef struct {
	int		state;
	Window		icon;
} WMState;

#define OLWSM_STATE	(1L<<0)	/* olwsm is running */
#define OLWM_STATE	(1L<<1)	/* olwm is running */
#define OLFM_STATE	(1L<<2)	/* olfm is running */

typedef struct {
	unsigned long	state;
} OLManagerState;


typedef struct {
	int	min_width;
	int	min_height;
	int	max_width;
	int	max_height;
	int	width_inc;
	int	height_inc;
} WMIconSize;


#define MessageHint	(1L<<7)

#define ConfigureDenied	(1L<<0)		/* WM_CONFIGURE_DENIED */
#define WindowMoved	(1L<<1)		/* WM_WINDOW_MOVED */
#define BangMessage	(1L<<2)		/* BANG! */
#define FocusMessage	(1L<<3)		/* WM_TAKE_FOCUS */

/*
 * Following for WMState structure
 */

#define WithdrawnState	0
#define NormalState	1
#define IconicState	3


typedef struct OlWinAttr {
#ifndef	OpenWindowsEnvironment
	unsigned long	flags;
#endif
	Atom		win_type;
	long		menu_type;
	long		pin_state;
#ifndef	OpenWindowsEnvironment
	long		cancel;
#endif
} OLWinAttr;


typedef struct OlWinColors {
	unsigned long	flags;
	unsigned long	fg_red;
	unsigned long	fg_green;
	unsigned long	fg_blue;
	unsigned long	bg_red;
	unsigned long	bg_green;
	unsigned long	bg_blue;
	unsigned long	bd_red;
	unsigned long	bd_green;
	unsigned long	bd_blue;
} OLWinColors;

/*
 * values for _OL_WIN_ATTR flags
 */

#define	_OL_WA_WIN_TYPE		(1<<0)
#define	_OL_WA_MENU_TYPE	(1<<1)
#define	_OL_WA_PIN_STATE	(1<<2)
#define	_OL_WA_CANCEL		(1<<3)

/*
 * values for _OL_WIN_COLORS flags
 */

#define	_OL_WC_FOREGROUND	(1<<0)
#define	_OL_WC_BACKGROUND	(1<<1)
#define	_OL_WC_BORDER		(1<<2)

/*
 * for compatiblity with earlier software
 */

#define WM_PUSHPIN_STATE	_OL_PIN_STATE
#define WM_WINDOW_BUSY		_OL_WIN_BUSY
#define MENU_DISMISS_ONLY	MENU_NONE


extern Atom	WM_DISMISS;
extern Atom	WM_DECORATION_HINTS;
extern Atom	WM_WINDOW_MOVED;
extern Atom	WM_TAKE_FOCUS;
extern Atom	WM_DELETE_WINDOW;
extern Atom	BANG;
extern Atom	WM_SAVE_YOURSELF;
extern Atom	WM_STATE;
extern Atom	WM_CHANGE_STATE;
extern Atom	WM_PROTOCOLS;

extern Atom	_OL_COPY;
extern Atom	_OL_CUT;
extern Atom	_OL_HELP_KEY;
extern Atom	_OL_WIN_ATTR;
extern Atom	_OL_WT_BASE;
extern Atom	_OL_WT_CMD;
extern Atom	_OL_WT_PROP;
extern Atom	_OL_WT_HELP;
extern Atom	_OL_WT_NOTICE;
extern Atom	_OL_WT_OTHER;
extern Atom	_OL_DECOR_ADD;
extern Atom	_OL_DECOR_DEL;
extern Atom	_OL_DECOR_CLOSE;
extern Atom	_OL_DECOR_RESIZE;
extern Atom	_OL_DECOR_HEADER;
extern Atom	_OL_DECOR_PIN;
extern Atom	_OL_WIN_COLORS;
extern Atom	_OL_PIN_STATE;
extern Atom	_OL_WIN_BUSY;
extern Atom	_OL_MENU_FULL;
extern Atom	_OL_MENU_LIMITED;
extern Atom	_OL_NONE;

extern void	InitializeOpenLook();
extern void	SetWMDecorationHints();
extern void	SetWMState();
extern void	SetWMWindowBusy();
extern Status	GetHelpKeyMessage();
extern void	SendProtocolMessage();
extern void	SetWMPushpinState();
extern int	GetOLWinAttr();
extern int	GetOLWinColors();

extern void     EnqueueCharProperty();
extern char *   GetCharProperty();
extern Atom *   GetAtomList();

#endif /* __OlClients_h__ */
