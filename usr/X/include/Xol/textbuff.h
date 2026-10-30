/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)olmisc:textbuff.h	1.7"
#endif

/*
 * textbuff.h
 *
 */

#ifndef _textbuff_h
#define _textbuff_h
#include <stdio.h>

/* private private private private private private private private */
/* private private private private private private private private */
/* private private private private private private private private */
/* private private private private private private private private */

/* TextBuffer table sizes and increments */

#define FTALLOC         8
#define LTALLOC         2
#define PTALLOC         4
#define BTALLOC         8
#define MAXLPP         96
#define BLOCKSIZE    2048
#define PQLIMIT        16

/* public public public public public public public public */
/* public public public public public public public public */
/* public public public public public public public public */
/* public public public public public public public public */

/* standard types               */

typedef int TextPosition;
typedef int TextLine;
typedef int TextPage;
typedef int TextBlock;

/* status of a text buffer file */

typedef enum
   {
   NOTOPEN, READWRITE, READONLY, NEWFILE
   } TextFileStatus;

/* status of edit operations    */

typedef enum
   {
   EDIT_FAILURE, EDIT_SUCCESS
   } EditResult;

/* Scan routine return values   */
/* and direction definitions    */

typedef enum
   {
   SCAN_NOTFOUND, SCAN_WRAPPED, SCAN_FOUND, SCAN_INVALID
   } ScanResult;

/* SaveTextBuffer status        */

typedef enum
   {
   SAVE_FAILURE, SAVE_SUCCESS
   } SaveResult;

/* TextEditOperations     */

#define TEXT_BUFFER_NOP                (0)
#define TEXT_BUFFER_DELETE_START_LINE  (1L<<0)
#define TEXT_BUFFER_DELETE_START_CHARS (1L<<1)
#define TEXT_BUFFER_DELETE_END_LINE    (1L<<2)
#define TEXT_BUFFER_DELETE_END_CHARS   (1L<<3)
#define TEXT_BUFFER_DELETE_JOIN_LINE   (1L<<4)
#define TEXT_BUFFER_DELETE_SIMPLE      (1L<<5)
#define TEXT_BUFFER_INSERT_SPLIT_LINE  (1L<<6)
#define TEXT_BUFFER_INSERT_LINE        (1L<<7)
#define TEXT_BUFFER_INSERT_CHARS       (1L<<8)

typedef Bufferof(TextBlock) BlockTable;

typedef struct
   {
   TextPage      pageindex;
   unsigned long timestamp;
   } PageQueue;

typedef struct
   {
   TextPosition  bytes;
   TextLine      lines;
   TextPage      qpos;
   BlockTable *  dpos;
   } Page;

typedef struct
   {
   TextPage      pageindex;
   Buffer *      buffer;
   unsigned long userData;
   } Line;

typedef Bufferof(Page) PageTable;

typedef Bufferof(Line) LineTable;

typedef struct _TextLocation
   {
   TextLine        line;
   TextPosition    offset;
   BufferElement * buffer;
   } TextLocation;

typedef int TextUndoHint;

typedef struct _TextUndoItem
   {
   char *       string;
   TextLocation start;
   TextLocation end;
   TextUndoHint hint;
   } TextUndoItem;

typedef void (*TextUpdateFunction)();
typedef struct _TextUpdateCallback
   {
   TextUpdateFunction f;
   caddr_t            d;
   } TextUpdateCallback;

typedef struct _TextBuffer
   {
   char *        filename;
   FILE *        tempfile;
   TextBlock     blockcnt;
   TextBlock     blocksize;
   LineTable     lines;
   PageTable     pages;
   BlockTable *  free_list;
   PageQueue     pqueue[PQLIMIT];
   TextPage      pagecount;
   TextPage      pageref;
   TextPage      curpageno;
   Buffer *      buffer;
   char          dirty;
   TextFileStatus       status;
   int                  refcount;
   TextUpdateCallback * update;
   TextUndoItem         deleted;
   TextUndoItem         insert;
   } TextBuffer;

/* `executable' macros          */

#define TextBufferUserData(text,line)  text-> lines.p[line].userData
#define TextBufferName(text)           (text-> filename)
#define TextBufferModified(text)       (text-> dirty)
#define TextBufferEmpty(text)          (text-> lines.used == 1 && \
                                        text-> lines.p[0].buffer-> used == 1)
#define TextBufferNamed(text)          (text-> filename != NULL)
#define LinesInTextBuffer(text)        (text-> lines.used)
#define LastTextBufferLine(text)       (text-> lines.used - 1)
#define LastCharacterInTextBufferLine(text, line)              \
   (text-> lines.p[line].buffer-> used - 1)
#define LengthOfTextBufferLine(text, line) (text-> lines.p[line].buffer-> used)

#define SameTextLocation(x,y)         (x.line == y.line && x.offset == y.offset)

/* extern interfaces            */

extern TextBuffer * AllocateTextBuffer();
extern TextBuffer * ReadFileIntoTextBuffer();
extern TextBuffer * ReadStringIntoTextBuffer();

extern char *       GetTextBufferLocation();

extern ScanResult   ForwardScanTextBuffer();
extern ScanResult   BackwardScanTextBuffer();

extern EditResult   ReplaceBlockInTextBuffer();
extern EditResult   ReplaceCharInTextBuffer();

extern TextLocation IncrementTextBufferLocation();
extern TextLocation LocationOfPosition();
extern TextLine     LineOfPosition();
extern TextPosition PositionOfLocation();
extern TextPosition PositionOfLine();
extern TextPosition LastTextBufferPosition();
extern TextLocation LastTextBufferLocation();
extern TextLocation StartCurrentTextBufferWord();
extern TextLocation EndCurrentTextBufferWord();
extern TextLocation PreviousTextBufferWord();
extern TextLocation NextTextBufferWord();
extern TextLocation NextLocation();
extern TextLocation PreviousLocation();

extern char *       GetTextBufferLine();
extern int          GetTextBufferChar();
extern int          CopyTextBufferBlock();
extern char *       GetTextBufferBlock();
extern Buffer *     GetTextBufferBuffer();
extern SaveResult   SaveTextBuffer();

extern void         RegisterTextBufferUpdate();
extern int          UnregisterTextBufferUpdate();

extern void         RegisterTextBufferScanFunctions();
extern void         RegisterTextBufferWordDefinition();

#endif
