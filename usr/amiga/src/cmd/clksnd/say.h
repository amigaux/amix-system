#ifdef TESTING
#define	SNDSFILE	"clksnds.as"
#else /* !TESTING */
#define	SNDSFILE	"/usr/amiga/lib/audio/clksnds.as"
#endif /* !TESTING */

#define	BLOUD	63
#define	BPER	448

extern char *progname;

void loadsnds(void);
void say(char *);
void saynumber(int);

