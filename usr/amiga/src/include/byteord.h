/*
 * The following macro's are used to convert between shorts, as
 * they are stored in memory, and as they appear in plot file
 * format.
 * Fbyte and Sbyte give the first and second bytes (in a plot file)
 * of their short argument.
 * Bshort gives the short made out of the given first and second
 * plot file bytes.
 */
#define	fbyte(s)	((s) & 0xFF)
#define	sbyte(s)	fbyte((s) >> bitsof(uchar))
#define	bshort(f, s)	((f) | (s) << 8)
