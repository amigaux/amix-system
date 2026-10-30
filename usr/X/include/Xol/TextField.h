/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)textfield:include/Xol/TextField.h	1.9"
#endif

/*
 * TextField.h
 *
 */

#ifndef _TextField_h
#define _TextField_h

#include <Xol/Manager.h>   /* include superclasses' header (yuch)*/

typedef struct _TextFieldClassRec *TextFieldWidgetClass;
typedef struct _TextFieldRec      *TextFieldWidget;

typedef enum 
   {OlTextFieldReturn, OlTextFieldPrevious, OlTextFieldNext} OlTextVerifyReason;

typedef struct
   {
   String             string;
   Boolean            ok;
   OlTextVerifyReason reason;
   } OlTextFieldVerify, *OlTextFieldVerifyPointer;

extern int    OlTextFieldCopyString();
extern char * OlTextFieldGetString();

extern WidgetClass /*NEW*/textFieldWidgetClass;

#endif /* _TextField_h */
