#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>

extern char *Malloc(), *Realloc(), *Calloc();

/* #include <exec/types.h> */
typedef unsigned long ULONG;

#include "load.h"

#define Vprintf if (0) (void)


static int fskip(fp, offset)
FILE *fp;
long offset;
{
    while (--offset>=0)
	if (getc(fp)==EOF)
	    return ferror(fp) ? -1 : 0;
    return 1;
}


#define ABORT goto errexit

ULONG *adosload(fp, name)
FILE *fp;
char *name;
{
    ULONG **hunktab;
    register i;
    int len, curhunk, numhunks;
    int name_len=20;
    char *name_buf = Malloc(20);
    ULONG hunktype, count, first, last, offset;
    static char *segtypes[] =
    {
	"code", "data", "bss",
    };

    while ((len=fread(&hunktype, sizeof hunktype, 1, fp))==1)
	switch (hunktype)
	{
	    ULONG tmp;
	case hunk_unit:
	    if ((len=fread(&count, sizeof count, 1, fp))!=1)
		ABORT;
	    count /= 4;
	    if (count>name_len)
		name_buf = Realloc(name_buf, name_len=count);
	    if (fread(name_buf, (int)count, 1, fp)!=1)
		ABORT;
	    Vprintf("hunk_unit: `%.*s'\n", (int)count, name_buf);
	    break;
	case hunk_header:
	    Vprintf("hunk_header:\n");
	    while (1)
	    {
		if ((len=fread(&count, sizeof count, 1, fp))!=1)
		    ABORT;
		if (count==0L)
		    break;
		count *= 4;
		if (count>name_len)
		    name_buf = Realloc(name_buf, name_len=count);
		if (fread(name_buf, (int)count, 1, fp)!=1)
		    ABORT;
		Vprintf("\tlibname: `%.*s'\n", (int)count, name_buf);
	    }
	    if ((len=fread(&numhunks , sizeof numhunks, 1, fp))!=1)
		ABORT;
	    Vprintf("\tHunk count: %ld\n", numhunks);
	    hunktab = (ULONG **)Calloc(numhunks, sizeof (ULONG));
	    if ((len=fread(&first, sizeof first, 1, fp))!=1)
		ABORT;
	    Vprintf("\tFirst hunk: %ld\n", first);
	    curhunk = first;
	    if ((len=fread(&last, sizeof last, 1, fp))!=1)
		ABORT;
	    Vprintf("\tLast hunk: %ld\n", last);
	    for ( i=first ; i<=last ; ++i )
	    {
		if ((len=fread(&count, sizeof count, 1, fp))!=1)
		    ABORT;
		tmp = count & 0xC0000000;
		count &= 0x3FFFFFFF;
		Vprintf("\tHunk %d size: 0x%06lx bytes in ", i, count*4);
		switch (tmp)
		{
		case 0x80000000:
		    Vprintf("FAST");
		    break;
		case 0x40000000:
		    Vprintf("CHIP");
		    break;
		default:
		    Vprintf("ANY");
		    break;
		}
		Vprintf(" memory\n");
		hunktab[i] = (ULONG *)Calloc(count+2, 4);
		hunktab[i][0] = count;
		if (i>0)
		    hunktab[i-1][1] = (ULONG)(&hunktab[i][1]);
	    }
	    break;
	case hunk_code:
	case hunk_data:
	case hunk_bss:
	    if ((len=fread(&count, sizeof count, 1, fp))!=1)
		ABORT;
	    tmp = count & 0xC0000000;
	    count &= 0x3FFFFFFF;
	    Vprintf("hunk_%s(%d): Length: 0x%06lx bytes in ",
		   segtypes[hunktype-hunk_code], curhunk, count*4);
	    switch (tmp)
	    {
	    case 0x80000000:
		Vprintf("FAST");
		break;
	    case 0x40000000:
		Vprintf("CHIP");
		break;
	    default:
		Vprintf("ANY");
		break;
	    }
	    Vprintf(" memory\n");

	    if (hunktype != hunk_bss && count)
		if ((len=fread(hunktab[curhunk]+2, count*4, 1, fp))!=1)
		    ABORT;
	    break;
	case hunk_reloc32:
	    Vprintf("hunk_reloc(32):\n");
	    while (1)
	    {
		ULONG *addr;
		if ((len=fread(&count, sizeof count, 1, fp))!=1)
		    ABORT;
		if (count==0L)
		    break;
		i = count;
		if ((len=fread(&count, sizeof count, 1, fp))!=1)
		    ABORT;
		Vprintf("\tHunk #%ld:\n", count);
		while (i>0)
		{
		    if ((len=fread(&offset, sizeof offset, 1, fp))!=1)
			ABORT;
		    Vprintf("\t\t0x%06lx\n", offset);
		    addr = (ULONG *)((unsigned char *)(&hunktab[curhunk][2])
				     + offset);
		    *addr += (ULONG)(hunktab[count]+2);
		    --i;
		}
	    }
	    break;
	case hunk_end:
	    Vprintf("hunk_end\n");
	    ++curhunk;
	    break;
	case hunk_reloc16:
	case hunk_reloc8:
	case hunk_name:
	case hunk_ext:
	case hunk_symbol:
	case hunk_debug:
	case hunk_overlay:
	case hunk_break:
	default:
	    fprintf(stderr, "adosload: Invalid hunk type 0x%08lX in %s\n",
		    hunktype, name);
	    return 0;
	}

    if (!ferror(stdin))
	return hunktab[0]+1;

errexit:
    if (len==0)
    {
	fprintf(stderr, "adosload: unexpected EOF in %06lx hunk in %s\n",
		hunktype, name);
    }
    else
    {
	char buf[BUFSIZ];
	sprintf(buf, "adosload: read error in %06lx hunk in %s",
		hunktype, name);
	perror(buf);
    }
    return 0;
}


ULONG *LoadSeg(name)
char *name;
{
    FILE *fp;
    ULONG *seg;

    if ( (fp=fopen(name, "r")) == NULL )
	return (ULONG *)0;
    seg = adosload(fp, name);
    fclose(fp);
    return seg;
}
