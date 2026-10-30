/*
 * Author:	William Chia-Wei Cheng (william@cs.ucla.edu)
 *
 * Copyright (C) 1990, 1991, William Cheng.
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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/xpixmap.e,v 2.10 91/08/05 09:05:23 william Exp $
 */

extern GC	xpmGC;

extern void	InitXPm ();
extern void	CleanUpXPm ();
extern void	BuildXPmBucket ();
extern void	BuildXPmBucket ();
extern int	XPmLookUp ();
extern void	MakeCachedPixmap ();
extern void	FlipXPmHorizontal ();
extern void	FlipXPmVertical ();
extern void	RotateXPmClockWise ();
extern void	RotateXPmCounter ();
extern void	CutXPixmap ();
extern void	DumpXPmObj ();
extern void	DrawXPmObj ();
extern struct ObjRec	* CreateXPmObj ();
extern void	SaveXPmObj ();
extern int	QuickFindColorIndex ();
extern int	MyReadPixmapFile ();
extern void	ReadXPmObj ();
extern void	FreeXPmObj ();
