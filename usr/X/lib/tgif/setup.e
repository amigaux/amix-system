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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/setup.e,v 2.11 92/02/06 00:16:58 william Exp $
 */

extern unsigned int	mainWinW;
extern unsigned int	mainWinH;
extern int	vSBarH;
extern int	hSBarW;
extern int	scrollBarW;
extern int	rulerW;
extern int	brdrW;
extern int	msgWindowW;
extern int	msgWindowH;
extern int	choiceImageW;
extern int	choiceImageH;
extern int	choiceWindowW;
extern int	choiceWindowH;
extern int	menuImageW;
extern int	menuImageH;
extern int	titleWindowW;
extern int	titleWindowH;
extern int	iconWindowW;
extern int	iconWindowH;

extern Display		* mainDisplay;
extern Colormap		mainColormap;
extern unsigned int	mainDepth;
extern int		mainScreen;
extern Visual		* mainVisual;

extern Window	rootWindow;
extern Window	mainWindow;
extern Window	drawWindow;
extern Window	choiceWindow;
extern Window	titleWindow;
extern Window	msgWindow;
extern Window	vSBarWindow;
extern Window	hSBarWindow;
extern Window	vRuleWindow;
extern Window	hRuleWindow;
extern Window	iconWindow;
extern Window	iconBaseWindow;

extern int	paperWidth;
extern int	paperHeight;
extern int	drawOrigX;
extern int	drawOrigY;
extern int	drawWinW;
extern int	drawWinH;

extern int	zoomScale;
extern int	zoomedIn;

extern struct BBRec	drawWinBBox;

extern int	colorDisplay;
extern int	fileModified;
extern int	objId;

extern void	UpdDrawWinWH ();
extern void	UpdDrawWinBBox ();
extern int	mainWinEventHandler ();
extern void	Setup ();
extern int	TieLooseEnds ();
extern void	MakeQuiescent ();
extern void	SetFileModified ();

extern int	myBgPixel;
extern int	myFgPixel;
extern int	myBorderPixel;
extern int	reverseVideo;

extern char	drawPath[];
extern char	bootDir[];
extern char	homeDir[];

extern int	symPathNumEntries;
extern char	* * symPath;

extern int	initDrawWinW;
extern int	initDrawWinH;

extern short	handleSize;
