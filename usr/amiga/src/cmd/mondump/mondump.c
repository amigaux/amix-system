/*
 *	Program to dump a "mon.out" file, produced by the standard
 *	monitor(3C) library routine and expected by the prof(1) utility.
 */

#include <stdio.h>
#include <mon.h>
#include <sys/stat.h>

static struct hdr head;			/* Defined in <mon.h> */

static long fsize (fd)
{
    struct stat sbuf;

    if (fstat (fd, &sbuf) < 0) {
	perror ("stat failed");
    }
    return (sbuf.st_size);
}

main (argc, argv)
int argc;
char *argv[];
{
    FILE *fp;
    struct cnt *calls;
    int filesize;
    int buckets;
    int profdatasize;
    int barsize;
    WORD *profdata;
    int i;
    int hits = 0;
    int regionsize;
    unsigned int pclow;	
    unsigned int pchigh;
    int limit;
    extern char *malloc ();

    /*
     *  Open the specified profiling data file and determine it's
     *  size.  We use the size value to figure out how big the profile
     *  buffer was.  BIG KLUDGE!  This should have been in the header
     *  so someday we might be able to tack other information on to the
     *  end of the file.
     */

    printf ("Dump of profiling information from '%s':\n", argv[1]);
    fp = fopen (argv[1], "r");
    filesize = fsize (fileno (fp));
    printf ("Filesize (bytes): %#x (%d)\n", filesize, filesize);

    /*
     *	Read and print the header information.
     */

    printf ("Header info, %d bytes at file offset %#x (%d)\n",
	    sizeof (struct hdr), ftell (fp), ftell (fp));
    if (fread ((char *) &head, sizeof (struct hdr), 1, fp) != 1) {
	perror (argv[1]);
	exit (1);
    }
    printf ("head.lpc (Low PC value) = %#x\n", head.lpc);
    printf ("head.hpc (High PC value)= %#x\n", head.hpc);
    printf ("head.nfns (Number of function call counts) = %#x (%d)\n",
		head.nfns, head.nfns);
    regionsize = head.hpc - head.lpc;
    printf ("Profiled region size %#x (%d) bytes\n", regionsize, regionsize);

    /*
     *	Read and print the call count information.  Each call count is
     *  a PC value which represents the PC of the instruction that _mcount
     *	returned to in the function that called _mcount, followed by a count
     *	of the number of times that the function was called.
     */

    printf ("Function call counter values, %d bytes at file offset %#x (%d)\n",
	    sizeof (struct cnt) * head.nfns, ftell (fp), ftell (fp));
    calls = (struct cnt *) malloc (head.nfns * sizeof (struct cnt));
    if (fread ((char *) calls, sizeof (struct cnt), head.nfns, fp)
	!= head.nfns) {
	perror (argv[1]);
	exit (1);
    }
    for (limit = 0; limit < head.nfns; limit++) {
	if (calls[limit].fnpc == 0) {
	    break;
	}
    }
    printf ("Dump of %d nonzero call counters (%d total):\n", limit,
							 head.nfns);
    for (i = 0; i < limit; i++) {
	if ((i % 4) == 0) {
	    printf ("\n");
	}
	printf ("%6d @ 0x%8.8x", calls[i].mcnt, calls[i].fnpc);
    }
    printf ("\n\n");

    /*
     *  Read and print the PC counters.
     *
     *  By definition, the bucket size will always be an integral value,
     *  and it will be the smallest integer such that the bucket size
     *  times the number of buckets will cover ALL the specified profiling
     *  range.  This relationship is inherent in the computations used to
     *  compute the buffer size passed to monitor(3C).
     *
     *  Since we can find the number of buckets from the profile data size
     *  and we know the profiling range (highpc - lowpc), we can compute a
     *  test value for the bucket size.  If it comes out too small, we
     *  know that the range is not an integral number of buckets in
     *  length, and we need to add one to the size of each bucket, since
     *  the number of them is fixed.
     *
     *  Once we have the bucket size, the scale is computed trivially
     *  from the relationship specified in profil(2):
     *
     *	scale / 2**16  =  sizeof(short) / barsize
     *
     *  Note that the manual page lies slightly when it says that
     *  a scale of 0xFFFF maps 2 bytes to each counter.  The correct
     *  scale is 0x10000, which the kernel is perfectly happy to use
     *  by the way.  In fact, the kernel will use any scale specified,
     *  which would be useful if instructions started on odd byte
     *  boundaries (I.E. a scale of 0x20000 would map one instruction
     *  byte to one profiling bucket).
     *
     */
    
    profdatasize = filesize - sizeof (head) - head.nfns * sizeof (struct cnt);
    printf ("Profiling data, %d bytes at file offset %#x (%d)\n",
	    profdatasize, ftell (fp), ftell (fp));

    buckets = profdatasize / sizeof (WORD);
    printf ("Found %#x (%d) profiling buckets\n", buckets, buckets);

    barsize = regionsize / buckets;
    if ((barsize * profdatasize) < regionsize) {
	barsize++;
    }
    printf ("Found bucket size %#x (%d)\n", barsize, barsize);

    profdata = (WORD *) malloc (buckets * sizeof (WORD));
    if (fread ((char *) profdata, sizeof (WORD), buckets, fp) != buckets) {
	perror (argv[1]);
	exit (1);
    }

    printf ("Dump of nonzero buckets (%d buckets total):\n", buckets);
    for (i = 0; i < buckets; i++) {
	pclow = (int) head.lpc + (i * barsize);
	pchigh = pclow + barsize - 1;
	if (profdata[i] > 0) {
	    printf ("\nbucket[%d]: PC %8x-%8x: %4d hits", i, pclow, pchigh,
					 profdata[i]);
	}
	hits += profdata[i];
    }
    printf ("\n\nHits: %d\n", hits);
}


