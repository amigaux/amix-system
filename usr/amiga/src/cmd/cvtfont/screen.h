/*
 * <sys/screen.h>
 *
 * At the moment, this file contains two sets of information:
 * 1) the structure definitions and manifest constants needed by
 *    user-level programs,
 * and
 * 2) the declarations for the internal kernel data structures
 *    used by the "screens" kernel subsystem and the device drivers
 *    which use it.
 *
 * Perhaps this should be split into two files someday.
 *
 */



/* fields in displaytype */
#define DT_NTSC		0x0001			/* NTSC video & clock rate */
#define DT_PAL		0x0002			/* PAL video & clock rate */
#define DT_HEDLEY	0x0004			/* 2024 monitor attached */
#define DT_AGNUSVER	0x0070			/* Agnus version */
#define DTB_AGNUSVER	4			/* Agnus version bit # */
#define DT_DENISEVER	0xff00			/* Denise version */
#define DTB_DENISEVER	8			/* Denise version bit # */

#define MAXBPL	8

#ifdef INKERNEL
extern unsigned int displaytype;

#define MAXSCREENS 32

#define splscr() spl3()
#endif /* INKERNEL */



/*
 * KEYMAP INFORMATION
 */

struct keyent
{
    unsigned int ke_actiontype:4;
    unsigned int ke_flags:4;
    unsigned int ke_value:24;
};

/* keyent.actiontype */
#define KA_CHR    0			/* Return a character */
#define KA_STR8   1			/* Return a string of 8-bit chars */
#define KA_STR16  2			/* Return a string of 16-bit chars */
#define KA_STR32  3			/* Return a string of 32-bit chars */
#define KA_STAT   5			/* Set a state bit */
#define KA_TAB    6			/* Lookup table */
#define KA_NOP    7			/* Do nothing */

/* keyent.flags */
#define KF_NOREP  0x1			/* Do not repeat */

struct keymap
{
    unsigned short km_magic;			/* Keymap magic number */
    unsigned char km_flags;
    unsigned char km_count;			/* Number of references */
    unsigned long km_length;			/* Total # bytes */
    unsigned char km_shiftkeys[16][4];		/* Definition of shift bits */
    struct keyent km_toptable[256];		/* Top level table */
    /* unsigned char km_tablearea[0]; */	/* Lower tables begin here */
};

/* keymap.km_magic */
#define KM_MAGIC 0x2a4b

#ifdef INKERNEL
extern struct keymap *defkmap, builtinkmap;
#endif /* INKERNEL */



/*
 * FONT INFORMATION
 *
 * WARNING: the layouts of (struct charent) and (struct font) are also
 * known by the code in font.s and the builtinfonts in fonts.s, neither
 * of which get them from this file!
 */

struct charent
{
    unsigned char ce_height;			/* Height of bitmap */
    unsigned char ce_width;			/* Width of bitmap */
    unsigned char ce_basel;			/* # pixels above baseline */
    unsigned char ce_move;			/* Horiz. cursor increment */
    unsigned char ce_bpl;			/* Bytes per line */
    unsigned char ce_pad;
    unsigned short ce_data;			/* Offset of bitmap data */
};

struct font
{
    unsigned short f_magic;			/* Font file magic number */
    unsigned short f_flags;
    unsigned short f_count;			/* Number of references */
    unsigned short f_pad0;
    unsigned long f_length;			/* Total # bytes */
    unsigned char f_height, f_width;		/* "Typical" character size */
    unsigned char f_baseline;			/* # pixels above baseline */
    unsigned char f_pad1;
    struct charent f_ctable[256];		/* Character table */
};

/* font.f_magic */
#define F_MAGIC 0x2a46

/* font.f_flags */
#define FF_BUILTIN  0x80000

#ifdef INKERNEL
extern struct font *deffont[4], *builtinfont[4];
#endif /* INKERNEL */



/*
 * MISCELLANEOUS `INKERNEL' INFORMATION
 */

#ifdef INKERNEL

#define MAXBPL	8			/* Max bitplanes in a bitmap */

#define Bf_ACTIVE	0x01
#define Bf_MAPPED	0x02
#define Bf_CHUNKY	0x04			/* Chunky pixels */

struct bitmap
{
    unsigned short flags;
    unsigned short width, height, depth, offset;
    unsigned short pad0;
    unsigned char *bpl[MAXBPL];
};

struct screen
{
    unsigned short flags;
    unsigned short modes;			/* Graphics modes, etc. */
    void *user;					/* for higher level */
    unsigned long idx;				/* for higher level */
    struct screen *parent;			/* Screen to show on close */
    void (*mifunc)();				/* Misc function handler */
    void (*kbfunc)();				/* Keyboard handler */
    unsigned short *coplist;			/* Current copper list */
    short hpos, vpos;				/* Position of screen */
    unsigned short height, width, depth;	/* Display size (bits) */
    unsigned short color[32];			/* Colors */
    unsigned char drawmode;			/* JAM1, etc. */
    unsigned char fgpen, bgpen;			/* fg/bg colors */
    short row, col;				/* Cursor row and column */
    struct bitmap *bmap;			/* Pointer to current bitmap */
    struct keymap *kmap;			/* Keyboard map */
    struct font *font[4];			/* Pointers to current fonts */
};

/* screen.flags */
#define Sf_INUSE	0x0001			/* "open" */
#define Sf_DISPLAY	0x0002			/* On the display */
#define Sf_KBD		0x0004			/* Getting keyboard input */
#define Sf_RAWKEY	0x0100			/* Send "raw" keyboard codes */
#define Sf_NEEDCOP	0x0200			/* Needs a new coplist */
#define Sf_COPUSED	0x0400			/* sp->coplist has been used */
#define Sf_VBCALL	0x0800			/* Do a SC_VBCALL next vb */
#define Sf_COPBUSY	0x1000			/* Coplist is being built */

/* screen.modes */
#define Sm_HIRES	0x0001			/* Amiga hires (width=640) */
#define Sm_ILACE	0x0002			/* Interlaced screen */
#define Sm_SUPERHIRES	0x0004			/* ECS superhires (w=1280) */
#define Sm_FASTSCAN	0x0008			/* ECS productivity (h=480) */
#define Sm_HAM		0x01			/* Amiga Hold-And-Modify */
#define Sm_HALFBRITE	0x02			/* Amiga Extra HalfBrite */
#define Sm_HEDLEY_F4	0x0100			/* Hedley-hires screen (F4) */
#define Sm_HEDLEY_F6	0x0200			/* Hedley-hires screen (F6) */
#define Sm_HEDLEY	0x0300			/* Hedley-hires screen (any) */

#define NCOLREGS(sp) \
    ((sp)->depth>5 ? ((sp)->modes&Sm_HAM ? 16 : 32) : 1<<(sp)->depth)

/* For screen.drawmode */
#define JAM1		0		/* jam 1 color into raster */
#define JAM2		1		/* jam 2 colors into raster */
#define COMPLEMENT	2		/* XOR bits into raster */
#define INVERSVID	4		/* inverse video for drawing modes */


/* 'cmd' arg to (*mifunc)() */
#define SC_CHANGE	1			/* Changing DISPLAY/KBD */
#define SC_BLDCOP	2			/* Rebuild copperlist */
#define SC_VBCALL	3			/* Vblank interrupt */


extern unsigned char mousebuttons;

extern struct screen *OpenScreen();
extern void CloseScreen(), SelectScreen(), DisplayScreen();
extern void SetKmap(), SetDefKmap();
extern struct keymap *GetKmap();

#endif /* INKERNEL */



/*
 * MISCELLANEOUS USER-LEVEL INFORMATION
 */

#define MB_0	1				/* Left */
#define MB_1	2				/* Middle */
#define MB_2	4				/* Right */


/* Users' ioctl commands and parameters */

#define SIOC		('S'<<8)
#define SIOCDISPLAYTYPE	(SIOC|1)
#define SIOCFRONT	(SIOC|2)
#define SIOCBACK	(SIOC|3)
#define SIOCACTIVATE	(SIOC|4)
#define SIOCSETKMAP	(SIOC|5)
#define SIOCGETKMAP	(SIOC|6)
#define SIOCSETDEFKMAP	(SIOC|7)
#define SIOCSETCMAP	(SIOC|10)
#define SIOCGETCMAP	(SIOC|11)
#define SIOCSETDEFCMAP	(SIOC|12)
#define SIOCSETFONT	(SIOC|0x80)		/* plus font number */
#define SIOCGETFONT	(SIOC|0x90)		/* plus font number */
#define SIOCSETDEFFONT	(SIOC|0xa0)		/* plus font number */

#define SIOCSETTYPE	(SIOC|32)
#define SIOCGETTYPE	(SIOC|33)
#define SIOCALLOCBMAP	(SIOC|34)
#define SIOCSELBMAP	(SIOC|35)
#define SIOCMAPBMAP	(SIOC|36)
#define SIOCSETINP	(SIOC|37)

struct scolor
{
    unsigned char gray;				/* 0-255 gray scale value */
    unsigned char red, green, blue;		/* 0-255 RGB */
};


#define	SIOCWINSIZE	(('j'<<8)|5)

struct winsize					/* results of SIOCWINSIZE */
{
    unsigned char	charsx, charsy;		/* Window size in characters */
    unsigned short	pixsx, pixsy;		/* Window size in pixels */
};

struct inputevent
{
    unsigned char type;
    unsigned char class;
    unsigned short code;
    unsigned long qualifiers;
};

struct scrtype
{
    unsigned short flags;			/* Notes about this request */
    unsigned short type;			/* Should be zero */
    unsigned short dispx, dispy;		/* Display size in pixels */
    unsigned short dispz;			/* Display depth in bits */
    unsigned long modes;			/* Display modes (HAM, etc.) */
};

#define ST_ANY		0x00			/* Any screen type */
#define ST_AMIGA	0x01

#define SF_LESSPLANES	0x01			/* Fewer planes are OK */
#define SF_MOREPLANES	0x02			/* More planes are OK */
#define SF_LESSRES	0x04			/* Less resolution is OK */
#define SF_MORERES	0x08			/* More resolution is OK */

#define SM_HAM		0x01			/* Amiga Hold-And-Modify */
#define SM_HALFBRITE	0x02			/* Amiga Extra HalfBrite */
#define SM_INTERLACE	0x04			/* Interlace */
#define SM_NONLACE	0x08			/* Not Interlace */
#define SM_HEDLEY	0x10			/* A2024 Hires mode */
#define SM_NONHEDLEY	0x20			/* Not A2024 Hires mode */
#define SM_HIRES	0x40			/* 640 x ? */
#define SM_LORES	0x80			/* 320 x ? */

struct bmap
{
    unsigned short flags;			/* Notes about this request */
    unsigned short bmapx, bmapy;		/* Bitmap size in pixels */
    unsigned short bmapz;			/* Bits per pixel */
};

struct mapbmap
{
    unsigned char mapnum;			/* Which bitmap to map */
    struct {
	unsigned char *virtaddr;		/* Desired address, or -1 */
	unsigned long nbytes;			/* Number of bytes desired */
    } bpl[MAXBPL];
};


/*
Local Variables:
comment-column: 48
End:
*/
