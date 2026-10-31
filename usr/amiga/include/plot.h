/*
 * This include file is for users to produce plot files.
 */


#define	WHITE	'W'
#define	BLACK	'B'
#define	XOR	'X'


extern void	space(),		/* set user coordinate range */
		aspace(),		/* space() but use aspect ratio */
		line(),			/* draw line */
		rfill(),		/* rectangle fill */
		cont(),			/* continue line */
		move(),			/* change current position */
		point();		/* draw dot */
