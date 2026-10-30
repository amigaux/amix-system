/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)textedit:TextEdit.h	1.4"
#endif

/*
 * TextEdit.h
 *
 */

#ifndef _TextEdit_h
#define _TextEdit_h

#include <Xol/buffutil.h>
#include <Xol/textbuff.h>
#include <Xol/Dynamic.h>

typedef Dimension * TabTable;

typedef struct
   {
   Boolean           ok;
   TextPosition      current_cursor;
   TextPosition      new_cursor;
   TextPosition      select_start;
   TextPosition      select_end;
   } OlTextMotionCallData, *OlTextMotionCallDataPointer;

typedef struct
   {
   Boolean           ok;
   TextPosition      current_cursor;
   TextPosition      select_start;
   TextPosition      select_end;
   TextPosition      new_cursor;
   TextPosition      new_select_start;
   TextPosition      new_select_end;
   char *            text;
   int               text_length;
   } OlTextModifyCallData, *OlTextModifyCallDataPointer;

typedef struct
   {
   Boolean           requestor;
   TextPosition      new_cursor;
   TextPosition      new_select_start;
   TextPosition      new_select_end;
   char *            inserted;
   char *            deleted;
   TextLocation      delete_start;
   TextLocation      delete_end;
   TextLocation      insert_start;
   TextLocation      insert_end;
   TextPosition      cursor_position;
   } OlTextPostModifyCallData, *OlTextPostModifyCallDataPointer;

typedef enum 
   { OL_MARGIN_EXPOSED, OL_MARGIN_CALCULATED } OlTextMarginHint;

typedef struct
   {
   OlTextMarginHint  hint;
   XRectangle *      rect;
   } OlTextMarginCallData, *OlTextMarginCallDataPointer;

#undef OL_TEXT_READ
#undef OL_TEXT_EDIT
typedef enum
   { OL_TEXT_EDIT=66, OL_TEXT_READ=67 } OlEditMode;

#undef OL_WRAP_OFF
#undef OL_WRAP_ANY
#undef OL_WRAP_WHITE_SPACE
typedef enum
   { OL_WRAP_OFF=74, OL_WRAP_ANY=75, OL_WRAP_WHITE_SPACE=76 } OlWrapMode;

#undef OL_STRING_SOURCE
#undef OL_DISK_SOURCE
#undef OL_TEXT_BUFFER_SOURCE
typedef enum
   { OL_DISK_SOURCE=15, OL_STRING_SOURCE=64, OL_TEXT_BUFFER_SOURCE=99 } OlSourceType;

typedef struct _TextEditClassRec *TextEditWidgetClass;
typedef struct _TextEditRec      *TextEditWidget;

extern Boolean      OlTextEditClearBuffer();
extern Boolean      OlTextEditCopyBuffer();
extern Boolean      OlTextEditCopySelection();
extern Boolean      OlTextEditReadSubString();
extern Boolean      OlTextEditGetLastPosition();
extern Boolean      OlTextEditGetCursorPosition();
extern Boolean      OlTextEditInsert();
extern Boolean      OlTextEditSetCursorPosition();
extern Boolean      OlTextEditRedraw();
extern void         OlTextEditResize();
extern Boolean      OlTextEditUpdate();
extern TextBuffer * OlTextEditTextBuffer();

extern WidgetClass textEditWidgetClass;

#endif
