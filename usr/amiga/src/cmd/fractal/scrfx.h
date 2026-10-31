#define putbyte(BITMAP, INDEX, MASK, COLOR)			    \
{								    \
    int I, COL=COLOR;						    \
    for (I=0; I < BITMAP->depth; ++I, COL >>= 1)		    \
    {								    \
	if (COLOR == -1)					    \
	{							    \
	    BITMAP->bpl[I][(INDEX)] ^= (MASK);			    \
	}							    \
	else							    \
	{							    \
	    if ((COL)&1)					    \
		BITMAP->bpl[I][(INDEX)] |= (MASK);		    \
	    else						    \
		BITMAP->bpl[I][(INDEX)] &= ~(MASK);		    \
	}							    \
    }								    \
}

extern void line(struct Bitmap *, int, int, int, int, int);
extern void point(struct Bitmap *, int, int, int);
extern void box(struct Bitmap *, int, int, int, int, int);
