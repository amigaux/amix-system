/*	Copyright (c) 1991 Commodore-Amiga, Inc.	*/
/*	  All Rights Reserved  	*/

/*	Author:  Fred Fish				*/

#ident	"@(#)knlist.c	1.2a"

#include <sys/types.h>
#include <sys/sysm68k.h>
#include <osfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <libelf.h>
#include <nlist.h>


#define DEBUGP(a)  /* printf a */	/* Uncomment for debug prints */

/*
 *	Variables that we use in several places.  Note that they
 *	have to be initialized explicitly on each call.  See knlinit().
 */

static int knl_fd;			/* File descriptor for ELF file */
static struct nlist *knl_list;		/* The nlist to adjust */
static Elf *knl_elf;			/* Base ELF library access pointer */
static Elf32_Ehdr *knl_ehdr;		/* Pointer to file ELF header */
static Elf32_Half knl_text_index;	/* Section index of .text section */
static Elf32_Half knl_data_index;	/* Section index of .data section */
static Elf32_Half knl_bss_index;	/* Section index of .bss section */
static Elf32_Word knl_text_size;	/* Size of .text section */
static Elf32_Word knl_data_size;	/* Size of .data section */
static Elf32_Word knl_bss_size;		/* Size of .bss section */
static Elf32_Word knl_text_start;	/* Address of start of text */
static Elf32_Word knl_data_start;	/* Address of start of data */
static Elf32_Word knl_bss_start;	/* Address of start of bss */
static Elf32_Word knl_end;		/* Address just past end of bss */

/*
 *	Prototypes for local functions.
 */

static int adjustlist (void);
static int findsections (void);
static int bindsections (void);
static int scansymtabs (void);
static void dosymbol (Elf32_Sym *sym, char *name);
static void knlinit (void);


static int kernelbase (Elf32_Word *baseaddr)
{
    return sysm68k(_m68k_KERNADDR, baseaddr);
}

/*
 *	"Kernel nlist" function.  Performs a normal nlist and then
 *	if that succeeds, checks to see the the specified file is
 *	an ELF relocatable file.  If so, the kernel is queried for
 *	the address of the start of it's ".text" segment, and all
 *	".text", ".data", and ".bss" symbols are adjusted to the
 *	same values that the boot loader relocation module assigned
 *	them at boot time.
 */

int knlist (const char *name, struct nlist *list)
{
    int result;

    if ((result = nlist (name, list)) == 0) {
	knlinit ();
	knl_list = list;
	if ((elf_version (EV_CURRENT) == EV_NONE) ||
	    ((knl_fd = open ((char *) name, 0)) < 0) ||
	    ((knl_elf = elf_begin (knl_fd, ELF_C_READ, (Elf *) 0)) == NULL) ||
	    (elf_kind (knl_elf) != ELF_K_ELF) ||
	    ((knl_ehdr = elf32_getehdr (knl_elf)) == NULL)) {
	    result = -1;
	} else {
	    if (knl_ehdr -> e_type == ET_REL) {
		result = adjustlist ();
	    }
	}
	if (knl_elf != NULL) (void) elf_end (knl_elf);
	if (knl_fd >= 0) (void) close (knl_fd);
    }
    return (result);
}

/*
 *	Given a pointer to an nlist struct, and a pointer to an open
 *	relocatable Elf file which is presumably the kernel file from
 *	which the system was booted, adjust all the symbol values to
 *	reflect the actual values to which they were assigned by the
 *	relocating boot loader.
 */

static int adjustlist (void)
{
    int result;

    if ((result = findsections ()) == 0) {
	if ((result = bindsections ()) == 0) {
	    result = scansymtabs ();
	}
    }
    return (result);
}

/*
 *	Scan through the section header table, locating the .text,
 *	.data, and .bss sections.  Save their section header table
 *	indices and their sizes.
 */

static int findsections (void)
{
    int result = 0;
    char *name;
    Elf32_Half scnindex;
    Elf_Scn *scn;
    Elf32_Shdr *shdr;

    for (scnindex = 1 ; scnindex < knl_ehdr -> e_shnum ; scnindex++) {
	if ((scn = elf_getscn (knl_elf, (size_t) scnindex)) != NULL) {
	    if ((shdr = elf32_getshdr (scn)) != NULL) {
		name = elf_strptr (knl_elf, knl_ehdr -> e_shstrndx,
				   (size_t) shdr -> sh_name);
		if (strcmp (name, ".text") == 0) {
		    knl_text_index = scnindex;
		    knl_text_size = shdr -> sh_size;
		} else if (strcmp (name, ".data") == 0) {
		    knl_data_index = scnindex;
		    knl_data_size = shdr -> sh_size;
		} else if (strcmp (name, ".bss") == 0) {
		    knl_bss_index = scnindex;
		    knl_bss_size = shdr -> sh_size;
		}
	    }
	}
    }
    DEBUGP (("text %d %#x\n", knl_text_index, knl_text_size));
    DEBUGP (("data %d %#x\n", knl_data_index, knl_data_size));
    DEBUGP (("bss %d %#x\n", knl_bss_index, knl_bss_size));
    return (result);
}

/*
 *	Figure out the starting address of each of the three
 *	sections of interest.  We use the algorithm that the .text
 *	section is bound to the base address specified by the kernel
 *	and retrieved at runtime via a system call.  Immediately
 *	after the kernel, comes the .data and .bss sections.
 *
 */

static int bindsections (void)
{
    int result;

    if ((result = kernelbase (&knl_text_start)) == 0) {
	knl_data_start = knl_text_start + knl_text_size;
	knl_bss_start = knl_data_start + knl_data_size;
	knl_end = knl_bss_start + knl_bss_size;
    }
    DEBUGP (("text_start %#x", knl_text_start));
    DEBUGP (("data_start %#x", knl_data_start));
    DEBUGP (("bss_start  %#x", knl_bss_start));
    DEBUGP (("end        %#x", knl_end));
    return (result);
}

/*
 *	Scan the section table header, finding all the symbol table
 *	sections, and computed the adjusted symbol values for each
 *	symbol in the user specified nlist.
 *
 *	Note that the library nlist only handles one symbol table, so
 *	our effort at being more general is wasted for now, but it is
 *	no more work to simply not quit at the first one.  Note there
 *	is also the potential for a bug here though, if nlist only
 *	finds the first symbol table, there is actually more than one,
 *	and one of the symbols we are interested in is defined with
 *	different values in each.
 *
 */

static int scansymtabs (void)
{
    int result = 0;
    Elf_Scn *scn;
    Elf32_Half scnindex;
    Elf32_Shdr *shdr;
    char *name;
    Elf_Data *symdata = NULL;
    Elf32_Sym *sym;
    int nsyms;

    for (scnindex = 1 ; scnindex < knl_ehdr -> e_shnum ; scnindex++) {
	if ((scn = elf_getscn (knl_elf, (size_t) scnindex)) != NULL) {
	    if ((shdr = elf32_getshdr (scn)) != NULL) {
		if (shdr -> sh_type == SHT_SYMTAB) {
		    symdata = (Elf_Data *) elf_getdata (scn, symdata);
		    if (symdata != NULL) {
			sym = (Elf32_Sym *) symdata -> d_buf;
			nsyms = symdata -> d_size / sizeof (Elf32_Sym);
			while (nsyms-- > 0) {
			    name = elf_strptr (knl_elf, shdr -> sh_link,
					       (size_t) (sym -> st_name));
			    dosymbol (sym, name);
			    sym++;
			}
		    }
		}
	    }
	}
    }
    return (result);
}

/*
 *	Process one symbol.  First check to see if it is a COMMON symbol,
 *	and if so, allocate space for it at the end of bss (growing bss
 *	in the process).  Note that the st_value field holds alignment
 *	contraints.
 *
 *	If it is not a COMMON symbol, check to see if it is in one of
 *	our three sections of interest, and if so, adjust the value by
 *	the base address of the section that it is in.
 *
 *	Finally, scan through the user's nlist and adjust any matching
 *	symbol appropriately.  Note that we must process every symbol
 *	even if it is not in the nlist, because the COMMON symbols need
 *	to be allocated as we find them.
 */

static void dosymbol (Elf32_Sym *sym, char *name)
{
    Elf32_Addr vaddr;	/* New symbol address */
    Elf32_Addr symval;	/* Local copy, avoid changing ELF data */
    Elf32_Half shndx;	/* Local copy, avoid changing ELF data */
    struct nlist *nlp;	/* Nlist scan pointer */

    symval = sym -> st_value;
    shndx = sym -> st_shndx;
    if (shndx == SHN_COMMON) {
	shndx = knl_bss_index;
	vaddr = knl_end;
	vaddr += (symval - 1);
	vaddr &= ~(symval - 1);
	symval = vaddr;
	knl_end = vaddr + sym -> st_size;
	knl_bss_size = knl_end - knl_bss_size;
    } else if (shndx == knl_text_index) {
	symval += knl_text_start;
    } else if (shndx == knl_data_index) {
	symval += knl_data_start;
    } else if (shndx == knl_bss_index) {
	symval += knl_bss_start;
    }
    if (name != NULL && *name && (nlp = knl_list) != NULL) {
	while ((nlp -> n_name != NULL) && *(nlp -> n_name)) {
	    if (strcmp (nlp -> n_name, name) == 0) {
		nlp -> n_value = symval;
		nlp -> n_scnum = shndx;
		break;
	    }
	    nlp++;
	}
    }
    DEBUGP (("sym '%s' %#x %d\n", name ? name : "", symval, sym -> st_shndx));
}

/*
 *	Initialize our static variables each time we are called.
 *	This is slightly less ugly than passing hordes of parameters around,
 *	or collecting all of them in a structure and passing a pointer to
 *	it around (all the derefs get ugly).  Besides, this code is quite
 *	cpu intensive anyway, so this is also the fastest.
 *
 */

static void knlinit ()
{
    knl_fd = 0;
    knl_list = NULL;
    knl_elf = NULL;
    knl_ehdr = NULL;
    knl_text_index = 0;
    knl_data_index = 0;
    knl_bss_index = 0;
    knl_text_size = 0;
    knl_data_size = 0;
    knl_bss_size = 0;
    knl_text_start = 0;
    knl_data_start = 0;
    knl_bss_start = 0;
    knl_end = 0;
}


/*
 *	Below is a simple test driver to call the knlist function
 *	with some user specified arguments.
 */

#ifdef BUILD_TEST

#include <stdio.h>

static struct nlist nl[1000];

#define H1 "          n_name  n_value   n_scnum n_type n_sclass n_numaux"
#define H2 "---------------- ---------- ------- ------ -------- --------"

main (int argc, char *argv[])
{
    struct nlist *nlp;
    char *filename;

    if (argc < 3) {
	printf ("usage: nlisttest <filename> <name> <name> ...\n");
    } else {
	filename = *++argv;
	argv++;
	argc -= 2;
	nlp = nl;
	while (argc-- > 0) {
	    nlp++ -> n_name = *argv++;
	}
	nlp -> n_name = "";
	if (knlist (filename, nl) == -1) {
	    perror (filename);
	} else {
	    printf ("%s\n", H1);
	    printf ("%s\n\n", H2);
	    for (nlp = nl; *(nlp -> n_name) != '\000'; nlp++) {
		printf ("%16s 0x%8.8lx %7d 0x%4.4x %8d %8d\n",
			nlp -> n_name,
			nlp -> n_value, nlp -> n_scnum, nlp -> n_type,
			nlp -> n_sclass, nlp -> n_numaux);
	    }
	}
    }
    return (0);
}

#endif	/* BUILD_TEST */
