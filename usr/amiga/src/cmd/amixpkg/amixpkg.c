
#include	<stdio.h>

/*
**	Since I really hate obscure pieces of code, here's the
**	explanation and justification for this one.
**
**	AT&T's packaging tools demand the existance of something
**	called a contents file.  (See /var/sadm/install/contents.)
**	This file lists all of the installed files, directories, etc.
**	When you use pkgrm to remove a package, it does so by
**	consulting this file.
**
**	Amixpkg takes the <packagename>.cont files that each package
**	on the tape leaves in /tmp and merges them with the contents
**	file.
**
**	The problem is files and directories that are in more than one
**	package.  For these, you can't just add a line to the contents
**	file and have it work.  You have to merge the entry for a
**	filesystem object you're adding with any preexisting entries
**	of the same name.
**
**	Eg.,
**
**		the "core" package  installs the directory "/usr" with
**		this entry:
**
**		    /usr d none 0755 bin bin core
**
**		when you add the "man" package, it supplies this
**		entry:
**
**		    /usr d none 0755 bin bin man
**
**		To work with the packaging tools, these must be merged
**		into this:
**
**		    /usr d none 0755 bin bin core man
**
**	And that is the purpose of PROCESS_CONTENTS.  When sprintf()'d
**	to a buffer and given the arguments of the temporary and the
**	permanent contents file, it builds a shell pipeline that will
**	sort and merge the entries to produce the correct contents
**	file.
**
**	David Miller
*/

# define	PROCESS_CONTENTS				\
		"sort %s | awk ' $1 ~ /^#/ {\n"			\
		"	print;\n"				\
		"	next;\n"				\
		"}\n"						\
		"{\n"						\
		"	if ($1 != lastname)\n"			\
		"	{\n"					\
		"		if (last != \"\")\n"		\
		"			print last;\n"	       	\
		"		last = $0;\n"			\
		"		lastname = $1;\n"		\
		"		next;\n"			\
		"	}\n"					\
		"}\n"						\
		"$2 == \"b\"  || $2 == \"c\" {\n"		\
		"	for (i = 9; i <= NF; i++)\n"		\
		"		last = last \" \" $i;\n"	\
		"	next;\n"				\
		"}\n"						\
		"$2 == \"d\" || $2 == \"p\" {\n"		\
		"	for (i = 7; i <= NF; i++)\n"		\
		"		last = last \" \" $i;\n"	\
		"	next;\n"				\
		"}\n"						\
		"$2 == \"f\" {\n"				\
		"	for (i = 10; i <= NF; i++)\n"		\
		"		last = last \" \" $i;\n"	\
		"	next;\n"				\
		"}\n"						\
		"END {\n"					\
		"	print last;\n"				\
		"}' > %s\n"


#define		TRUE	1
#define		FALSE	0

#define		MAX	200
#define		LEN	2048

#define		iswhite(c)	((c) == ' ' || (c) == '\t' || (c) == '\n')

struct pkg {
	char *pkg;			/* short identifying keyword */
	char *name;			/* wordy description of package */
	int size;			/* size of package in blocks */
	int inst;			/* package selected for installation? */
	int opt;			/* is this an optional package? */
	int zcat;			/* is the archive compressed? */
};

struct pkg list[MAX];
int list_top = 0;
int verbose = FALSE;
int emit_options = FALSE;

char pattern[LEN];

extern char *my_malloc();
extern char *str_save();
char *parse_key();

char *progname;

char *yes[MAX];
int yes_top = 0;

char *unpackdir = "/";
char version[LEN];

int max_size = -1;
int install_flag = FALSE;
int batch = FALSE;


main(argc, argv)
int argc;
char **argv;
{
	extern char *optarg;
	extern int optind, opterr;
	int c;
	int errflag = 0;
	int ret;
	int readlist = TRUE;
	char buf[LEN];
	int i;

	progname = argv[0];
	*pattern = '\0';

	while ((c = getopt(argc, argv, "bdir:s:y:vm?")) != -1)
		switch(c) {
		case 'b':		/* batch mode */
			batch = TRUE;
			break;

		case 'd':		/* don't read List from the tape */
			readlist = FALSE;
			break;

		case 'i':
			install_flag = TRUE;
			break;

		case 'r':		/* root directory to unpack in */
			unpackdir = str_save(optarg);
			break;

		case 's':		/* maximum size allowed (in blocks) */
			max_size = atoi(optarg);
			break;

		case 'y':
			yes[yes_top++] = str_save(optarg);
			break;

		case 'v':
			verbose = TRUE;
			break;

		case 'm':
			emit_options = TRUE;
			break;

		default:
			errflag++;
		}

	if (errflag) {
fprintf(stderr, "usage: %s [options]\n", progname);
fprintf(stderr, "  -d      Don't read header List archive from tape\n");
fprintf(stderr, "  -i      Present initial installation choices\n");
fprintf(stderr, "  -m      Output amixpkg options to produce user's choices\n");
fprintf(stderr, "  -r dir  Root directory to install\n");
fprintf(stderr, "  -s size Only size blocks are available for installation\n");
fprintf(stderr, "  -t dir  Directory for List header archive\n");
fprintf(stderr, "  -y pkg  Set default install to yes for this package\n");
fprintf(stderr, "  -v      Be verbose when installing\n");
		exit(1);
	}

	putenv("QUIETCPIO=foo");
	putenv("QUIETDD=foo");

	/*
	 * 1st tape archive: index containing two files: List and Version
	 * 2nd tape archive: all package content files together to be handy
	 * 3rd tape archive: first package in List
	 */

	sprintf(buf, "%s/tmp", unpackdir);
	chdir(buf);
	if (readlist) {
		close(open("/dev/rmt/4h", 0));		/* rewind tape */
		if (system("cpio -imdcuB < /dev/rmt/4hn > /dev/null") != 0) {
			fprintf(stderr, "can't read index from tape\n");
			exit(1);
		}
	}
	read_list();

	if (optind < argc) {
		int err = 0;

		while (optind < argc)
			err += eat_names(argv[optind++]);
		if (err > 0)
			exit(1);
	} else {
		if (install_flag)
			ret = installation_choices();
		else if (batch)	   /* batch mode, don't show selection list */
			ret = FALSE;
		else
			ret = TRUE;

		while (ret) {
			show_list();
			ret = choose_names();
		}
	}

	if (emit_options)
		output_options();

	if (!do_install())
		exit(1);

	exit(0);
}


output_options() {
	FILE *fp;
	int i;

	fp = fopen("options", "w");
	if (fp == NULL) {
		fprintf(stderr, "can't open options file: ");
		perror("");
		exit(1);
	}

	for (i = 0; i < list_top; i++)
		if (list[i].inst)
			fprintf(fp, "-y %s ", list[i].pkg);
	fprintf(fp, "\n");
	fclose(fp);
	exit(0);
}


installation_choices() {
	char buf[LEN];
	int standard = 0;
	int whole = 0;
	int i;
	int avail, wanted;

	for (i = 0; i < list_top; i++) {
		if (!list[i].opt)
			standard += list[i].size;
		whole += list[i].size;
	}

	while (1) {
		printf("\n");
		printf("(1)  Standard Amiga UNIX installation    %6.1f MB\n",
						((float) standard) / 2048.0);
		printf("(2)  Everything on the tape              %6.1f MB\n",
						((float) whole) / 2048.0);
		printf("(3)  Custom selection\n");

		printf("\nChoose: [1] ");

		if (fgets(buf, LEN, stdin) == NULL)
			continue;

		if (strcmp(buf, "1\n") == 0 || *buf == '\n') {
			if (max_size > -1 && standard > max_size) {
			    avail = max_size / 2048;
			    wanted = standard / 2048;

			    printf("\nWARNING:  ");
			    printf("Your allocated space for installation ");
			    printf("is %d MB,\n", avail);
			    printf("but you have selected to install %d MB.\n",
							wanted);
			    continue;
			}

			select_standard(TRUE);
			return FALSE;
		}
		if (strcmp(buf, "2\n") == 0) {
			if (max_size > -1 && whole > max_size) {
			    avail = max_size / 2048;
			    wanted = standard / 2048;

			    printf("\nWARNING:  ");
			    printf("Your allocated space for installation ");
			    printf("is %d MB,\n", avail);
			    printf("but you have selected to install %d MB.\n",
								wanted);
			    continue;
			}
			select_all(TRUE);
			return FALSE;
		}
		if (strcmp(buf, "3\n") == 0) {
			return TRUE;
		}
	}
}


seek_tape() {
	int fd;

	fd = open("/dev/rmt/4hn", 0);
	if (fd < 0) {
		perror("can't seek tape");
		return FALSE;
	}

	close(fd);
	return TRUE;
}


do_install() {
	int pos = 0;
	int i;
	int ret;
	char buf[1024];

	printf("\nReading tape...\n");
	chdir(unpackdir);

	if ( system("dd if=/dev/rmt/4hn bs=256k|cpio -imdcu") != 0) {
		fprintf(stderr, "Reading package content files failed\n");
		return FALSE;
	}

	for (i = 0; i < list_top; i++) {
		if (!list[i].inst)
			continue;

		while (pos < i) {
			if (!seek_tape())
				return FALSE;
			pos++;
		}

		if (verbose)
			printf("Installing %s\n", list[i].name);

		if (list[i].zcat)
			sprintf(buf,
		  "dd if=/dev/rmt/4hn bs=256k | zcat | cpio -imdcu %s", pattern);
		else
			sprintf(buf,
			 "dd if=/dev/rmt/4hn bs=256k | cpio -imdcu %s", pattern);

		ret = system(buf);

		if (ret != 0) {
			fprintf(stderr, "installation of %s failed\n",
								list[i].pkg);
			return FALSE;
		}

		if (*pattern == '\0')
			update_contents(i);
				/* leave the package install slime trail */
		pos++;
	}
	printf("Done reading tape.\n");

	sync();
	return TRUE;
}


find_list(s)
char *s;
{
	int i;

	for (i = 0; i < list_top; i++)
		if (strcmp(s, list[i].pkg) == 0)
			return i;

	return -1;
}


choose_names() {
	char buf[LEN];
	int total = 0;
	int i;
	char *p;

	for (i = 0; i < list_top; i++)
		if (list[i].inst)
			total += list[i].size;

	printf("%-8s %-10s %-40s %8s\n", "", "", "", "====  ");
	printf("%53s Total: %6.1f MB\n", "", ((float) total) / 2048.0);

	fputs("\nEnter package name to install or ignore; enter GO when ready.\n",
								stdout);

#if 0
	fputs("\nToggle packages by name.  Type \"go\" when done: ", stdout);
	fputs("\nChoose packages by name, \"standard\" or \"all\";", stdout);
	fputs("  Prefix with ! to omit.\n", stdout);
	fputs("Use \"go\" when done: ", stdout);
#endif

	fflush(stdout);

	if (fgets(buf, LEN, stdin) == NULL) {
		buf[0] = '\0';
		return TRUE;
	}
	for (p = buf; *p && *p != '\n'; p++) ;
	*p = '\0';

	if (strcmp(buf, "restart") == 0) {
		sprintf(buf, "%s/tmp", unpackdir);
		chdir(buf);
		close(open("/dev/rmt/4h", 0));		/* rewind tape */
		if (system("cpio -imdcuB < /dev/rmt/4hn > /dev/null") != 0) {
			fprintf(stderr, "can't read index from tape\n");
			exit(1);
		}
		read_list();
		return TRUE;
	}

	if (strncmp(buf, "go", 2) == 0
	||  strncmp(buf, "GO", 2) == 0) {
		check_pattern(buf);

		if (pattern[0] == '\0' && max_size > -1 && total > max_size) {
			printf("\nWARNING:  ");
			printf("Your allocated space for installation ");
			printf("is %d block%s,\n",
						max_size, max_size == 1 ? "" : "s");
			printf("but you have selected to install %d block%s.\n",
						  total, total == 1 ? "" : "s");
			printf("\nGo ahead with the installation anyway? [n] ");
			fflush(stdout);

			if (fgets(buf, LEN, stdin) == NULL)
				return TRUE;
			if (*buf != 'y' && *buf != 'Y')
				return TRUE;
		}
		return FALSE;
	}

	eat_names(buf);
	return TRUE;
}


check_pattern(s)
char *s;
{

	s += 2;
	while (*s && iswhite(*s))
		s++;

	if (*s) {
		if (*s == '/')
			s++;		/* packages have relative pathnames */
		strcpy(pattern, s);
	} else
		*pattern = '\0';
}


select_all(inst)
int inst;
{
	int i;

	for (i = 0; i < list_top; i++)
		list[i].inst = inst;
}


select_standard(inst)
int inst;
{
	int i;

	for (i = 0; i < list_top; i++)
		if (!list[i].opt)
			list[i].inst = inst;
		else
			list[i].inst = !inst;
}


eat_names(buf_orig)
char *buf_orig;
{
	char *p, *q;
	int i;
	int err = FALSE;
	char buf[2048];

	strcpy(buf, buf_orig);
	p = buf;
	while (*p) {
		for (; *p && !isalnum(*p); p++) ;
		q = p;
		for (; *p && isalnum(*p); p++) ;
		if (*p)
			*p++ = '\0';

		if (strcmp(q, "all") == 0)
			select_all(TRUE);
		else if (strcmp(q, "standard") == 0)
			select_standard(TRUE);
		else if (strcmp(q, "none") == 0)
			select_all(FALSE);
		else {
			i = find_list(q);
			if (i < 0) {
				printf("Unknown package:  %s\n", q);
				err = TRUE;
			} else
				list[i].inst = !list[i].inst;
		}
	}

	return err;
}


show_list() {
	int i;

	if (*version)
		printf("\nTape contents:  %s\n", version);
	fputs("\n", stdout);
	printf("%-8s %-10s %-40s %8s\n",
				"Inst", "Name", "Description", "Size  ");
	printf("%-8s %-10s %-40s %8s\n",
				"----", "----", "-----------", "----  ");

	for (i = 0; i < list_top; i++) {
		printf("%-8s %-10s %-40s %6.1f\n",
			list[i].inst ? "yes" : "no",
			list[i].pkg, list[i].name,
				((float) list[i].size) / 2048.0);
	}
}


char	cmd[4096];

update_contents(i)
int i;
{
	FILE *fp, *newfp;
	char new_cont[LEN];
	char cont[LEN];
	char pkg_cont[LEN];
	char buf[LEN];

	sprintf(cont, "%s/var/sadm/install/contents", unpackdir);
	sprintf(new_cont, "%s/var/sadm/install/new_contents", unpackdir);
	sprintf(pkg_cont, "%s/tmp/%s.cont", unpackdir, list[i].pkg);

	newfp = fopen(new_cont, "w");
	if (newfp == NULL)
		return;

	fp = fopen(pkg_cont, "r");
	if (fp != NULL ) {
		while (fgets(buf, LEN, fp) != NULL)
			if (*buf != '#')
				fputs(buf, newfp);
		fclose(fp);
	}

	fp = fopen(cont, "r");
	if (fp != NULL) {
		while (fgets(buf, LEN, fp) != NULL)
			if (*buf != '#')
				fputs(buf, newfp);
		fclose(fp);
	}

	fprintf(newfp, "# Last modified by amixpkg for %s package\n",
							list[i].pkg);
	fclose(newfp);
	sprintf(cmd, PROCESS_CONTENTS, new_cont, cont);
	if( system(cmd) !=0 ) {
		fprintf(stderr,	"%s package contents file processing failed\n",
							list[i].pkg);
		exit(1);
	}
	    
	unlink(new_cont);

	mkdir("var/sadm/pkg", 0755);
	sprintf(buf, "var/sadm/pkg/%s", list[i].pkg);
	mkdir(buf, 0755);
	sprintf(buf, "var/sadm/pkg/%s/install", list[i].pkg);
	mkdir(buf, 0755);
	sprintf(buf, "var/sadm/pkg/%s/save", list[i].pkg);
	mkdir(buf, 0755);

	sprintf(buf, "var/sadm/pkg/%s/pkginfo", list[i].pkg);

	fp = fopen(buf, "w");
	fprintf(fp, "CLASSES= none\n");
	fprintf(fp, "OAMBASE=/usr/sadm/sysadm\n");
	fprintf(fp, "PKG=%s\n", list[i].pkg);
	fprintf(fp, "NAME=%s\n", list[i].name);
	fprintf(fp, "CATEGORY=system\n");
	fprintf(fp, "ARCH=Amiga\n");
	fprintf(fp, "PKGINST=%s\n", list[i].pkg);
	fprintf(fp, "PKGSAV=/var/sadm/pkg/%s/save\n", list[i].pkg);
	fclose(fp);

	sync();		/* Well, it can't hurt! */

}


read_list() {
	FILE *fp;
	char buf[LEN];
	char *p;
	int i;

	list_top = 0;

	fp = fopen("List", "r");
	if (fp == NULL) {
		perror("Can't open List");
		exit(1);
	}

	while (fgets(buf, LEN, fp) != NULL) {
		for (p = buf; *p && *p != '\n'; p++) ;
		*p = '\0';

		list[list_top].pkg = parse_key(buf, "pkg=");
		list[list_top].name = parse_key(buf, "name=");
		p = parse_key(buf, "size=");
		if (p == NULL || *p == '\0')
			list[list_top].size = 0;
		else
			list[list_top].size = atoi(p);

		p = parse_key(buf, "opt=");
		if (p != NULL && (*p == 'y' || *p == 'Y'))
			list[list_top].opt = TRUE;
		else
			list[list_top].opt = FALSE;

		p = parse_key(buf, "zcat=");
		if (p != NULL && (*p == 'y' || *p == 'Y'))
			list[list_top].zcat = TRUE;
		else
			list[list_top].zcat = FALSE;

		list[list_top].inst = FALSE;
		list_top++;
	}

	fclose(fp);

	fp = fopen("Version", "r");
	if (fp != NULL) {
		if (fgets(version, LEN, fp) != NULL) {
			for (p = version; *p && *p != '\n'; p++) ;
			*p = '\0';
		} else
			*version = '\0';
		fclose(fp);
	} else
		*version = '\0';

	for (i = 0; i < yes_top; i++)
		eat_names(yes[i]);
}


char *
parse_key(s, key)
char *s;
char *key;
{
	char *p;
	char *q;
	char *ret;
	char save;

	p = s;
	while (*p) {
		if (*p == '"') {
			p++;
			while (*p && *p != '"')
				p++;
		}

		if (*p == *key && strncmp(p, key, strlen(key)) == 0) {
			p += strlen(key);
			if (*p == '"') {
				p++;
				for (q = p; *q && *q != '"'; q++) ;
			} else
				for (q = p; *q && (isalnum(*q)); q++) ;
			save = *q;
			*q = '\0';
			ret = str_save(p);
			*q = save;
			return ret;
		}
		p++;
	}

	return "";
}


char *
my_malloc(size)
unsigned size;
{
	char *p;
	extern char *malloc();

	p = malloc(size);
	if (p == NULL) {
		fprintf(stderr, "%s:  out of memory (can't malloc %d bytes)\n",
							progname, size);
		exit(1);
	}
	return p;
}


char *
str_save(s)
char *s;
{
char *p;

	p = my_malloc(strlen(s) + 1);
	strcpy(p, s);

	return(p);
}
