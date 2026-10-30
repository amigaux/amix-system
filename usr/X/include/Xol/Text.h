/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)text:include/Xol/Text.h	1.8"

#ifndef _OlText_h
#define _OlText_h

#include <Xol/Manager.h>		/* include superclasses' header */

/* Class record constants */

extern WidgetClass textWidgetClass;

typedef struct _TextClassRec	*TextWidgetClass;
typedef struct _TextRec	*TextWidget;

#endif
