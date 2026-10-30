/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)olmisc:array.h	1.7"
#endif

#ifndef _Ol_array_h_
#define _Ol_array_h_

/*************************************************************************
 *
 * Description:	OpenLook interface to a generic array
 *
 *******************************file*header******************************/

#include <X11/IntrinsicP.h>	/* for XtPointer */
 
#include <Xol/OpenLookP.h>	/* for OL_ARGS, OlNeedFunctionPrototypes */

typedef XtPointer _OlArrayElement;

typedef struct {
    _OlArrayElement *	array;
    Cardinal		num_elements;
    Cardinal		num_slots;
} _OlArrayRec, * _OlArray;

#define _OL_NULL_ARRAY		((XtPointer *)0)
#define _OL_NULL_ARRAY_INDEX	-1
#define _OL_ARRAY_IS_EMPTY(A)	((A)->num_elements == 0)
#define _OL_ARRAY_INITIAL	{ 0 , 0 , 0 }

void	_OlArrayDelete OL_ARGS((_OlArray, int));
#define _OlArrayFree(arrayPtr) \
	    { \
		XtFree((arrayPtr)->array); \
		_OlArrayInit(arrayPtr); \
	    }
#define _OlArrayInit(arrayPtr) \
	    { \
		(arrayPtr)->num_elements	= 0; \
		(arrayPtr)->num_slots		= 0; \
		(arrayPtr)->array		= NULL; \
	    }
#define	_OlArrayAppend(arrayPtr, data) \
		_OlArrayInsert((arrayPtr), (arrayPtr)->num_elements, data)
#define	_OlArrayUniqueAppend(arrayPtr, data) \
	    if (_OlArrayFind(arrayPtr,data) == _OL_NULL_ARRAY_INDEX)	\
		_OlArrayInsert(arrayPtr,(arrayPtr)->num_elements,data);	\
	    else
#define	_OlArrayInsert(arrayPtr, indx, data) \
		__OlArrayInsert(arrayPtr, indx, (_OlArrayElement)(data))
void	__OlArrayInsert OL_ARGS((_OlArray, int, _OlArrayElement));

#define	_OlArrayFind(arrayPtr, data) \
		__OlArrayFind(arrayPtr, (_OlArrayElement)(data))
int	__OlArrayFind OL_ARGS((_OlArray, _OlArrayElement));

int	_OlArrayFindByName OL_ARGS((_OlArray, String));

#define _OlArrayItem(arrayPtr, indx)	((arrayPtr)->array[indx])
#define _OlArraySize(arrayPtr)		(arrayPtr)->num_elements

#endif /* _Ol_array_h_ */
