#define	DEFAULT_FONT	"/usr/amiga/lib/font/newtopaz11"

extern struct font *openfont(char *);
extern void scrprintfat(struct Bitmap *bp,
		       struct font *,
		       int,
		       int,
		       char *,
		       ...);
extern void scrputstrat(struct Bitmap *bp,
		       struct font *,
		       int,
		       int,
		       register char *);
