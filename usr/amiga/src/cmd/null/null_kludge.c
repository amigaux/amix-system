#include <fcntl.h>
#include <osfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <libelf.h>

extern int errno;

/*
 * null_kludge:  program to add, delete, or detect a special
 *     ELF section in a program which will cause a page of zeroes
 *     to appear at virtual address zero.
 *     This is useful to get some buggy AT&T programs to run.
 *     Any programmer whose code needs this to work is a rotten egg.
 *
 *					-=] Ford [=-
 *
 * Usage:  null -a {file}       Add null_kludge section to file.
 *         null -d {file}       Delete null_kludge section from file.
 *         null -q {file}       Zero exit status indicates presence
 *                              of null_kludge section in file.
 * -q is optional.
 *
 */


#define PAGESIZE 4096

char *progname;

int null_kludge(char *name, int fd, char action);


static void usage(void)
{
    (void)fprintf(stderr, "Usage: %s [-a|-d|-q] file ...\n", progname);
}


int main(int argc, char *argv[])
{
    int fd;
    int status = 0;
    int mode = O_RDONLY;
    char action = 'q';

    (void)elf_version(EV_CURRENT);

    progname = argv[0];

    if (argc>1 && argv[1][0] == '-' && !argv[1][2])
    {
	action = argv[1][1];
	switch (action)
	{
	case 'a':
	case 'd':
	    mode = O_RDWR;
	case 'q':
	    break;
	default:
	    usage();
	    return 1;
	}
	++argv, --argc;
    }

    if (argc<2)
    {
	usage();
	return 1;
    }

    for ( ++argv, --argc ; argc>0 ; ++argv, --argc )
    {
	fd = open(*argv, mode);
	if (fd<0)
	{
	    (void)fprintf(stderr, "%s: can't open %s: %s\n",
			  progname, *argv, strerror(errno));
	    ++status;
	    continue;
	}
	if (null_kludge(*argv, fd, action))
	    ++status;
	(void)close(fd);
    }

    return status;
}


int null_kludge(char *name, int fd, char action)
{
    Elf *elfptr;
    Elf32_Ehdr *ehdr;
    Elf32_Phdr *phdr, *newphdr;
    unsigned i;
    int retval;

    elfptr = elf_begin(fd, (action=='q')?ELF_C_READ:ELF_C_RDWR, (Elf *)0);
    if (!elfptr)
    {
	fprintf(stderr, "$%s: %s: elf_begin failed: %s\n",
		progname, name, elf_errmsg(-1));
	return 1;
    }

    ehdr = elf32_getehdr(elfptr);
    if (!ehdr)
    {
	fprintf(stderr, "%s: %s: elf32_getehdr failed: %s\n",
		progname, name, elf_errmsg(-1));
	return 1;
    }

    phdr = elf32_getphdr(elfptr);
    if (!phdr)
    {
	fprintf(stderr, "%s: %s: elf32_getphdr failed: %s\n",
		progname, name, elf_errmsg(-1));
	return 1;
    }

    for ( i=0 ; i<ehdr->e_phnum ; ++i )
	if (phdr[i].p_type  == PT_LOAD &&
	    phdr[i].p_flags == PF_R &&
	    phdr[i].p_vaddr == 0 &&
	    phdr[i].p_memsz == PAGESIZE)
	    break;

    switch (action)
    {
    case 'q':
	retval = (i >= ehdr->e_phnum);
	break;

    case 'a':
	if (i < ehdr->e_phnum)
	{					/* Already here,    */
	    retval = 0;				/*   no work to do. */
	    break;
	}

	newphdr = elf32_newphdr(elfptr, ehdr->e_phnum+1);
	if (!newphdr)
	{
	    fprintf(stderr, "%s: can't add new section to %s: %s\n",
		    progname, name, elf_errmsg(-1));
	    retval = 1;
	    break;
	}

	memcpy(newphdr, phdr, ehdr->e_phnum * ehdr->e_phentsize);
	/* UGLY KLUDGE */
	for ( i=0 ; i < ehdr->e_phnum - 1 ; ++i )
	    newphdr[i].p_offset += ehdr->e_phentsize;

	memset(&newphdr[i], 0, sizeof newphdr[i]);
	newphdr[i].p_type  = PT_LOAD;
	newphdr[i].p_flags = PF_R;
	newphdr[i].p_vaddr = 0;
	newphdr[i].p_memsz = PAGESIZE;

	if (elf_update(elfptr, ELF_C_WRITE) == (off_t)-1)
	{
	    fprintf(stderr, "%s: can't elf_update %s %s\n",
		    progname, name, elf_errmsg(-1));
	    retval = 1;
	    break;
	}

	retval = 0;
	break;

    case 'd':
	if (i >= ehdr->e_phnum)
	{					/* No such section, */
	    retval = 0;				/*   no work to do. */
	    break;
	}

	fprintf(stderr, "%s: deleting not supported yet\n",
		progname);
	retval = 1;
	break;
    }

    (void)elf_end(elfptr);

    return retval;
}
