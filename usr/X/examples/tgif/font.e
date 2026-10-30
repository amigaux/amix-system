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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/font.e,v 2.10 91/08/02 14:54:14 william Exp $
 */

extern XFontStruct	* canvasFontPtr;
extern int	canvasFontHeight;
extern int	canvasFontAsc;
extern int	canvasFontDes;

extern int	canvasFontIndex;

extern XFontStruct	* rulerFontPtr;
extern int	rulerFontWidth;
extern int	rulerFontHeight;
extern int	rulerFontAsc;
extern int	rulerFontDes;

extern XFontStruct	* defaultFontPtr;
extern int	defaultFontWidth;
extern int	defaultFontHeight;
extern int	defaultFontAsc;
extern int	defaultFontDes;

extern int	* pointSize;
extern int	pointSize75[];
extern int	pointSize100[];
extern int	curFontDPI;
extern int	curFont;
extern int	curSize;
extern int	curStyle;
extern int	curRotate;

extern struct XFontStruct	* GetFontStruct ();
extern int	FontIndex ();
extern int	ValidCharCode ();
extern char	* CharCodeTranslate ();
extern void	PrepareEightBitFontInfo ();
extern void	GetPSFontStr ();
extern void	CurFontMsg ();
extern int	NeedEncode ();
extern void	DumpEightBitFontInfo ();
extern void	SetCanvasFont ();
extern void	InitFonts ();
extern void	ChangeFontJust ();
extern void	ChangeFontStyle ();
extern void	StyleMenu ();
extern void	ChangeFontSize ();
extern int	GetSizeMenuIndex ();
extern int	ParseSizeMenuIndex ();
extern void	SizeMenu ();
extern void	ChangeFont ();
extern void	FontMenu ();
extern void	ChangeVSpace ();
extern void	SaveCurFont ();
extern void	RestoreCurFont ();
extern void	PushCurFont ();
extern void	PopCurFont ();
extern void	CleanUpFonts ();
