#include <sys/types.h>
#include <amiga/screen.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h> 
#include <errno.h>
#include "pointer.h"
#include "scrfx.h"
#include "scrfont.h"

#define EVENT_LIST_SIZE	100

#define META(x)		((x) | 0x80)
#define CTRL(x)		((x) & 0x1F)
#define ESC		0x1B
#define CSI		META(ESC)
#define CR		(CTRL('m'))
#define TAB		(0x09)

#define YOFFSET		(8)
#define YPAD		(4)
#define MAXY 		(258)
#define XOFFSET		(152)
#define XPAD		(4)
#define MAXX		(152)

#define REDY		(315)
#define GREENY		(340)
#define BLUEY		(365)
#define COLX		(8)

struct boxes
{
    int y, x, h, w;
};

static void usage(void);
static int select_colors(struct scolor *, int);
static void draw_palette(struct Bitmap *);
static struct boxes *create_color_boxes(struct Bitmap *, int);
static int process_input_events(int, struct Bitmap *, int, struct scolor *,
				struct boxes *);

static void selectcolorbox(struct Bitmap *, struct boxes *, int);
static void setcolorscale(struct Bitmap *, struct scolor *, int);
static void draw_rgb_scales(struct Bitmap *);
static void setcolornums(struct Bitmap *, struct scolor *, int);
static void checkpointer(struct Bitmap *, struct pointer *,
			struct scolor *, int, int);

static char *progname;

static struct font *fp;			/* pointer to font information */

int
main(int argc, char *argv[])
{
    int scrfd;
    char *scrname;
    struct scrtype scrtype;
    struct scolor *scolor;

    progname = *argv++;
    --argc;

    if (argc > 1)
	usage();

    if (argc == 1)
	scrname = *argv;
    else
	scrname = "/dev/stdout";

    if ((fp = openfont(DEFAULT_FONT)) == NULL)
    {
	(void) fprintf(stderr, "%s: Can't open font: %s!\n", DEFAULT_FONT);
	exit(1);
    }

    if ((scrfd = open(scrname, O_RDWR)) < 0)
    {
	(void) fprintf(stderr, "%s: Couldn't open screen device: %s (%s)\n",
		       progname, scrname, strerror(errno));
	exit(1);
    }

    if (ioctl(scrfd, SIOCGETTYPE, &scrtype))
    {
	(void) fprintf(stderr, "%s: SIOCGETTYPE failed (%s)\n", progname,
		       strerror(errno));
	exit(1);
    }

    scolor = (struct scolor *)malloc(sizeof(*scolor) * (1 << scrtype.dispz));

    if (scolor == NULL)
    {
	(void) fprintf(stderr, "%s: Failed to allocate memory for color "
		       "table (%s)\n", progname, strerror(errno));
	exit(1);
    }

    if (ioctl(scrfd, SIOCGETCMAP, scolor))
    {
	(void) fprintf(stderr, "%s: SIOCGETCMAP failed (%s)\n", progname,
		       strerror(errno));
	exit(1);
    }

    if (select_colors(scolor, scrtype.dispz))
    {
	if (ioctl(scrfd, SIOCSETCMAP, scolor))
	{
	    (void) fprintf(stderr, "%s: SIOCSETCMAP failed (%s)\n", progname,
			   strerror(errno));
	    exit(1);
	}
    }

    exit(0);
}

static int
select_colors(struct scolor *scolor, int depth)
{
    int sd, i;
    struct Bitmap bp;
    struct boxes *boxes;
    static struct scrtype scrtype =
    {
	0,				/* flags */
	ST_ANY,				/* type */
	320,				/* width */
	400,				/* height */
	6,				/* depth */
	0,				/* padding */
	SM_HAM|SM_NONOVERSCAN,
    };

    if ((sd = OpenScreen("Screen Color Selector", &scrtype, 0)) < 0)
    {
	(void)fprintf(stderr, "%s: Couldn't open HAM screen device (%s)\n",
		      progname, ScreenError());
	exit(1);
    }

    memset(&bp, 0, (size_t) sizeof(bp));
    
    if (NewBitmap(sd, &bp) == -1)
	goto CloseAndBarf;

    /* Clear screen */ 
    for (i=0; i < bp.depth; ++i)
	memset(bp.bpl[i], 0, (bp.width / 8) * bp.height);

    (void) ScreenColors(sd, scolor);

    draw_palette(&bp);

    if (DisplayScreen(sd))
    {
    UnmapCloseAndBarf:
	for (i=0; i < bp.depth; ++i)
	    (void) munmap(bp.bpl[i], bp.width * bp.height);

    CloseAndBarf:
	(void) CloseScreen(sd);

	return 0;				/* bad things happened */
    }

    if ((boxes = create_color_boxes(&bp, depth)) == NULL)
	goto UnmapCloseAndBarf;

    /* Draw color boxes */
    for (i=0; i < (1 << depth); ++i)
	box(&bp, boxes[i].x, boxes[i].y, boxes[i].h, boxes[i].w, i);

    /* Draw RGB scales */

    draw_rgb_scales(&bp);

    return process_input_events(sd, &bp, depth, scolor, boxes);
}

static void
draw_palette(struct Bitmap *bp)
{
    int r, g, b;
    int base;

    base = YOFFSET * (bp->width / 8) + (COLX/8);

    for (r=0; r < 16; ++r)
    {
	for (g=0; g < 16; ++g)
	{
	    for (b=0; b < 16; ++b)
	    {
		*(unsigned char *)(bp->bpl[5] + base) = 0x7F;
		*(unsigned char *)(bp->bpl[4] + base) = 0xBF;
		*(unsigned char *)(bp->bpl[3] + base) = (0x80&(b<<4)) |
		    (0x40&(r<<3)) | (0x3F*(g>>3));
		*(unsigned char *)(bp->bpl[2] + base) = (0x80&(b<<5)) |
		    (0x40&(r<<4)) | (0x3F*((g>>2)&1));
		*(unsigned char *)(bp->bpl[1] + base) = (0x80&(b<<6)) |
		    (0x40&(r<<5)) | (0x3F*((g>>1)&1));
		*(unsigned char *)(bp->bpl[0] + base) = (0x80&(b<<7)) |
		    (0x40&(r<<6)) | (0x3FFF*(g&1));

		base += sizeof(unsigned char);
	    }

	    base += 24;
	}
    }
}

static void
draw_rgb_scales(struct Bitmap *bp)
{
    *(unsigned char *)(bp->bpl[5] + (REDY * (bp->width/8))) = 0x03;
    *(unsigned char *)(bp->bpl[4] + (REDY * (bp->width/8))) = 0x06;
    line(bp, 8, REDY, 8 + (16 * 8), REDY, 0x2F); /* R */

    *(unsigned char *)(bp->bpl[5] + (GREENY * (bp->width/8))) = 0x03;
    *(unsigned char *)(bp->bpl[4] + (GREENY * (bp->width/8))) = 0x06;
    line(bp, 8, GREENY, 8 + (16 * 8), GREENY, 0x3F); /* G */

    *(unsigned char *)(bp->bpl[5] + (BLUEY * (bp->width/8))) = 0x03;
    *(unsigned char *)(bp->bpl[4] + (BLUEY * (bp->width/8))) = 0x06;
    line(bp, 8, BLUEY, 8 + (16 * 8), BLUEY, 0x1F); /* B */
}

static struct boxes *
create_color_boxes(struct Bitmap *bp, int depth)
{
    int columns = (1 << depth) / 2, rows = 2;
    int c, r;
    struct boxes *boxes;

    boxes = (struct boxes *)malloc(sizeof(*boxes) * (1 << depth));
    
    if (boxes == NULL)
    {
	(void) fprintf(stderr,
		       "%s: Couldn't allocate memory for color buttons.\n",
		       progname);

	return NULL;
    }

    while (columns > rows)
    {
	columns /= 2;
	rows *= 2;
    }

    for (r=0; r < rows; ++r)
    {
	for (c=0; c < columns; ++c)
	{
	    boxes[(r*columns)+c].y = YOFFSET + (MAXY / rows * r) + (r * YPAD);
	    boxes[(r*columns)+c].x = XOFFSET + (MAXX / columns * c)+(c * XPAD);
	    boxes[(r*columns)+c].h = (MAXY / rows) - (2 * YPAD);
	    boxes[(r*columns)+c].w = (MAXX / columns) - (2 * XPAD);
	}
    }

    return boxes;
}

/*
** process_input_events: process as many screen input events as
** possible.
*/

static int
process_input_events(int sd, struct Bitmap *bp, int depth,
		     struct scolor *scolor, struct boxes *boxes)
{
    int events_read, retval, current_color;
    int current_event, last_event;
    struct inputevent event_list[EVENT_LIST_SIZE];
    static struct pointer pointer;

    current_color = 0;
    selectcolorbox(bp, boxes, current_color);
    setcolorscale(bp, scolor, current_color);

    pointer.displayed = B_FALSE;
    pointer.x = bp->width / 2;
    pointer.y = bp->height / 2;
    pointer.width = 16;
    pointer.height = 32;
    pointer.minx = pointer.miny = 0;
    pointer.maxx = bp->width - 1;
    pointer.maxy = bp->height - 1;

    display_pointer(bp, &pointer);

    for (;;)
    {
	if ((events_read = read(sd, event_list, sizeof(event_list))) < 0)
	{
	    /* Ignore Errors... who cares about them here */
	    return;
	}

	last_event = events_read / sizeof(struct inputevent);
	current_event = 0;

	while (current_event < last_event)
	{
	    struct inputevent *ie = &event_list[current_event];

	    switch(ie->type)
	    {
	    case IETYPE_ASCII:
		switch ((ie->code)&0xFF)
		{
		case ESC:
		    return 0;

		case CR:
		    return 1;
		}
		break;

	    case IETYPE_MOUSE:
		switch (ie->class)
		{
		case IECLASS_MOUSEBUTTON:
		    {
			int button, down_p;

			button = ie->code >> 8;
			down_p = ie->code & 1;

			if (button == 0 && down_p)
			{
			    if (pointer.x >= COLX &&
				pointer.x < COLX + (8 * 16) &&
				pointer.y >= YOFFSET &&
				pointer.y < YOFFSET + (8 * 32))
			    {
				/*
				 ** the pointer was clicked in the color
				 ** palate
				 */

				int R, G, B, S;

				R = ((pointer.y - YOFFSET) / 16)<<4;
				G = ((pointer.y - YOFFSET) % 16)<<4;
				B = ((pointer.x - 8) / 8)<<4;
				S = ((((R&0xF0)>>4) + ((G&0xF0)>>4) + ((B&0xF0)>>4)) / 3)<<4;

				setcolorscale(bp, scolor, current_color);

				scolor[current_color].gray = S;
				scolor[current_color].red = R;
				scolor[current_color].green = G;
				scolor[current_color].blue = B;

				(void) ScreenColors(sd, scolor);
				setcolorscale(bp, scolor, current_color);
			    }
			    else
			    {
				int i;

				for (i=0; i < (1 << depth); ++i)
				{
				    if (pointer.x >= boxes[i].x &&
					pointer.x < boxes[i].x +
					boxes[i].w &&
					pointer.y >= boxes[i].y &&
					pointer.y < boxes[i].y +
					boxes[i].h)
				    {
					/*
					 ** the pointer was clicked in
					 ** a color box
					 */

					if (i != current_color)
					{
					    selectcolorbox(bp,
							   boxes,
							   current_color);
					    setcolorscale(bp, scolor,
							  current_color);
					    current_color = i;
					    selectcolorbox(bp,
							   boxes,
							   current_color);
					    setcolorscale(bp,
							  scolor,
							  current_color);
					}
					break;
				    }
				}
			    }

			    checkpointer(bp, &pointer, scolor,
					 current_color, sd);
			}
			else if (button == 0 && !down_p)
			{
			    pointer.minx = pointer.miny = 0;
			    pointer.maxx = bp->width - 1;
			    pointer.maxy = bp->height - 1;
			}

		    }
		    break;

		case IECLASS_MOUSEMOTION:
		    {
			signed char deltax, deltay;

			deltax = (signed char)(ie->code>>8)&0xFF;
			deltay = (signed char)(ie->code&0xFF);

			move_pointer(bp, &pointer, deltax, deltay);
		    }
		}
	    }

	    ++current_event;
	}
    }
}

static void
checkpointer(struct Bitmap *bp, struct pointer *pointer,
	     struct scolor *scolor, int current_color, int sd)
{
    if (pointer->x >= COLX && pointer->x < COLX+(8*16) &&
	     pointer->y >= (REDY-10))
    {
	int x = (pointer->x - COLX) / 8;
	/* Pointer is on an RGB scale */

	/* Does is event near Red scale? */
	if (pointer->y >= (REDY-5) &&
	    pointer->y <= (REDY+5))
	{
	    if (x != (scolor[current_color].red>>4))
	    {
		setcolorscale(bp,
			      scolor,
			      current_color);
		scolor[current_color].red = (x << 4);

		scolor[current_color].gray =
		    (((scolor[current_color].red>>4) +
		     (scolor[current_color].green>>4) +
		     (scolor[current_color].blue>>4)) /
			 3)<<4;
					    
		(void) ScreenColors(sd, scolor);
		setcolorscale(bp,
			      scolor,
			      current_color);
	    }
	    else
	    {
		pointer->minx=COLX;
		pointer->maxx=COLX + (8 * 16);
		pointer->miny=REDY-5;
		pointer->maxy=REDY+5;
	    }
	}
	else if (pointer->y >= (GREENY-5) &&
		 pointer->y <= (GREENY+5))
	{
	    /* Green hack */
	    if (x != (scolor[current_color].green>>4))
	    {
		setcolorscale(bp,
			      scolor,
			      current_color);
		scolor[current_color].green = (x << 4);

		scolor[current_color].gray =
		    (((scolor[current_color].red>>4) +
		     (scolor[current_color].green>>4) +
		     (scolor[current_color].blue>>4)) /
			 3)<<4;
					    
		(void) ScreenColors(sd, scolor);
		setcolorscale(bp,
			      scolor,
			      current_color);
	    }
	    else
	    {
		pointer->minx=COLX;
		pointer->maxx=COLX + (8 * 16);
		pointer->miny=GREENY-5;
		pointer->maxy=GREENY+5;
	    }
	}
	else if (pointer->y >= (BLUEY-5) &&
		 pointer->y <= (BLUEY+5))
	{
	    /* blue hack */

	    if (x != (scolor[current_color].blue>>4))
	    {
		setcolorscale(bp,
			      scolor,
			      current_color);
		scolor[current_color].blue = (x << 4);

		scolor[current_color].gray =
		    (((scolor[current_color].red>>4) +
		     (scolor[current_color].green>>4) +
		     (scolor[current_color].blue>>4)) /
			 3)<<4;
					    
		(void) ScreenColors(sd, scolor);
		setcolorscale(bp,
			      scolor,
			      current_color);
	    }
	    else
	    {
		pointer->minx=COLX;
		pointer->maxx=COLX + (8 * 16);
		pointer->miny=BLUEY-5;
		pointer->maxy=BLUEY+5;
	    }
	}
    }
}


static void
selectcolorbox(struct Bitmap *bp, struct boxes *boxes, int color)
{
    line(bp,
	 boxes[color].x - 2,
	 boxes[color].y - 2,
	 boxes[color].x - 2,
	 boxes[color].y + boxes[color].h + 2,
	 -1);

    line(bp,
	 boxes[color].x - 2,
	 boxes[color].y + boxes[color].h + 2,
	 boxes[color].x + boxes[color].w + 2,
	 boxes[color].y + boxes[color].h + 2,
	 -1);

    line(bp,
	 boxes[color].x + boxes[color].w + 2,
	 boxes[color].y + boxes[color].h + 2,
	 boxes[color].x + boxes[color].w + 2,
	 boxes[color].y - 2,
	 -1);

    line(bp,
	 boxes[color].x - 2,
	 boxes[color].y - 2,
	 boxes[color].x + boxes[color].w + 2,
	 boxes[color].y - 2,
	 -1);
}

static void
setcolorscale(struct Bitmap *bp, struct scolor *scolor, int color)
{
    /* RED */
    box(bp, COLX+((scolor[color].red>>4)*8), REDY-5, 5, 8, -1);
    box(bp, COLX+((scolor[color].red>>4)*8), REDY+1, 5, 8, -1);

    /* GREEN */
    box(bp, COLX+((scolor[color].green>>4)*8), GREENY-5, 5, 8, -1);
    box(bp, COLX+((scolor[color].green>>4)*8), GREENY+1, 5, 8, -1);

    /* BLUE */
    box(bp, COLX+((scolor[color].blue>>4)*8), BLUEY-5, 5, 8, -1);
    box(bp, COLX+((scolor[color].blue>>4)*8), BLUEY+1, 5, 8, -1);

    setcolornums(bp, scolor, color);
}

static void
setcolornums(struct Bitmap *bp, struct scolor *scolor, int color)
{
    scrprintfat(bp, fp, REDY-5, 150, "%2d", scolor[color].red >> 4);
    scrprintfat(bp, fp, GREENY-5, 150, "%2d", scolor[color].green >> 4);
    scrprintfat(bp, fp, BLUEY-5, 150, "%2d", scolor[color].blue >> 4);
}

static void
usage()
{
    (void) fprintf(stderr, "usage: %s scrdevicename\n", progname);
    exit(1);
}
