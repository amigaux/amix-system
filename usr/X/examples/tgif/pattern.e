/*
 * Author:      William Chia-Wei Cheng (william@cs.ucla.edu)
 *
 * Copyright (C) 1989, 1990, 1991, William Cheng.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose is hereby granted by the Author without
 * fee, provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of the Author not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The Author makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/pattern.e,v 2.6 91/06/28 15:28:30 william Exp $
 */

extern int	objFill;
extern int	lineStyle;
extern int	lineWidth;
extern int	penPat;
extern int	curSpline;
extern int	curDash;

extern void	ChangeAllSelFill ();
extern void	ChangeAllSelLineStyle ();
extern void	ChangeAllSelLineType ();
extern void	ChangeAllSelLineWidth ();
extern void	ChangeAllSelDashes ();
extern void	ChangeAllSelPen ();
extern void	ToggleAllSelLineType ();
extern void	ModeMenu ();
extern void	FillMenu ();
extern void	LineWidthMenu ();
extern void	LineStyleMenu ();
extern void	LineTypeMenu ();
extern void	LineDashMenu ();
extern void	PenMenu ();
