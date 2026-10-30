
/* The disk "environment" is a longword array that describes the
** disk geometry.  It is variable sized, with the length at the beginning.
** Here are the constants for a standard geometry.
*/

/* these are the offsets into the array */

#define DE_TABLESIZE	0	/* standard value is 11 */
#define DE_SIZEBLOCK	1	/* in longwords: standard value is 128 */
#define DE_SECORG	2	/* not used; must be 0 */
#define DE_NUMHEADS	3	/* # of heads (surfaces). drive specific */
#define DE_SECSPERBLK	4	/* not used; must be 1 */
#define DE_BLKSPERTRACK 5	/* blocks per track. drive specific */
#define DE_RESERVEDBLKS 6	/* unavailable blocks at start.	 usually 2 */
#define DE_PREFAC	7	/* not used; must be 0 */
#define DE_INTERLEAVE	8	/* usually 0 */
#define DE_LOWCYL	9	/* starting cylinder. typically 0 */
#define DE_UPPERCYL	10	/* max cylinder.  drive specific */
#define DE_NUMBUFFERS	11	/* starting # of buffers.  typically 5 */
#define DE_MEMBUFTYPE	12	/* type of mem to allocate for buffers. */
#define DE_BUFMEMTYPE	12	/* same as above, better name
				 * 1 is public, 3 is chip, 5 is fast */
#define DE_MAXTRANSFER	13	/* Max number bytes to transfer at a time */
#define DE_MASK		14	/* Address Mask to block out certain memory */
#define DE_BOOTPRI	15	/* Boot priority for autoboot */
#define DE_DOSTYPE	16	/* ASCII (HEX) string showing filesystem type;
				 * 0X444F5300 is old filesystem,
				 * 0X444F5301 is fast file system */
#define DE_BAUD		17	/* Baud rate for serial handler */
#define DE_CONTROL	18	/* Control word for handler/filesystem */
#define DE_BOOTBLOCKS	19	/* Number of blocks containing boot code */

