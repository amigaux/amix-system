/*
 * The following definitions make C more amenable to a purist.
 */
#ifndef	HSTYLE_H
#define	HSTYLE_H


#define	bool	char			/* boolean type */
#define	uint	unsigned int		/* short names for unsigned types */
#define	ulong	unsigned long
#define	uchar	unsigned char
#define	ushort	unsigned short int
#define	not	!			/* logical negation operator */
#define	and	&&			/* logical conjunction */
#define	or	||			/* logical disjunction */
#define	TRUE	(0 == 0)
#define	FALSE	(not TRUE)
#define	loop	while (TRUE)		/* loop until break */
#define	EOS	'\0'			/* end-of-string char */
#define	NULL	0			/* invalid pointer */

#define	nel(a)		(sizeof( a) / sizeof( (a)[0]))
#define	endof(a)	(&(a)[nel( a)])
#define	bitsof(a)	(sizeof(a) * 8)
#endif
