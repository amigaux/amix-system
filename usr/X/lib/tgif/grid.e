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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/grid.e,v 2.19 92/02/06 00:16:20 william Exp $
 */

extern int	gridSystem;
extern int	gridOn;
extern int	xyEnglishGrid;
extern int	xyMetricGrid;
extern int	pageStyle;
extern int	whereToPrint;
extern int	moveMode;
extern int	gridShown;
extern int	mapShown;
extern char	* layoutMenuStr[];

extern void	MyHDotLine ();
extern void	MyVDotLine ();
extern void	RedrawGridLines ();
extern void	DrawGridLines ();
extern void	ToggleGridSystem ();
extern void	IncGrid ();
extern void	DecGrid ();
extern void	ToggleGridShown ();
extern void	ToggleSnapOn ();
extern void	ToggleWhereToPrint ();
extern void	ToggleColorPostScript ();
extern void	ToggleMoveMode ();
extern void	ToggleMapShown ();
extern void	DefaultZoom ();
extern void	ZoomIn ();
extern void	ZoomWayOut ();
extern void	ZoomOut ();
extern void	PreciseZoom ();
extern int	UpdPageStyle ();
extern void	ChangePageStyle ();
extern void	SetUpLayoutMenuStr ();
extern void	LayoutSubMenu ();
extern void	LayoutMenu ();
extern void	GridXY ();
extern void	CleanUpGrids ();
