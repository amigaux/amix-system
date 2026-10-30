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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/poly.e,v 2.7 91/09/11 12:25:18 william Exp $
 */

extern int	polyDrawn;

extern short	origWidthOfLine[];
extern short	origArrowHeadH[];
extern short	origArrowHeadW[];

extern short	*curWidthOfLine;
extern short	*curArrowHeadH;
extern short	*curArrowHeadW;

extern XPoint	* MakePolyVertex ();
extern void	CalcPolyBBox ();
/* extern void	RotatePolyObj (); */
extern void	UpdPolyBBox ();
extern void	MyLine ();
extern void	DrawPoly ();
extern void	DumpPolyObj ();
extern void	DrawPolyObj ();
extern void	SavePolyObj ();
extern void	ReadPolyObj ();
extern void	FreePolyObj ();
