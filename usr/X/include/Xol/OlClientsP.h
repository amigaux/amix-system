/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)olclib:OlClientsP.h	1.4"

#ifndef _OlClientsP_h
#define _OlClientsP_h

#include <Xol/OlClients.h>

#ifdef DELIMITER
#undef DELIMITER
#endif

#define DELIMITER	0x1f

#define DEF_STRING(s,d)	        (s == NULL ? d : s)
#define NULL_DEF_STRING(s)      DEF_STRING(s,"")

#endif
