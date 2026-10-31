

#include <stdio.h>

#include "exec/types.h"
#include "libraries/diskfont.h"

#include "screen.h"

#define		uchar		unsigned char
#define		ulong		unsigned long


struct charDef {
	WORD charOffset;
	WORD charBitWidth;
};



extern ULONG *LoadSeg();
void cvtfont();

char *progname;

/* The buffer where the keymap will be built. */
union
{
    struct font u_font;
    uchar u_data[65536];	/* maximum size allowed */
} fontbuf;


#define FONT (fontbuf.u_font)
#define OFF(x) ((unsigned int)((uchar *)(x) - fontbuf.u_data))

uchar *dataend = fontbuf.u_data + sizeof (struct font);


static uchar *alloc(n)
unsigned n;
{
uchar *p = dataend;

	p = (uchar *)((((ulong)p)+3)&~3);

	dataend = p + n;
	if (dataend > (fontbuf.u_data + sizeof (fontbuf.u_data))) {
		fprintf(stderr, "%s: font too large!\n", progname);
		exit(1);
	}
	return(p);
}


main(argc, argv)
int argc;
char *argv[];
{
    int status=0;
    ULONG *seglist;

    progname = *argv;

    ++argv, --argc;

    if (argc < 1)
    {
    usage:
	fprintf(stderr, "Usage: %s <filename>\n", progname);
	return -1;
    }

    if (!(seglist = LoadSeg(*argv)))
    {
	char buf[BUFSIZ];
	sprintf(buf, "%s: can't LoadSeg %s", progname, *argv);
	perror(buf);
	return -2;
    }

    if (seglist[0])
	fprintf(stderr, "Hmm... there is more than one hunk here...\n");

    cvtfont( (struct DiskFontHeader *) (seglist + (char *) 8) );

    (void)fwrite(&FONT, FONT.f_length, 1, stdout);

    if (ferror(stdout))
    {
	char buf[BUFSIZ];
	sprintf(buf, "%s: stdout write error", progname);
	perror(buf);
	return -3;
    }

    return 0;
}


#define		TF	h->dfh_TF	/* TextFont within DiskFontHeader */

static void cvtfont(h)
struct DiskFontHeader *h;
{
int i, j, k;
WORD *charspace;
WORD *charkern;
struct charDef *chardef;
uchar *p, *q;
ulong *lp, *lq;
int byte_off, bit_off;
int bpl;
ulong l;
int offset;
int width;
int space;
int kern;

	memset(FONT.f_ctable, 0, sizeof(FONT.f_ctable));

	FONT.f_magic = F_MAGIC;
	FONT.f_flags = 0;
	FONT.f_count = 0;
	FONT.f_pad0  = 0;
	FONT.f_pad1  = 0;

	fprintf(stderr, "dfh_Name is %s\n", h->dfh_Name);

	FONT.f_height	= TF.tf_YSize;
	FONT.f_width	= TF.tf_XSize;
	FONT.f_baseline	= TF.tf_Baseline;

	fprintf(stderr, "Height   =   %d\n", FONT.f_height);
	fprintf(stderr, "Width    =   %d\n", FONT.f_width);
	fprintf(stderr, "Baseline =   %d\n", FONT.f_baseline);
	fprintf(stderr, "LoChar   =   %d\n", TF.tf_LoChar);
	fprintf(stderr, "HiChar   =   %d\n", TF.tf_HiChar);

	charspace = (WORD *) TF.tf_CharSpace;
	charkern  = (WORD *) TF.tf_CharKern;
	chardef   = (struct charDef *) TF.tf_CharLoc;

	for (i = TF.tf_LoChar; i <= TF.tf_HiChar; i++) {

		FONT.f_ctable[i].ce_height	= FONT.f_height;
		FONT.f_ctable[i].ce_basel	= FONT.f_baseline;

		if (charspace == NULL)
			space = FONT.f_width;
		else
			space = (int) charspace[i - TF.tf_LoChar];

		FONT.f_ctable[i].ce_move	= space;
		FONT.f_ctable[i].ce_width	= space;

		bpl = 4;
		FONT.f_ctable[i].ce_bpl = bpl;

		p = alloc(bpl * FONT.f_height);
		FONT.f_ctable[i].ce_data = OFF(p);

		offset = (int) chardef[i - TF.tf_LoChar].charOffset;
		width	 = chardef[i - TF.tf_LoChar].charBitWidth;

		if (charkern == NULL)
			kern = 0;
		else
			kern	 = charkern[i - TF.tf_LoChar];

		if (kern < 0) {
			offset -= kern;
			width += kern;
			kern = 0;
		}

		if (width + kern > space)
			width = space - kern;

		byte_off = offset / 8;
		bit_off  = offset % 8;

		q = (uchar *) TF.tf_CharData + byte_off;

		for (j = 0; j < FONT.f_height; j++) {

			l = *(ulong *)q;

			if (i == 0x37)
				fprintf(stderr, "%08lx %3d %3d %3d %3d ",
				l, bit_off, width, space,
				kern);
			

			l <<= bit_off;
			l >>= 32 - width;
			l <<=  space - (width + kern);

#if 0
			l >>=  32 - (bit_off + width); 
			l <<=  space - (width + kern);
			l &= ~0 >> (32 - (space - kern));
#endif

			if (i == 0x37)
				fprintf(stderr, "%lx\n", l);


			*(ulong *)p = l;

			p += bpl;
			q += TF.tf_Modulo;
		}

	}

	FONT.f_length = OFF(dataend);
}



