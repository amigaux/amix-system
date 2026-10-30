/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)textedit:TextEditP.h	1.9"
#endif

/*
 * TextEditP.h
 *
 */

#ifndef _TextEditP_h
#define _TextEditP_h

typedef struct {int foo;} TextEditClassPart;

#include <Xol/TextEdit.h>
#include <Xol/PrimitiveP.h>

typedef struct _TextEditClassRec 
   {
   CoreClassPart        core_class;
   PrimitiveClassPart   primitive_class;
   TextEditClassPart    textedit_class;
   } TextEditClassRec;

typedef enum 
   {
   OlselectNull, OlselectPosition, OlselectChar, OlselectWord,
   OlselectLine, OlselectParagraph, OlselectAll
   } OlSelectType;

typedef enum
   {
   OlCopyDrag, OlMoveDrag
   } OlDragMode;

typedef enum
   {
   OlCursorOn, OlCursorOff, OlCursorBlinkOff, OlCursorBlinkOn
   } OlCursorState;

typedef struct _TextCursor
   {
   int             limit;
   int             width;
   int             height;
   int             xoffset;
   int             baseline;
   unsigned char * inbits;
   unsigned char * outbits;
   } TextCursor;

typedef Bufferof(int) WrapLocation;
typedef Bufferof(WrapLocation *) WrapTable;

typedef struct _DisplayTable
   {
   char *       p;
   int          used;
   int          size;
   char         flag;
   TextLocation wraploc;
   } DisplayTable;

typedef struct
   {
   TextPosition    displayPosition;
   TextPosition    cursorPosition;
   TextPosition    selectStart;
   TextPosition    selectEnd;
   TextPosition    anchor;
   Dimension       leftMargin;
   Dimension       rightMargin;
   Dimension       topMargin;
   Dimension       bottomMargin;
   OlEditMode      editMode;
   OlWrapMode      wrapMode;
   OlSourceType    sourceType;
   char *          source;
   XtCallbackList  motionVerification;
   XtCallbackList  modifyVerification;
   XtCallbackList  postModifyNotification;
   XtCallbackList  margin;
   XtCallbackList  buttons;
   XtCallbackList  keys;
   long            blinkRate;
   Boolean         updateState;
   Boolean         shouldBlink;
   Boolean	   insertTab;
   Boolean	   insertReturn;
   TabTable        tabs;
   OlRegisterFocusFunc register_focus;

   /* not in resource table */

   TextLocation    displayLocation;
   TextLocation    cursorLocation;

   /* private information */

   char *          clip_contents;
   DisplayTable *  DT;
   int             DTsize;
   XtIntervalId    blink_timer;
   int             lineCount;
   int             linesVisible;
   int             charsVisible;
   int             lineHeight;
   int             charWidth;
   int             xOffset;
   int             maxX;
   Widget          vsb;
   Widget          hsb;
   TextBuffer *    textBuffer;
   GC              gc;
   GC              invgc;
   GC              insgc;
   int             save_offset;
   char            selectMode;
   unsigned char   dyn_flags;
   WrapTable *     wrapTable;
   unsigned int    mask;
   unsigned long   dynamic;
   Pixmap          CursorIn;
   Pixmap          CursorOut;
   TextCursor *    CursorP;
   OlCursorState   cursor_state;
   Position        cursor_x;
   Position        cursor_y;
   char            need_vsb;
   char            need_hsb;
   Dimension       prev_width;
   Dimension       prev_height;
   } TextEditPart;

typedef struct _TextEditRec
   {
   CorePart           core;
   PrimitivePart      primitive;
   TextEditPart       textedit;
   } TextEditRec;

#define ASCENT(widget)        (widget-> primitive.font-> ascent)
#define DESCENT(widget)       (widget-> primitive.font-> descent)
#define FONTHT(widget)        (ASCENT(widget) + DESCENT(widget))
#define FONTWID(widget)       (widget-> primitive.font-> max_bounds.width)
#define PAGE_T_GAP(widget)    ((int)(widget-> textedit.topMargin))
#define PAGE_B_GAP(widget)    ((int)(widget-> textedit.bottomMargin))
#define PAGE_R_GAP(widget)    ((int)(widget-> textedit.rightMargin))
#define PAGE_L_GAP(widget)    ((int)(widget-> textedit.leftMargin))
#define PAGE_T_MARGIN(widget) ((int)(widget-> textedit.topMargin))
#define PAGE_L_MARGIN(widget) ((int)(widget-> textedit.leftMargin))
#define PAGE_R_MARGIN(widget) ((int)(widget-> core.width) - PAGE_R_GAP(widget))
#define PAGE_B_MARGIN(widget) ((int)(widget-> core.height) - PAGE_B_GAP(widget))
#define PAGEHT(widget)        (PAGE_B_MARGIN(widget) - PAGE_T_MARGIN(widget))
#define PAGEWID(widget)       (PAGE_R_MARGIN(widget) - PAGE_L_MARGIN(widget))
#define PAGE_LINE_HT(widget)  (PAGEHT(widget) / FONTHT(widget))
#define LINES_VISIBLE(widget) (widget-> textedit.linesVisible)
#define HGAP(ctx)             (0)
#define HORIZONTAL_SHIFT(ctx) (8 * ENSPACE(ctx-> primitive.font))
#define ENSPACE(fs)           (_CharWidth('n', fs,                  \
                                            fs-> per_char,          \
                                            fs-> min_char_or_byte2, \
                                            fs-> max_char_or_byte2, \
                                            fs-> max_bounds.width))

#define PRINTLOC(L,T)   \
(void)fprintf(stderr,"%s:(%5d, %5d)\n", T, L.line, L.offset)

#define PRINTPOS(P,T)   \
(void)fprintf(stderr,"%s:(%5d)\n", T, P)

#define PRINTRECT(rect, T) \
(void)fprintf(stderr,"%s: %d,%d %d,%d\n",T,rect.x,rect.y,rect.width,rect.height)

#define PRINTSELECT(text, T) \
(void)fprintf(stderr,"%s: %d to %d\n",T,text-> selectStart, text-> selectEnd)

/* dynamics resources bit masks */
#define OL_B_TEXTEDIT_BG		(1 << 0)
#define OL_B_TEXTEDIT_FONTCOLOR		(1 << 1)

extern TextEditClassRec textEditClassRec;

#endif
