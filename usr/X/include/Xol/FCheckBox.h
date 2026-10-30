/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)flat:FCheckBox.h	1.2"
#endif

#ifndef _OL_FCHECKBOX_H
#define _OL_FCHECKBOX_H

/*
 ************************************************************************	
 * Description:
 *	This is the flat checkbox container's public header file.
 ************************************************************************	
 */

#include <Xol/FExclusive.h>

/*
 ************************************************************************	
 * Define class and instance pointers:
 *	- extern pointer to class data/procedures
 *	- typedef pointer to widget's class structure
 *	- typedef pointer to widget's instance structure
 ************************************************************************	
 */

extern WidgetClass			flatCheckBoxWidgetClass;
typedef struct _FlatCheckBoxClassRec *	FlatCheckBoxWidgetClass;
typedef struct _FlatCheckBoxRec *	FlatCheckBoxWidget;

#endif /* _OL_FCHECKBOX_H */
