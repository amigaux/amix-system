

#define	NARG	3			/* # words in a message */

/*
 * command argument to ioctl( )
 */
#define	CIOSENDM	('c'<<8 | 0)
#define	CIOGETM		('c'<<8 | 1)
#define	CIOGETD		('c'<<8 | 2)
#define	CIOBOMB		('c'<<8 | 3)
#define	CIOGIVEC	('c'<<8 | 4)
#define	CIOMOUSE	('c'<<8 | 5)
#define	CIOSPRITE	('c'<<8 | 6)
#define	CIOSENDA	('c'<<8 | 7)
#define	CIOTAKEA	('c'<<8 | 8)
#define	CIOSCROLL	('c'<<8 | 9)
#define	CIOCOPSET	('c'<<8 | 10)
#define	CIOCURSOR	('c'<<8 | 11)
#define	CIOCURCUR	('c'<<8 | 12)
#define	CIOKILLC	('c'<<8 | 13)
#define	CIOSCUM		('c'<<8 | 100)

/*
 * errno extras
 */
#define	EDBUSY	1
#define	EDATTN	2

/*
 * message sources/destinations
 *	The order may not be changed.  The parameters may not be changed,
 * except WINDOW and DESTMAX.  When sending messages, the INDIRECT bit directs
 * delivery to the window mgr of the dst rather than the dst itself.
 */
#define	DRIVER		0		/* hr console driver */
#define	SMGR		1		/* screen manager */
#define	WMGR		2		/* window managers (4 of them) */
#define	WINDOW		6		/* windows (4+4 of them) */
#define	DESTMAX		(WINDOW+4+4)	/* first invalid src/dst */
#define	INDIRECT	0100		/* indirect-destination bit */


struct message {
	uchar	m_src,
		m_com;
	uint	m_msg[NARG];
};
#define	m_dst	m_src

struct xfer {
	uint	x_src,
		x_count;
	char	*x_srcp,
		*x_dstp;
};

struct mrect {
	uint	mr_xorg,
		mr_yorg,
		mr_xlen,
		mr_ylen;
	bool	mr_inside;
};

struct scum {
	uint	n;
	char	buf[5120];
};


/* m_com
 */
#define	DMOUSE		0
#define	DKEY		1
#define	DGETC		2
#define	DPUTD		3
#define	DTCGETA		4
#define	DTCSETA		5
#define	DSCROLLED	6
#define	DFLUSH		99
#define	DSCUM		100

/*
 * names of mouse keys used in DMOUSE messages
 */
#define	MKLEFT	0x80			/* left-hand mouse key */
#define	MKRIGHT	0x40			/* right-hand mouse key */
