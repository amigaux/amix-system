/*
 * Author:	Kou1 Ma2da (matsuda@ccs.mt.nec.co.jp)
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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/mainmenu.e,v 1.11 92/02/06 00:16:25 william Exp $
 */
extern int	pinnedMainMenu;
extern int	mainMenuPinDistance;
extern int	mainMenuX;
extern int	mainMenuY;
extern int	mainMenuW;
extern int	mainMenuH;
extern Window	mainMenuWindow;

extern int	numStacking;
extern Window	* stackingWins;

extern void	InitMainMenu ();
extern void	CleanUpMainMenu ();
extern void	SaveMainMenuWindow ();
extern void	MoveMainMenuWindow ();
extern void	RedrawMainMenuWindow ();
extern int	MainMenuEventHandler ();
extern void	SaveStackingOrder ();
extern void	MoveSubMenuWindow ();
extern void	RealizeMenuSubWindow ();
extern void	UpdateSubMenu ();
