#define B_end		(0)		/* end of table */
#define B_bitfield	(1)		/* display `|FOO' */
#define B_bitnum	(2)		/* display `FOO=%d'*/
#define B_bitval	(3)		/* display `FOO=0x%x' */
#define B_type		(4)		/* display `FOO' */

struct Bitfields
{
    int flags;
    int value;
    char *name;
};

extern char *compilebitfield(int, struct Bitfields *);


