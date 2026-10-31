/*
 * A program to set the colours.
 */
#include <stdio.h>
#include <sys/screen.h>


/*
 * A useful function.
 */
#define	nel(s)	(sizeof(s)/sizeof(*s))


/*
 * Some gray scales.
 */
char *monotab[] ={
	"00 black",
	"2a gray1",
	"55 gray2",
	"80 gray3",
	"aa gray4",
	"d5 gray5",
	"ff white",
};


/*
 * Some colours.
 */
char *colotab[] ={
	"000000 black",
	"0000ff blue",
	"00ff00 green",
	"00ffff cyan",
	"ff0000 red",
	"ff00ff magenta",
	"ffff00 yellow",
	"ffffff white",
};


/*
 * The various modes.
 */
char *modetab[] ={
	"show     d;",
	"showall  g;",
	"over     o;",

	"bc       a; s00;",
	"fc       a; s01;",

	"0        a; s00;",
	"1        a; s01;",

	"consbac  a; s00;",
	"consfor  a; s01;",
        "consdef  m00=B70; m01=344;",

        "shadcol  a; s01;",
        "shadsel  a; s05;",
        "windext  a; s00;",
        "windsel  a; s07;",
        "windbac  a; s03;",
        "windfor  a; s02; s06;",
        "menucol  a; s17; s21; s25; s29;",
        "iconcol  a; s18; s22; s26; s30;",
        "iconhil  a; s19; s23; s27; s31;",

	"amixwin  m00=000; m01=630; m02=344; m03=B70;\
	          m04=000; m05=404; m06=344; m07=A76;\
	          m08=000; m09=000; m10=000; m11=000;\
	          m12=000; m13=000; m14=000; m15=000;\
	          m16=000; m17=865; m18=A00; m19=000;\
	          m20=000; m21=865; m22=A00; m23=000;\
	          m24=000; m25=865; m26=A00; m27=000;\
	          m28=000; m29=865; m30=A00; m31=000;",

	"2024win  m00=000; m01=009; m02=000; m03=880;\
	          m04=000; m05=009; m06=000; m07=009;\
	          m08=000; m09=000; m10=000; m11=000;\
	          m12=000; m13=000; m14=000; m15=000;\
	          m16=000; m17=009; m18=889; m19=000;\
	          m20=000; m21=009; m22=889; m23=000;\
	          m24=000; m25=009; m26=889; m27=000;\
	          m28=000; m29=009; m30=889; m31=000;",

	"c00      a; s00;",
	"c01      a; s01;",
	"c02      a; s02;",
	"c03      a; s03;",
	"c04      a; s04;",
	"c05      a; s05;",
	"c06      a; s06;",
	"c07      a; s07;",
	"c08      a; s08;",
	"c09      a; s09;",
	"c10      a; s10;",
	"c11      a; s11;",
	"c12      a; s12;",
	"c13      a; s13;",
	"c14      a; s14;",
	"c15      a; s15;",
	"c16      a; s16;",
	"c17      a; s17;",
	"c18      a; s18;",
	"c19      a; s19;",
	"c20      a; s20;",
	"c21      a; s21;",
	"c22      a; s22;",
	"c23      a; s23;",
	"c24      a; s24;",
	"c25      a; s25;",
	"c26      a; s26;",
	"c27      a; s27;",
	"c28      a; s28;",
	"c29      a; s29;",
	"c30      a; s30;",
	"c31      a; s31;",
};


main(argc, argv)
char *argv[];
{
	char		*cp;
	char		*ap;
	int		in;
	int		mf;
	int		of;
	unsigned short	cd;
	unsigned short	cn;
	unsigned short	cu;
	unsigned long	ca;
	unsigned long	cv;
	struct scolor	cm[32];
	char		*lookup();

	memset(cm, 0, sizeof(cm));
	if (ioctl(0, SIOCGETCMAP, cm) < 0)
		panic("ioctl SIOCGETCMAP failed");

	if ((cd=ioctl(0, SIOCDISPLAYTYPE)) < 0)
		cd = 1;
	else
		cd = (cd&(DT_MONOCHROME|DT_HEDLEY)) ? 0 : 1;

	if (argc == 1) {
		cprint(cm, cd);
		exit(0);
	}

	mf = 0;
	of = 0;
	cu = 0;
	ca = 0;
	for (in=1; in<argc; ++in) {
		ap = argv[in];
		if (*ap != '-')
			usage();
		if ((cp=lookup(&ap[1])) == NULL)
			panic("Unknown option: %s", ap);
		while (*cp != '\0') {
			while (*cp==' ' || *cp=='\t')
				++cp;
			switch (*cp++) {
			case 'a':	/* Extract next argument */
				if (++in >= argc)
					usage();
				change(argv[in], cd, &ca, &cu);
				break;
			case 'd':	/* Display */
				cprint(cm, cd);
				break;
			case 'g':	/* Display all */
				aprint(cm);
				break;
			case 'm':	/* Set colour to constant */
				cn = scand2(ap, &cp);
				if (*cp++ != '=')
					error(ap);
				if (scanhn(&cv, cp, 3) == 0)
					error(ap);
				cp += 3;

				setcol(&cm[cn], 0x7, ((cv&0xF00)<<12)|
						     ((cv&0x0F0)<< 8)|
						     ((cv&0x00F)<< 4));
				mf = 1;
				break;
			case 'o':	/* Set override option */
				of = 1;
				break;
			case 's':	/* Set colour to argument */
				cn = scand2(ap, &cp);
				setcol(&cm[cn], cu, ca);
				mf = 1;
				break;
			default:
				error(ap);
			}
			if (*cp++ != ';')
				error(ap);
		}
	}

	/* Just for Keith */
	if (of == 0)
		cblind(cm, cd);

	if (mf != 0) {
		if (ioctl(0, SIOCSETCMAP, cm) < 0)
			panic("ioctl SIOCSETCMAP failed");
	}
}


/*
 * Look up a argument in our table.
 */
char *
lookup(ap)
char *ap;
{
	register char	*bp;
	register char	*mp;
	int		oi;

	for (oi=0; oi<nel(modetab); ++oi) {
		for (bp=ap, mp=modetab[oi];; ++bp, ++mp) {
			if (*bp=='\0' && *mp==' ')
				return (mp);
			if (*bp != *mp)
				break;
			if (*bp=='\0' || *mp==' ')
				break;
		}
	}
	return (NULL);
}


/*
 * Scan a two digit decimal number.
 */
scand2(ap, pp)
char *ap;
char **pp;
{
	register char	*p;
	register int	i;
	int		d;
	int		n;

	n = 0;
	p = *pp;
	for (i=0; i<2; ++i) {
		if (*p>='0' && *p<='9')
			d = *p++ - '0';
		else
			error(ap);
		n = (n*10) + d;
	}
	*pp = p;
	return (n);
}


/*
 * Scan a hexadecimal number as a colour value.
 */
scanhn(rp, cp, nd)
unsigned long	*rp;
char		*cp;
int		nd;
{
	int		i;
	int		d;
	unsigned long	rv;

	rv = 0;
	for (i=0; i<nd; ++i) {
		if (*cp>='0' && *cp<='9')
			d = *cp++ - '0';
		else if (*cp>='a' && *cp<='f')
			d = *cp++ - 'a' + 10;
		else if (*cp>='A' && *cp<='F')
			d = *cp++ - 'A' + 10;
		else
			return (0);
		rv = (rv<<4) | d;
	}

	*rp = rv;
	return (1);
}


/*
 * Change a colour into its rgb equivalent.
 */
change(cp, cd, rp, up)
char		*cp;
unsigned short	cd;
unsigned long	*rp;
unsigned short	*up;
{
	char		*np;
	int		ni;
	int		ln;
	unsigned long	nv;

	if (cd != 0) {
		for (ni=0; ni<nel(colotab); ++ni) {
			np = colotab[ni];
			if (strcmp(&np[7], cp) == 0) {
				if (scanhn(rp, np, 6) == 0)
					error(cp);
				*up = 0x7;
				return;
			}
		}
	} else {
		for (ni=0; ni<nel(monotab); ++ni) {
			np = monotab[ni];
			if (strcmp(&np[3], cp) == 0) {
				if (scanhn(&nv, np, 2) == 0)
					error(cp);
				*rp = nv << 24;
				*up = 0x8;
				return;
			}
		}
	}

	ln = strlen(cp);
	if (scanhn(&nv, cp, ln) == 0)
		panic("Bad color value %s", cp);

	switch (ln) {
	case 0:
		*rp = 0x0;
		*up = 0x0;
		return;
	case 1:
		*rp = (nv&0xF) << 28;
		*up = 0x8;
		return;
	case 2:
		*rp = (nv&0xFF) << 24;
		*up = 0x8;
		return;
	case 3:
		*rp = ((nv&0xF00)<<12) | ((nv&0x0F0)<<8) | ((nv&0x00F)<<4);
		*up = 0x7;
		return;
	case 4:
		*rp = ((nv&0xF000)<<16) | ((nv&0x0F00)<<12)
		    | ((nv&0x00F0)<< 8) | ((nv&0x000F)<< 4);
		*up = 0xF;
		return;
	case 5:
		*rp = ((nv&0xFFF00)<<12)|((nv&0x000F0)<<8)|((nv&0x0000F)<<4);
		*up = 0xF;
		return;
	case 6:
		*rp = nv&0xFFFFFF;
		*up = 0x7;
		return;
	case 7:
		*rp = ((nv&0xF000000)<<4) | (nv&0x0FFFFFF);
		*up = 0xF;
		return;
	case 8:
		*rp = nv;
		*up = 0xF;
		return;
	default:
		panic("Bad color value %s", cp);
	}
}


/*
 * Set the given colour number in the colour map to the given value.
 */
setcol(cp, cu, cv)
struct scolor	*cp;
unsigned short	cu;
unsigned long	cv;
{
	if ((cu&8) != 0)
		cp->gray  = ((cv>>24)&0xFF);
	if ((cu&4) != 0)
		cp->red   = ((cv>>16)&0xFF);
	if ((cu&2) != 0)
		cp->green = ((cv>> 8)&0xFF);
	if ((cu&1) != 0)
		cp->blue  = ((cv    )&0xFF);
}


/*
 * Make sure the foreground and background colours are different.
 */
cblind(cp, cd)
struct scolor	*cp;
unsigned short	cd;
{
	if (cd != 0) {
		if (cp[0].red   != cp[1].red
		||  cp[0].green != cp[1].green
		||  cp[0].blue  != cp[1].blue)
			return;
	} else {
		if (cp[0].gray != cp[1].gray)
			return;
	}
	panic("Background and foreground should be different");
}


/*
 * Display the entire colour map.
 */
aprint(cp)
struct scolor *cp;
{
	int i;

	for (i=0; i<32; i++) {
		printf("c%02d=[%02x]%02x%02x%02x  ", i,
			cp[i].gray, cp[i].red, cp[i].green, cp[i].blue);
		if ((i%4) == 3)
			printf("\n");
	}
}


/*
 * Display the particular colours relevant for our screen.
 */
cprint(cp, cd)
struct scolor	*cp;
unsigned short	cd;
{
	int	ci;
	int	nc;
	char	*pickit();

	if (cd != 0)
		printf("color monitor\n");
	else
		printf("monochrome monitor\n");

	printf("background: %s\n", pickit(&cp[0], cd));
	printf("foreground: %s\n", pickit(&cp[1], cd));

	if ((nc=ioctl(0, SIOCGETNCOLREGS)) < 0)
		nc = 2;
	for (ci=2; ci<nc; ci++)
		printf("color%02d:    %s\n", ci, pickit(&cp[ci], cd));
}


/*
 * Find the name of the given colour.
 */
char *
pickit(cp, cd)
struct scolor	*cp;
unsigned short	cd;
{
	int		ni;
	unsigned long	cv;
	unsigned long	nv;
	static char	buf[32];

	sprintf(buf, "[%x]%x%x%x",
		cp->gray>>4, cp->red>>4, cp->green>>4, cp->blue>>4);

	if (cd == 0)
		sprintf(&buf[6], " (%s)", &monotab[(unsigned int)cp->gray*7/256][3]);
	else {
		cv = ((cp->red<<16)|(cp->green<<8)|(cp->blue)) & 0x00C0C0C0;
		for (ni=0; ni<nel(colotab); ++ni) {
			if (scanhn(&nv, colotab[ni], 6) == 0)
				error("pickit");
			if ((nv&0x00C0C0C0) == cv) {
				sprintf(&buf[6], " (%s)", &colotab[ni][7]);
				break;
			}
		}
	}
	return (buf);
}


/*
 * Print out a usage message and exit.
 */
usage()
{
	panic("Usage: color [-fc color] [-bc color]");
}


/*
 * An internal error has occurred.
 */
error(ap)
{
	panic("Internal error: %s", ap);
}


/*
 * Print out an error message and exit.
 */
panic(a0, a1, a2, a3, a4, a5, a6, a7)
char	*a0;
int	a1;
int	a2;
int	a3;
int	a4;
int	a5;
int	a6;
int	a7;
{
	fprintf(stderr, a0, a1, a2, a3, a4, a5, a6, a7);
	fprintf(stderr, "\n");
	exit(1);
}
