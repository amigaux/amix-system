#include <fcntl.h>
#include <osfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <libelf.h>
#include <a.out.h>


/*
 * elf2coff:  program to create a COFF file from an ELF file.
 *     This only handles a very particular set of cases, and is only
 *     meant to be used to boot an ELF kernel using a COFF loader.
 *
 *					-=] Ford [=-
 *
 * Usage:  elf2coff <IN >OUT
 *
 */


struct
{
    struct filehdr filehdr;
    struct aouthdr aouthdr;
    struct scnhdr textscn;
    struct scnhdr datascn;
    struct scnhdr bssscn;
} header =
{
    { MC68MAGIC, 3, 0, 0, 0, sizeof (struct aouthdr),
      F_RELFLG|F_EXEC|F_LNNO|F_LSYMS|F_AR32W, },
    { 0x010b, 0, 0, 0, 0, 0, 0, 0, },
    { _TEXT, 0, 0, 0, 0, 0, 0, 0, 0, STYP_TEXT, },
    { _DATA, 0, 0, 0, 0, 0, 0, 0, 0, STYP_DATA, },
    { _BSS, 0, 0, 0, 0, 0, 0, 0, 0, STYP_BSS, },
};


int main(int argc, char *argv[])
{
    Elf *elfptr;
    Elf32_Ehdr *ehdr;
    Elf_Scn *sp, *tscn=0, *dscn=0;
    Elf32_Shdr *shdr;
    Elf_Data *dp;
    char *sname;

    (void)elf_version(EV_CURRENT);

    elfptr = elf_begin(0, ELF_C_READ, (Elf *)0);
    if (!elfptr)
    {
	fprintf(stderr, "%s: elf_begin failed: %s\n",
		argv[0], elf_errmsg(-1));
	return 1;
    }

    ehdr = elf32_getehdr(elfptr);
    if (!ehdr)
    {
	fprintf(stderr, "%s: elf32_getehdr failed: %s\n",
		argv[0], elf_errmsg(-1));
	return 1;
    }
    header.aouthdr.entry = ehdr->e_entry;

    sp = 0;
    while ((sp=elf_nextscn(elfptr, sp)))
    {
	shdr = elf32_getshdr(sp);
	sname = elf_strptr(elfptr, ehdr->e_shstrndx,
				 (size_t)shdr->sh_name);
	fprintf(stderr, " %#12x %#12x %#12x %-20s",
		shdr->sh_flags, shdr->sh_addr, shdr->sh_size,
		sname?sname:"???");
	if (shdr->sh_flags & SHF_ALLOC &&
	    shdr->sh_type == SHT_PROGBITS &&
	    shdr->sh_size)
	{
	    if (!tscn)
	    {
		fputs("text\n", stderr);
		tscn = sp;
		header.textscn.s_size = shdr->sh_size;
		header.textscn.s_vaddr = shdr->sh_addr;
		header.textscn.s_paddr = shdr->sh_addr;
		header.textscn.s_scnptr = sizeof header;
		header.aouthdr.tsize = shdr->sh_size;
		header.aouthdr.text_start = header.textscn.s_vaddr;
	    }
	    else
		if (!dscn)
		{
		    fputs("data\n", stderr);
		    dscn = sp;
		    header.datascn.s_size = shdr->sh_size;
		    header.datascn.s_vaddr = shdr->sh_addr;
		    header.datascn.s_paddr = shdr->sh_addr;
		    header.datascn.s_scnptr =
			header.textscn.s_scnptr + header.textscn.s_size;
		    header.aouthdr.dsize = shdr->sh_size;
		    header.aouthdr.data_start = header.datascn.s_vaddr;
		}
		else
		    fputs("ignored\n", stderr);
	}
	else
	    if (shdr->sh_flags & SHF_ALLOC &&
		shdr->sh_type == SHT_NOBITS &&
		shdr->sh_size)
	    {
		fputs("bss\n", stderr);
		header.bssscn.s_size = shdr->sh_size;
		header.bssscn.s_vaddr = shdr->sh_addr;
		header.bssscn.s_paddr = shdr->sh_addr;
		header.aouthdr.bsize = shdr->sh_size;
	    }
	    else
		fputs("who cares\n", stderr);
    }

    (void)time(&header.filehdr.f_timdat);
    fwrite(&header, sizeof header, 1, stdout);
    dp = elf_getdata(tscn, (Elf_Data *)0);
    fwrite(dp->d_buf, dp->d_size, 1, stdout);
    dp = elf_getdata(dscn, (Elf_Data *)0);
    fwrite(dp->d_buf, dp->d_size, 1, stdout);

    (void)elf_end(elfptr);

    return ferror(stdout);
}
