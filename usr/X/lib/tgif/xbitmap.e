/*
 * Author:	William Chia-Wei Cheng (william@cs.ucla.edu)
 *
 * Copyright (C) 1990, 1991, 1992, William Cheng.
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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/xbitmap.e,v 2.22 92/02/06 00:17:19 william Exp $
 */

extern GC	xbmGC;
extern int	askForXBmSpec;
extern int	stripEPSComments;
extern Pixmap	dummyBitmap;

extern void	InitXBm ();
extern void	CleanUpXBm ();
extern void	CalcTransform ();
extern void	MakeCachedBitmap ();
extern int	ExtractBitmap ();
extern void	InvertXBitmaps ();
extern void	ParseCutSpec ();
extern void	CutXBitmap ();
extern void	DumpXBitmapFile ();
extern void	DumpXBmObj ();
extern int	NeedsToCacheXBmObj ();
extern void	DrawXBmObj ();
extern struct ObjRec	* CreateXBmObj ();
extern void	SaveXBmObj ();
extern void	ReadXBmObj ();
extern void	FreeXBmObj ();
