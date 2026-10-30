/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	NOIDENT
#ident	"@(#)flat:Flat.h	1.15"
#endif

#ifndef _OL_FLAT_H
#define _OL_FLAT_H

/*
 ************************************************************************	
 * Description:
 *	This is the flat container meta class's public header file.
 ************************************************************************	
 */

#include <Xol/Primitive.h>

/*
 ************************************************************************	
 * Define class and instance pointers:
 *	- extern pointer to class data/procedures
 *	- typedef pointer to widget's class structure
 *	- typedef pointer to widget's instance structure
 ************************************************************************	
 */

extern WidgetClass		flatWidgetClass;
typedef struct _FlatClassRec *	FlatWidgetClass;
typedef struct _FlatRec *	FlatWidget;

#endif /* _OL_FLAT_H */
