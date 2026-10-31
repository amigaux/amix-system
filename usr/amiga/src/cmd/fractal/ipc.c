#include <sys/types.h>
#include <sys/mman.h>
#include <amiga/screen.h>
#include <memory.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "fractal.h"

int
grab_shared_header_memory(struct fractal_set **fractal_set) 
{
    int fd;
    caddr_t addr;

    if ((fd = open("/dev/zero", O_RDWR)) < 0)
    {
	(void) fprintf(stderr, "%s: Couldn't open /dev/zero to map "
		       "shared memory (%s)\n", progname, strerror(errno));

	return -1;
    }

    addr = mmap(0, sizeof(**fractal_set), PROT_READ|PROT_WRITE, MAP_SHARED,
		fd, 0);

    while (close(fd) < 0 && errno == EINTR)
	;

    if (addr == (caddr_t)-1)
	return -1;			/* mmap failed, darn. */

    *fractal_set = (struct fractal_set *)addr;

    return 0;				/* Everything is Ok. */
}

int
grab_shared_image_memory(struct fractal_set *fractal_set) 
{
    int fd, i;
    caddr_t addr;

    if ((fd = open("/dev/zero", O_RDWR)) < 0)
    {
	(void) fprintf(stderr, "%s: Couldn't open /dev/zero to map "
		       "shared screen memory (%s)\n", progname,
		       strerror(errno));

	return -1;
    }

    addr = mmap(0, fractal_set->size * fractal_set->bp.depth,
		PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    while (close(fd) < 0 && errno == EINTR)
	;

    if (addr == (caddr_t)-1)
	return -1;			/* mmap failed, darn. */

    for (i=0; i < fractal_set->bp.depth; ++i)
	fractal_set->bp.bpl[i] =
	    (unsigned char *)addr + (fractal_set->size * i);

    return 0;			/* Everything is Ok. */
}
