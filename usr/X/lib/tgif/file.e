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
 * @(#)$Header: /amnt/kona/tangram/u/william/X11/TGIF2/RCS/file.e,v 2.26 92/02/06 00:16:14 william Exp $
 */

extern int	PRTGIF;
extern char	curFileName[];
extern int	curFileDefined;
extern int	fileVersion;
extern int	importingFile;
extern int	psDotsPerInch;
extern int	printMag;
extern int	saveTmpOnReturn;
extern int	warpToWinCenter;

extern char	* psXOffStr[];
extern float	psXOff[];
extern char	* psYOffStr[];
extern float	psYOff[];
extern float	psPageWidthInInch[];
extern float	psPageHeightInInch[];
extern char	printCommand[];
extern char	outputDir[];
extern char	* fileMenuStr[];

extern void	ClearFileInfo ();
extern int	OkayToCreateFile ();
extern void	Save ();
extern int	SaveTmpFile ();
extern void	SaveNewFile ();
extern void	SaveFile ();
extern char	* ParseStr ();
extern char	* FindChar ();
extern int	ReadObj ();
extern void	ChangeDomain ();
extern void	AdjForOldVersion ();
extern void	ImportFile ();
extern void	ImportXBitmapFile ();
extern void	ImportXPixmapFile ();
extern void	ImportEPSFile ();
extern int	LoadFile ();
extern void	DumpPatFill ();
extern void	DumpSymOutline ();
extern void	DumpBBox ();
extern void	ModifyOutputFileName ();
extern void	Dump ();
extern void	PrintWithCommand ();
extern void	SetPrintReduction ();
extern void	NewProc ();
extern void	OpenProc ();
extern int	QuitProc ();
extern int	SolveProc ();
extern int	SimulateProc ();
extern int	AnimateProc ();
extern int	ProbeProc ();
extern int	EscapeProc ();
extern int	FileSubMenu ();
extern int	FileMenu ();
extern void	CleanUpFiles ();
