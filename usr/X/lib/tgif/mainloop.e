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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/mainloop.e,v 2.10 92/02/06 00:16:23 william Exp $
 */

extern int	cmdLineBW;
extern int	cmdLineRV;
extern char	* cmdLineForeground;
extern char	* cmdLineBackground;
extern char	* cmdLineBorder;
extern int	geometrySpecified;
extern int	exitNormally;
extern char	geometrySpec[];
extern char	* displayName;
extern char	initMsg1[];
extern char	initMsg2[];
extern int	numExtraWins;

extern struct WinInfoRec	* extraWinInfo;

extern int	AddExtraWinInfo ();
extern void	DeallocStrings ();
extern void	CleanUp ();
extern void	ExposeEventHandler ();
extern void	MainLoop ();
extern void	Animate ();
extern void	UpdAttrVal ();
