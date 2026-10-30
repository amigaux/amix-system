/*
 * Author:      William Chia-Wei Cheng (william@cs.ucla.edu)
 *
 * Copyright (C) 1989, 1990, 1991, 1992, William Cheng.
 * 
 * Permission limited to the use, copy, modify, and distribute this software
 * and its documentation for any purpose is hereby granted by the Author without
 * fee, provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of the Author not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The Author makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.  All other
 * rights are reserved by the Author.
 *
 * THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/raster.e,v 2.21 92/02/06 00:16:46 william Exp $
 */

extern GC	rasterGC;

extern Pixmap	patPixmap[];
extern GC	patGC;

extern unsigned long	xorOne;
extern unsigned long	xorZero;

extern GC	drawGC;
extern GC	defaultGC;
extern GC	revDefaultGC;
extern GC	revGrayGC;

extern Pixmap	choicePixmap[];
extern Pixmap	patPixmap[];
extern Pixmap	* lineWidthPixmap;
extern Pixmap	lineTypePixmap[];
extern Pixmap	dashPixmap[];
extern Pixmap	lineStylePixmap[];
extern Pixmap	justPixmap[];
extern Pixmap	alignHoriPixmap[];
extern Pixmap	alignVertPixmap[];
extern Pixmap	filePixmap;
extern Pixmap	landscapePixmap;
extern Pixmap	specialPixmap;
extern Pixmap	vspacePixmap;
extern Pixmap	rcbRadiusPixmap;
extern Pixmap	constMovePixmap;
extern Pixmap	unConstMovePixmap;
extern Pixmap	editPixmap;

extern Pixmap	rotatePixmap[];
extern Pixmap	whereToPrintPixmap[];
extern Pixmap	* shortLineWidthPixmap;
extern Pixmap	shortLineStylePixmap[];
extern Pixmap	shortLineTypePixmap[];
extern Pixmap	shortDashPixmap[];

extern int	maxLineWidths;

extern int	pat_w[], pat_h[];
extern int	dashListLength[];
extern char	*dashList[];

extern void	InitPattern ();
extern void	RedrawChoiceWindow ();
extern void	CleanUpRasters ();
extern int	UpgradePenFill ();
