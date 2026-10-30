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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/menu.e,v 2.9 92/02/06 00:16:28 william Exp $
 */
extern int	iconWindowShown;
extern int	iconWindowCreated;
extern int	showVersion;
extern int	activeMenu;
extern char	*mainMenuStr[];
extern GC	textMenuGC;
extern GC	rvPixmapMenuGC;

extern int	TextMenuLoop ();
extern int	PxMpMenuLoop ();
extern int	MainMenu ();
extern int	IsPrefix ();
extern void	RedrawTitleWindow ();
extern void	RedrawIconWindow ();
extern void	InitTitle ();
extern void	InitMenu ();
extern void	CleanUpMenu ();
extern void	SaveDrawWinInfo ();
extern void	UnIconify ();
extern void	IconEventHandler ();
extern void	Iconify ();
extern void	TitleEventHandler ();
