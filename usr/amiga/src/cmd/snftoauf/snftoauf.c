#

#include	<stdio.h>
#include	<stdarg.h>
#include	<sys/types.h>
#include	<osfcn.h>
#include	<sys/stat.h>
#include	<stdlib.h>
#include	<sys/screen.h>

#include	"Xproto.h"
#include	"fontstruct.h"
#include	"snfstruct.h"
#include	"font.h"

#define	ERROR	(-1)

FontInfoRec	*snfip;
struct	font	aufi;
struct	font	*aufip = &aufi;

struct AUFDATA {
	char *adata; 
	int asize;
	struct AUFDATA *nxt;
};

struct	AUFDATA	*aufbase = NULL;
struct	AUFDATA	*auflast;

short	dataoffset	= sizeof aufi;

void
ferr(char *fmt,...) {
	va_list ap;
	char strbuf[256];

	va_start(ap,fmt);
	(void)vsprintf(strbuf,fmt,ap);
	va_end(ap);
	fprintf(stderr,"ERROR -- %s!\n",strbuf);
	exit(1);
}


void *
getmem(register int s) {
	void *mp;

	if ((mp = malloc(s)) == NULL)
		ferr("Can't allocate %d bytes of memory",s);
	return(mp);
}


void
readsnffont(register char *fn) {
	int snffd;
	struct stat sbuf;

	if ((snffd = open(fn,0)) == ERROR)
		ferr("Can't open snf font file: %s",fn);
	if (fstat(snffd,&sbuf) == ERROR)
		ferr("Can't stat snf font file: %s",fn);
	snfip = getmem(sbuf.st_size);
	if (read(snffd,snfip,sbuf.st_size) != sbuf.st_size)
		ferr("Read error on snf font file: %s",fn);
	close(snffd);
}


#ifdef	unusedcode
#else
void
displaysnfchar(int c) {
	register CharInfoRec *cip;
	int i,j;
	char *bp1,*bp2;
	int mask;

	cip = ADDRCharInfoRec(snfip) + (c - snfip->chFirst);
	printf("c=0x%x, byteoff=%d, exits=%d, cw=%d, rsb=%d, lsb=%d, as=%d, ds=%d\n",
			c,
			cip->byteOffset,cip->exists,
			cip->metrics.characterWidth,
			cip->metrics.rightSideBearing,
			cip->metrics.leftSideBearing,
			cip->metrics.ascent,
			cip->metrics.descent);
	bp1 = ADDRCHARGLYPHS(snfip) + cip->byteOffset;
	for (i=0;i<GLHEIGHTPIXELS(cip);i++) {
		bp2 = bp1;
		mask = 0x80;
		for (j=0;j<GLWIDTHPIXELS(cip);j++) {
			if (*bp2&mask)
				putchar('X');
			else
				putchar('.');
			if (mask == 0x1) {
				mask = 0x80;
				bp2++;
			} else
				mask >>= 1;
		}
		bp1 += GLYPHWIDTHBYTESPADDED(cip);
		putchar('\n');
	}
}
#endif

#ifdef	DEBUG
void
displayaufchar(int c) {
	register struct charent  *cip;
	int i,j;
	char *bp1,*bp2;
	int mask;

	cip = &(aufip->f_ctable[c]);
	printf("c=0x%x, height=%d, width=%d, basel=%d, move=%d, bpl=%d, off=%d\n",
			c,
			cip->ce_height,cip->ce_width,
			cip->ce_basel,cip->ce_move,
			cip->ce_bpl,cip->ce_data);
	bp1 = (char *)aufip + cip->ce_data;
	for (i=0;i<cip->ce_height;i++) {
		bp2 = bp1 + cip->ce_bpl - ((cip->ce_width-1)>>3) - 1;
		mask = 0x1 << ((cip->ce_width-1)&0x7);
		for (j=0;j<cip->ce_width;j++) {
			if (*bp2&mask)
				putchar('X');
			else
				putchar('.');
			if (mask == 0x1) {
				mask = 0x80;
				bp2++;
			} else
				mask >>= 1;
		}
		bp1 += cip->ce_bpl;
		putchar('\n');
	}
}

void
displayauffont() {
	register int i;

	for (i=1;i<255;i++)
		displayaufchar(i);
}
#endif


void
verifyconvertability() {

	if (snfip->pixDepth != 1)
		ferr("Can't convert fonts of pixDepth != 1, this has %d",snfip->pixDepth);
}

void
addtoaufdata(register char *d,int s) {
	register struct AUFDATA *adp;

	adp = getmem(sizeof *adp);
	if (aufbase == NULL)
		aufbase = adp;
	else
		auflast->nxt = adp;
	adp->nxt = NULL;
	adp->adata = d;
	adp->asize = s;
	auflast = adp;
}


void
convertchar(int c) {
	register CharInfoRec *scip;
	struct charent *acip;
	int i,j;
	char *sbp1,*sbp2;
	char *abp1,*abp2;
	int smask,amask;

	scip = ADDRCharInfoRec(snfip) + (c - snfip->chFirst);
	acip = &(aufip->f_ctable[c]);

	if (!(scip->exists))
		return;
	if (GLWIDTHPIXELS(scip) > 32)
		fprintf(stderr,"Warning, character 0x%x is %d pixels wide\n",c,GLWIDTHPIXELS(scip));
	
	acip->ce_height = GLHEIGHTPIXELS(&snfip->maxbounds);
	acip->ce_width = scip->metrics.characterWidth + 1;
	acip->ce_basel = snfip->maxbounds.metrics.ascent;
	acip->ce_move = acip->ce_width;
	if (acip->ce_width <= 8)
		acip->ce_bpl = 1;
	else if (acip->ce_width <= 16)
		acip->ce_bpl = 2;
	else if (acip->ce_width <= 32)
		acip->ce_bpl = 4;
	else if (acip->ce_width <= 64)
		acip->ce_bpl = 8;
	else if (acip->ce_width <= 128)
		acip->ce_bpl = 16;
	abp1 = getmem(acip->ce_height * acip->ce_bpl);
	memset(abp1,0,acip->ce_height * acip->ce_bpl);
	addtoaufdata(abp1,acip->ce_height * acip->ce_bpl);
	abp1 += acip->ce_bpl * (acip->ce_basel - scip->metrics.ascent);
	acip->ce_data = dataoffset;
	dataoffset += acip->ce_height * acip->ce_bpl;
	
	sbp1 = ADDRCHARGLYPHS(snfip) + scip->byteOffset;
	for (i=0;i<GLHEIGHTPIXELS(scip);i++) {
		abp2 = abp1 + acip->ce_bpl - ((acip->ce_width-scip->metrics.leftSideBearing-1)>>3) - 1;
		sbp2 = sbp1;
		smask = 0x80;
		amask = 0x1 << ((acip->ce_width-scip->metrics.leftSideBearing-1)&0x7);
		for (j=0;j<GLWIDTHPIXELS(scip);j++) {
			if (*sbp2&smask)
				*abp2 |= amask;
			if (smask == 0x1) {
				smask = 0x80;
				sbp2++;
			} else
				smask >>= 1;
			if (amask == 0x1) {
				amask = 0x80;
				abp2++;
			} else
				amask >>= 1;
		}
		sbp1 += GLYPHWIDTHBYTESPADDED(scip);
		abp1 += acip->ce_bpl;
	}
}


void
buildauffont() {
	register int c;

	aufip->f_magic = F_MAGIC;
	aufip->f_flags = 0;
	aufip->f_count = 0;

	for (c=snfip->chFirst;c<=snfip->chLast;c++)
		convertchar(c);
	aufip->f_length = dataoffset;
		/* We'll consider 'M' to be a "typical" character */
	aufip->f_height = aufip->f_ctable['M'].ce_height;
	aufip->f_width = aufip->f_ctable['M'].ce_width;
	aufip->f_baseline = snfip->maxbounds.metrics.ascent;
}


void
writeauffont(register char *fn) {
	int afd;
	register struct AUFDATA *aup;

	if ((afd = creat(fn,0666)) == ERROR)
		ferr("Can't creat unix font file: %s",fn);
	if (write(afd,aufip,sizeof *aufip) != sizeof *aufip)
		ferr("Write error on unix font file: %s",fn);
	
	for (aup=aufbase;aup != NULL;aup=aup->nxt)
		if (write(afd,aup->adata,aup->asize) != aup->asize)
			ferr("Write error on unix font file: %s",fn);
	close(afd);
}

#ifdef	DEBUG
void
openfont(char *fn) {
	register int ffd;
	struct stat sbuf;

	if ((ffd = open(fn,0)) == ERROR)
		ferr("openfont");
	if (fstat(ffd,&sbuf) == ERROR)
		ferr("openfont");
	if ((aufip = malloc(sbuf.st_size)) == NULL)
		ferr("openfont");
	if (read(ffd,aufip,sbuf.st_size) != sbuf.st_size)
		ferr("openfont");
	close(ffd);
}
#endif

int
main(int argc,char **argv) {

	if (argc != 3) {
		fprintf(stderr,"Usage: %s snffontfile auffontfile\n",*argv);
		exit(1);
	}
	readsnffont(*++argv);
	verifyconvertability();
	buildauffont();
	writeauffont(*++argv);
#ifdef	DEBUG
	openfont(*argv);
	displayauffont();
#endif
}
