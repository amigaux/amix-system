
/*
 *  Todo:
 *	loop detection in get_xx() calls
 */

/*
 * 3/7/92 Support added for drives reporting
 *
 *     rdb_CylBlocks != rdb_Heads * rdb_Sectors
 *
 * because of unavailable surrogate blocks per cylinder.
 *
 * Thanks to Larry Rosenman for tracking this down.
 * 
 */

typedef		long			LONG;
typedef		unsigned long		ULONG;
typedef		unsigned char		UBYTE;
#define EXEC_TYPES_H
#include	<sys/amiga/hardblocks.h>/* AmigaDOS disk partition standard */
#include	<sys/amiga/env.h>	/* defines for disk environment */
#include	<stdio.h>
#include	<assert.h>


#define		BSIZE		512		/* bytes per block */
#define		TRUE		1
#define		FALSE		0


extern struct RigidDiskBlock		*create_rdb();
extern struct PartitionBlock		*create_pb();	/* make a partition */
extern struct FileSysHeaderBlock	*create_fhb();	/* file sys hdr blk */
extern struct LoadSegBlock		*create_lsb();	/* loadseg list */
extern struct RigidDiskBlock		*get_rdb();
extern struct BadBlockBlock		*get_bbb();
extern struct PartitionBlock		*get_pb();
extern struct FileSysHeaderBlock	*get_fhb();
extern struct LoadSegBlock		*get_lsb();
extern struct BadBlockBlock		*put_bbb();
extern struct PartitionBlock		*put_pb();
extern struct FileSysHeaderBlock	*put_fhb();
extern struct LoadSegBlock		*put_lsb();

extern long sum2();
extern long my_atol();
extern char *my_malloc();
extern long partition_size();

long rdb_pos = 0;
long rdb_found_pos = 0;			/* position rdb was found at on disk */
long sectors_per_track = 64;


int add_part = FALSE;			/* add a partition to an existing rdb */
int partition = -1;			/* -p selected partition */
int custom_blocks = -1;			/* -C custom boot blocks */
int boot_flg = -1;			/* -b -B partition bootable */
int mount_flg = -1;			/* -m -M make parition automountable */
int boot_pri = -1;			/* -P boot priority */
int lastid_flg = -1;			/* -e -E last drive in system bit */
long fs_type = -1;			/* -F option */
int env_reserved = -1;			/* -R option */
int verbose = FALSE;
int show_header = FALSE;
int no_flags = FALSE;

char *loadseg_file = "";
int loadseg_flg = 0;
int heuristic = 0;
int disklen = 0;			/* -d disk length */
long move_rdb_pos = -1;


main(argc, argv)
int argc;
char **argv;
{
int fd;
struct RigidDiskBlock *p;
int c;
int errflg = 0;
int write_flg = 0;
int create_flg = 0;
extern int optind, opterr;
extern char *optarg;
char *device;
int ret;

	assert(sizeof(struct RigidDiskBlock) <= BSIZE);
	assert(sizeof(struct BadBlockBlock) <= BSIZE);
	assert(sizeof(struct PartitionBlock) <= BSIZE);
	assert(sizeof(struct FileSysHeaderBlock) <= BSIZE);
	assert(sizeof(struct LoadSegBlock) <= BSIZE);

	opterr = 0;
	while ((c = getopt(argc, argv, "abcp:s:wBC:P:vHVF:L:mMf:r:R:eEh:d:")) != -1) {
		switch (c) {
		case 'a':		/* add a partition to an existing rdb */
			add_part = TRUE;
			write_flg = 2;
			break;

		case 'e':
			lastid_flg = 0;
			break;

		case 'E':
			lastid_flg = 1;
			break;

		case 's':			/* sectors per track */
			sectors_per_track = my_atol(optarg);
			break;

		case 'c':			/* create new rdb */
			create_flg = TRUE;
			write_flg = 2;
			break;

		case 'w':			/* write rdb */
			write_flg = 2;
			break;

		case 'p':			/* select partition to 'edit' */
			partition = my_atol(optarg);
			if (partition <= 0) {
				fprintf(stderr,
				     "rdb: partitions are numbered from 1\n");
				errflg++;
			}
			write_flg = 2;
			break;

		case 'b':				/* not bootable */
			boot_flg = 0;
			break;

		case 'B':				/* bootable */
			boot_flg = 1;
			break;

		case 'm':				/* not mountable */
			mount_flg = 0;
			break;

		case 'M':				/* mountable */
			mount_flg = 1;
			break;

		case 'C':				/* custom boot blocks */
			custom_blocks = my_atol(optarg);
			break;

		case 'P':				/* boot priority */
			boot_pri = my_atol(optarg);
			break;

		case 'F':				/* fs type */
			fs_type = my_atol(optarg);
			break;

		case 'r':	/* relocate (move) rdb to a new pos on disk */
			move_rdb_pos = my_atol(optarg);
			write_flg = 2;
			break;

		case 'R':				/* reserved blocks */
			env_reserved = my_atol(optarg);
			break;

		case 'v':				/* verbose */
			verbose = TRUE;
			break;

		case 'H':				/* extra verbosity */
			show_header = TRUE;
			no_flags = FALSE;
			break;

		case 'V':			   /* more extra verbosity */
			show_header = TRUE;
			no_flags = TRUE;
			break;

		case 'L':		    /* default pos for rdb on disk */
			rdb_pos = my_atol(optarg);
			break;

		case 'f':		/* add file system, loadseg list */
			strcpy(loadseg_file, optarg);
			loadseg_flg = 1;
			write_flg = 2;
			break;

		case 'h':	/* rdb heuristic support for install script */
			heuristic = atoi(optarg);
			break;

		case 'd':	/* disk length for gap computation */
			disklen = atoi(optarg);
			break;

		default:
			errflg++;
		}
	}

	if (optind < argc)
		device = argv[optind++];
	else
		errflg++;

	if (add_part && (argc - optind != 3)) {
		fprintf(stderr, "rdb: improper partition list\n");
		errflg++;
	} else if (create_flg && add_part){
		fprintf(stderr, "-c and -a are exclusive\n");
		errflg++;
	} else if (add_part && partition != -1) {
		fprintf(stderr, "-c and -p are exclusive\n");
		errflg++;
	} else if (create_flg && disklen == 0) {
		fprintf(stderr, "rdb: -c requires -d disksize \n");
		errflg++;
	} else if (create_flg && (argc - optind) % 3 != 0) {
		fprintf(stderr, "rdb: improper partition list\n");
		errflg++;
	} else if (partition > -1 && create_flg) {
		fprintf(stderr, "rdb: -p and -c are exclusive\n");
		errflg++;
	} else if (partition > -1 && loadseg_flg) {
		fprintf(stderr, "rdb: -p and -f are exclusive\n");
		errflg++;
	} else if (lastid_flg > -1 && partition > -1) {
		fprintf(stderr, "rdb: -p and -e or -E are exclusive\n");
		errflg++;
	} else if (lastid_flg > -1 && loadseg_flg) {
		fprintf(stderr, "rdb: -e or -E and -f are exclusive\n");
		errflg++;
	}
	if (partition == -1 &&
		(boot_flg != -1
		|| mount_flg != -1
		|| custom_blocks != -1
		|| boot_pri != -1
		|| env_reserved != -1
		|| fs_type != -1)) {
			fprintf(stderr,
				"rdb: -[bBmMPFCR] may only be used with -p\n");
			errflg++;
	}


	if (errflg) {
	fprintf(stderr, "usage: rdb [-v] [-w] [-H] device\n");
	fprintf(stderr, "       rdb -a device name start length\n");
	fprintf(stderr, "       rdb -c [-s n] device [name start length]*\n");;
	fprintf(stderr,
"       rdb -p partition [-b] [-B] [-m] [-M] [-C n] [-F n] [-P n] [-R n] device\n");
		exit(1);
	}

	fd = open(device, write_flg);
	if (fd < 0) {
		perror("can't open device");
		exit(1);
	}

	p = get_rdb(fd);

	if (add_part)
		add_partition(p, argc, argv);
	else if (move_rdb_pos != -1) {
		clear_rdb(fd, move_rdb_pos);
		rdb_pos = move_rdb_pos;
	} else if (heuristic != 0)
		rdb_heuristic(p);
	else if (create_flg)
		p = create_rdb(argc, argv);
	else if (partition > -1)
		edit_rdb(p);
	else if (loadseg_flg)
		add_loadseg(p);
	else if (lastid_flg != -1) {
		write_flg = 1;
		if (lastid_flg == 1)
			p->rdb_Flags |= RDBFF_LASTTID;
		else
			p->rdb_Flags &= ~RDBFF_LASTTID;
	} else
		show_rdb(p);

	if (write_flg)
		put_rdb(fd, p, rdb_pos);

	exit(0);
}


rdb_heuristic(rdb)
struct RigidDiskBlock *rdb;
{
struct PartitionBlock *q;
int got_15 = FALSE;
int got_5 = FALSE;
int got_2 = FALSE;
struct PartitionBlock *root = NULL;
struct PartitionBlock *swap = NULL;
struct PartitionBlock *boot = NULL;
long size;
int root_num;
int boot_num;
int swap_num;
int count = 0;


	switch (heuristic) {
	case 1:				/* rdb "suitable" for Unix use? */
		if (rdb == NULL)
			exit(1);
		for (q = rdb->rdb_PartitionList; q; q = q->pb_Next) {
			size = partition_size(rdb, q) / 1048576;

			if (!got_15 && size >= 16)
				got_15 = TRUE;
			else if (!got_5 && size >= 5)
				got_5 = TRUE;
			else if (!got_2 && size >= 2)
				got_2 = TRUE;
		}
		if (got_2 && got_5 && got_15)
			exit(0);
		else
			exit(1);
		break;				/* NOTREACHED */

	case 2:		/* are there obvious choices for Unix partitions? */
	case 3:		/* print out obvious choices for Unix partitions */
	case 4:		/* emit choices for Unix partitions */
		if (rdb == NULL)
			exit(1);
		for (q = rdb->rdb_PartitionList; q; q = q->pb_Next) {
			count++;
			size = partition_size(rdb, q) / 1048576;

			if (strncmp(q->pb_DriveName, "WB_", 3) == 0
			||  strcmp(q->pb_DriveName, "Work") == 0)
				continue;

			if (boot == NULL &&
				(((strcmp(q->pb_DriveName, "Unix_Boot") == 0 ||
				   strcmp(q->pb_DriveName, "UNIX_Boot") ==0) &&
				size >= 2) || (size >= 2 && size <= 3))) {
					boot = q;
					boot_num = count;
			} else if (swap == NULL &&
				(((strcmp(q->pb_DriveName, "Unix_Swap") == 0 ||
				   strcmp(q->pb_DriveName, "UNIX_Swap") == 0) &&
				size >= 5) || (size >= 5 && size <= 30))) {
					swap = q;
					swap_num = count;
			} else if (root == NULL &&
				(((strcmp(q->pb_DriveName, "Unix_Root") == 0 ||
				   strcmp(q->pb_DriveName, "UNIX_Root") == 0) &&
				size >= 20) || size >= 40)) {
					root = q;
					root_num = count;
			}
		}

		switch (heuristic) {
		case 2:
			if (root && swap && boot)
				exit(0);	/* there is an obvious choice */
			else
				exit(1);
			break;

		case 3:
			show_header = TRUE;
			no_flags = TRUE;
			if (root && swap && boot) {
				printf("Root:  %d:  ", root_num);
				show_part(rdb, root);
				printf("Swap:  %d:  ", swap_num);
				show_part(rdb, swap);
				printf("Boot:  %d:  ", boot_num);
				show_part(rdb, boot);
				exit(0);
			} else {
				fprintf(stderr,
				    "error: no obvious partition choices\n");
				exit(1);
			}
			break;

		case 4:
			if (root && swap && boot) {
				printf("ROOTPART=%d\n", root_num);
				printf("SWAPPART=%d\n", swap_num);
				printf("BOOTPART=%d\n", boot_num);
			} else {
				fprintf(stderr,
				    "error: no obvious partition choices\n");
				exit(1);
			}
			break;

		default:
			fprintf(stderr, "rdb internal error: bad case\n");
			exit(1);
		}
		break;

	case 5:		/* how many partitions are in the partition table? */
		if (rdb != NULL)
			for (q = rdb->rdb_PartitionList; q; q = q->pb_Next)
				count++;
		printf("%d\n", count);
		exit(0);
		break;

	case 6:		/* determine largest gap start & length */
		find_biggest_gap(rdb);
		break;

	case 7:		/* return sectors per track */
		if (rdb == NULL)
			exit(1);
		printf("%d\n", rdb->rdb_Sectors);
		exit(0);
		break;

	case 8:		/* return block rdb was found at on disk */
		printf("%d\n", rdb_found_pos);
		exit(0);
		break;

	default:
		fprintf(stderr, "unknown rdb heuristic: %d\n", heuristic);
		exit(1);
	}
}


#define	MAX_GAPS	50

struct rdb_gap {
	long base, length;
};

struct rdb_gap gap[MAX_GAPS];

int gap_top = 0;


register_gap(base, len)
long base;
long len;
{

	if (gap_top < MAX_GAPS) {
		gap[gap_top].base = base;
		gap[gap_top].length = len;
		gap_top++;
	}
}


struct rdb_gap *
biggest_gap() {
	int i;
	struct rdb_gap *p = NULL;
	long max = 0;

	for (i = 0; i < gap_top; i++) {
		if (gap[i].length > max) {
			max = gap[i].length;
			p = &gap[i];
		}
	}

	return p;
}


struct PartitionBlock *
next_lowest_base(rdb, this)
struct RigidDiskBlock *rdb;
long this;
{
	struct PartitionBlock *p;
	long low = 0;
	struct PartitionBlock *ptr = NULL;
	long base;

	if (rdb == NULL)
		return NULL;

	for (p = rdb->rdb_PartitionList; p; p = p->pb_Next) {
		base = p->pb_Environment[DE_LOWCYL] * rdb->rdb_CylBlocks; 

		if (base >= this && (base < low || low == 0)) {
			low = base;
			ptr = p;
		}
	}

	return ptr;
}


struct PartitionBlock *
highest_base(rdb)
struct RigidDiskBlock *rdb;
{
	struct PartitionBlock *p;
	long max = 0;
	struct PartitionBlock *ptr = NULL;
	long base;

	if (rdb == NULL)
		return NULL;

	for (p = rdb->rdb_PartitionList; p; p = p->pb_Next) {
		base = p->pb_Environment[DE_LOWCYL] * rdb->rdb_CylBlocks;

		if (base > max) {
			max = base;
			ptr = p;
		}
	}

	return ptr;
}


find_biggest_gap(rdb)
struct RigidDiskBlock *rdb;
{
	struct PartitionBlock *p;
	long oldbase, base, len;
	struct rdb_gap *big;

	p = next_lowest_base(rdb, 1);
	if (p != NULL) {
		base = p->pb_Environment[DE_LOWCYL] * rdb->rdb_CylBlocks;
		if (base > 128)
			register_gap(128, base - 128);
	}

	while (p != NULL) {
		len = (p->pb_Environment[DE_UPPERCYL] + 1 -
		    p->pb_Environment[DE_LOWCYL]) * rdb->rdb_CylBlocks;

		oldbase = base + len;

		p = next_lowest_base(rdb, oldbase);
		if (p != NULL) {
			base = p->pb_Environment[DE_LOWCYL] * rdb->rdb_CylBlocks;
			register_gap(oldbase, base - oldbase);
		}
	}

	p = highest_base(rdb);
	if (p != NULL) {
		base = p->pb_Environment[DE_LOWCYL] * rdb->rdb_CylBlocks;
		len = (p->pb_Environment[DE_UPPERCYL] + 1 -
		    p->pb_Environment[DE_LOWCYL]) * rdb->rdb_CylBlocks;
		base += len;
		if (disklen > 0 && disklen > base)
			register_gap(base, disklen - base);
	} else if (disklen > 128)
		register_gap(128, disklen - 128);

	big = biggest_gap();

	if (big == NULL)
		exit(1);

	printf("%d %d\n", big->base, big->length);
	exit(0);
}



add_loadseg(rdb)
struct RigidDiskBlock *rdb;
{
struct FileSysHeaderBlock *p, *q;
int fd;

	fd = open(loadseg_file, 0);
	if (fd < 0) {
		fprintf(stderr, "can't open %s", loadseg_file);
		perror("");
		exit(1);
	}

	p = create_fhb(fd);

	if (rdb->rdb_FileSysHeaderList == NULL)
		rdb->rdb_FileSysHeaderList = p;
	else {
		q = rdb->rdb_FileSysHeaderList;
		while (q->fhb_Next != NULL)
			q = q->fhb_Next;

		q->fhb_Next = p;
	}

	close(fd);
}


add_partition(rdb, argc, argv)
struct RigidDiskBlock *rdb;
int argc;
char **argv;
{
struct PartitionBlock *oldp = NULL;
struct PartitionBlock *p;
struct PartitionBlock *new;
long start;
long len;
extern int optind;
long base;

	start = my_atol(argv[optind + 1]);
	len = my_atol(argv[optind + 2]);

	p = rdb->rdb_PartitionList;

	if (p)
		base = p->pb_Environment[DE_LOWCYL] * rdb->rdb_CylBlocks;
	while (p && base < start) {
		oldp = p;
		p = p->pb_Next;

		if (p != NULL)
			base = p->pb_Environment[DE_LOWCYL] * rdb->rdb_CylBlocks;
	}

	new = create_pb(rdb, argv[optind], start, len);

	if (oldp == NULL) {
		new->pb_Next = rdb->rdb_PartitionList;
		rdb->rdb_PartitionList = new;
	} else {
		new->pb_Next = oldp->pb_Next;
		oldp->pb_Next = new;
	}
}


edit_rdb(rdb)
struct RigidDiskBlock *rdb;
{
struct PartitionBlock *p;
int fd;

	p = rdb->rdb_PartitionList;

	partition--;			/* number partitions from 1 */
	while (p && partition > 0) {
		p = p->pb_Next;
		partition--;
	}

	if (partition || !p) {
		fprintf(stderr, "rdb: no such partition\n");
		exit(1);
	}

	if (boot_flg == -1
	&&  mount_flg == -1
	&&  custom_blocks == -1
	&&  boot_pri == -1
	&&  env_reserved == -1
	&&  fs_type == -1) {
		show_part(rdb, p);
		exit(0);
	}

	if (boot_flg == 0)
		p->pb_Flags &= ~PBFF_BOOTABLE;
	else if (boot_flg == 1) {
		p->pb_Flags |= PBFF_BOOTABLE;
		p->pb_Flags &= ~PBFF_NOMOUNT;	/* make it mountable too */
	}

	if (mount_flg == 1)
		p->pb_Flags &= ~PBFF_NOMOUNT;
	else if (mount_flg == 0)
		p->pb_Flags |= PBFF_NOMOUNT;

/*
 *  If we're sticking values into the environment vector, make sure
 *  the tablesize is big enough
 */

	if (custom_blocks != -1
	||  boot_pri != -1
	||  fs_type != -1) {
		if (p->pb_Environment[DE_TABLESIZE] < 19)
			p->pb_Environment[DE_TABLESIZE] = 19;
/*
 *  Todo:  we're extending the environment vector, but not filling in
 *		any default values.  Perhaps we should.
 */
	}

	if (env_reserved != -1)
		p->pb_Environment[DE_RESERVEDBLKS] = env_reserved;

	if (custom_blocks != -1)
		p->pb_Environment[DE_BOOTBLOCKS] = custom_blocks;

	if (boot_pri != -1)
		p->pb_Environment[DE_BOOTPRI] = boot_pri;

	if (fs_type != -1)
		p->pb_Environment[DE_DOSTYPE] = fs_type;
}


struct RigidDiskBlock *
create_rdb(argc, argv)
int argc;
char **argv;
{
char dummy[BSIZE];
struct RigidDiskBlock *p;
struct PartitionBlock *q;
struct PartitionBlock *qold = NULL;
extern int optind;
int i;
long start, len;

	assert((argc - optind) % 3 == 0);

	p = (struct RigidDiskBlock *) my_malloc(BSIZE);

	p->rdb_ID = IDNAME_RIGIDDISK;
	p->rdb_HostID = 7;
	p->rdb_BlockBytes = BSIZE;
	p->rdb_Flags = RDBFF_NORESELECT;  /* because AmigaDos was broken */
	if (lastid_flg == 1)
		p->rdb_Flags |= RDBFF_LASTTID;
		
	p->rdb_Heads = 1;
	p->rdb_Sectors = sectors_per_track;
	p->rdb_CylBlocks = p->rdb_Sectors * p->rdb_Heads;

	for (i = 0; i < 6; i++)
		p->rdb_Reserved1[i] = 0xFFFFFFFF;

	if (optind < argc) {
		start = my_atol(argv[optind + 1]);
		len = my_atol(argv[optind + 2]);
		qold = create_pb(p, argv[optind], start, len);
		p->rdb_PartitionList = qold;
		optind += 3;
	}

	while (optind < argc) {
		start = my_atol(argv[optind + 1]);
		len = my_atol(argv[optind + 2]);
		q = create_pb(p, argv[optind], start, len);
		qold->pb_Next = q;
		qold = q;
		optind += 3;
	}

	set_fixed_sizes(p);
	return(p);
}


set_fixed_sizes(rdb)
struct RigidDiskBlock *rdb;
{
	struct PartitionBlock *p;
	long lowcyl = 100;
	long highcyl;

	highcyl = disklen / rdb->rdb_Sectors;

	for (p = rdb->rdb_PartitionList; p; p = p->pb_Next) {
		if (lowcyl > p->pb_Environment[DE_LOWCYL])
			lowcyl = p->pb_Environment[DE_LOWCYL];
		if (highcyl < p->pb_Environment[DE_UPPERCYL])
			highcyl = p->pb_Environment[DE_UPPERCYL];
	}

	rdb->rdb_Cylinders = highcyl;
	rdb->rdb_RDBBlocksHi = lowcyl * rdb->rdb_CylBlocks - 1;
	rdb->rdb_HiCylinder = highcyl - 1;
}


struct PartitionBlock *
create_pb(q, name, start, len)
struct RigidDiskBlock *q;
char *name;
long start;
long len;
{
struct PartitionBlock *p;
long end;

	p = (struct PartitionBlock *) my_malloc(BSIZE);

	if ( (start % q->rdb_CylBlocks) != 0
	||   (len % q->rdb_CylBlocks) != 0) {
		fprintf(stderr,
"rdb: partition start and length blocks must lie on a cylinder boundary\n");
		exit(1);
	}

	start = start / q->rdb_CylBlocks;
	len = len / q->rdb_CylBlocks;
	end = start + len - 1;

	p->pb_ID = IDNAME_PARTITION;
	p->pb_HostID = 7;
	strncpy(p->pb_DriveName, name, 32);
	p->pb_DriveName[31] = '\0';

	p->pb_Environment[DE_TABLESIZE]		= 19;
	p->pb_Environment[DE_SIZEBLOCK]		= 128;
	p->pb_Environment[DE_NUMHEADS]		= 1;
	p->pb_Environment[DE_SECSPERBLK]	= 1;
	p->pb_Environment[DE_BLKSPERTRACK]	= q->rdb_CylBlocks;
	p->pb_Environment[DE_LOWCYL]		= start;
	p->pb_Environment[DE_UPPERCYL]		= end;
	p->pb_Environment[DE_NUMBUFFERS]	= 30;
	p->pb_Environment[DE_MAXTRANSFER]	= 0xFFFFFF;
	p->pb_Environment[DE_MASK]		= 0xFFFFFFFC;
	p->pb_Environment[DE_DOSTYPE]		= 0x72657376;	/* "resv" */

	return(p);
}


struct FileSysHeaderBlock *
create_fhb(fd)
int fd;			/* open file description for loadseg file */
{
struct FileSysHeaderBlock *p;

	p = (struct FileSysHeaderBlock *) my_malloc(BSIZE);

	p->fhb_ID = IDNAME_FILESYSHEADER;
	p->fhb_HostID = 7;
	p->fhb_DosType = 0x444f5301;
	p->fhb_Version = 2;
	p->fhb_PatchFlags = 0x180;
	p->fhb_GlobalVec = -1;

	p->fhb_SegListBlocks = create_lsb(fd);

	return(p);
}


struct LoadSegBlock *
create_lsb(fd)
int fd;			/* open file description for loadseg file */
{
struct LoadSegBlock *first, *p;
int n;

	p = (struct LoadSegBlock *) my_malloc(BSIZE);

	p->lsb_ID = IDNAME_LOADSEG;
	p->lsb_HostID = 7;

	n = read(fd, p->lsb_LoadData, 123 * sizeof(long));

	if (n < 0) {
		perror("bad read of loadseg file");
		exit(1);
	} else if (n == 0) {
		free(p);
		return(NULL);
	} else
		p->lsb_Next = create_lsb(fd);

	assert(n % 4 == 0);			/* even longwords */
	p->lsb_SummedLongs = (n + 20) / sizeof(long);

	return(p);
}


show_rdb(p)
struct RigidDiskBlock *p;
{

	if (!p) {
		fprintf(stderr, "cannot find rigid disk block\n");
		exit(1);
	}

	show_partitions(p);
}


show_partitions(p)
struct RigidDiskBlock *p;
{
struct PartitionBlock *q;
int i;
int head_count = 0;

	assert(p != NULL);

	q = p->rdb_PartitionList;

	if (show_header)
	    if (no_flags)
		printf("#    %-15s %8s %8s  %5s\n\n",
					"Name", "Start", "Blocks", "Size");
	    else
		printf("#    %-15s %8s %8s  %5s      Flags\n\n",
					"Name", "Start", "Blocks", "Size");

	while (q) {
		head_count++;
		if (show_header)
			printf("%2d:  ", head_count);
	
		show_part(p, q);
		q = q->pb_Next;
	}

}


long
partition_size(rdb, p)
struct RigidDiskBlock *rdb;
struct PartitionBlock *p;
{
long len;
long size;

	len = (p->pb_Environment[DE_UPPERCYL] + 1 -
	    p->pb_Environment[DE_LOWCYL]) * rdb->rdb_CylBlocks;

	size = len * rdb->rdb_BlockBytes;

	return size;
}


show_part(rdb, p)
struct RigidDiskBlock *rdb;
struct PartitionBlock *p;
{
long len, base;
long size;
int i;
char s[100];
char extras[100];

	assert(rdb != NULL);
	assert(p != NULL);

	base = p->pb_Environment[DE_LOWCYL] * rdb->rdb_CylBlocks;
	len = (p->pb_Environment[DE_UPPERCYL] + 1 -
	    p->pb_Environment[DE_LOWCYL]) * rdb->rdb_CylBlocks;
	size = partition_size(rdb, p);

	if (size < 1024)
		sprintf(s, "%5ld bytes", size);
	else if (size < 1048576)
		sprintf(s, "%5ld K    ", size / 1024);
	else
		sprintf(s, "%5ld MB   ", size / 1048576);


	if (no_flags)
		*extras = '\0';
	else
		sprintf(extras, "%c%c P%d C%d F0x%lx",
				(p->pb_Flags & PBFF_NOMOUNT) ? 'm' : 'M',
				(p->pb_Flags & PBFF_BOOTABLE) ? 'B' : 'b',
				p->pb_Environment[DE_BOOTPRI],
				p->pb_Environment[DE_BOOTBLOCKS],
				p->pb_Environment[DE_DOSTYPE]);

	if (show_header)
		printf("%-15s %8ld %8ld %s %s\n",
					p->pb_DriveName, base, len, s, extras);
	else
		printf("%s %ld %ld\n", p->pb_DriveName, base, len);

	if (verbose) {
		printf("\tflags:  %X\n", p->pb_Flags);
		printf("\t0:  ");
		for (i = 0; i < 6; i++)
			printf("%.8x ", p->pb_Environment[i]);
		printf("\n\t6:  ");

		for (i = 6; i < 13; i++)
			printf("%.8x ", p->pb_Environment[i]);
		printf("\n\t13: ");

		for (i = 13; i < 20; i++)
			printf("%.8x ", p->pb_Environment[i]);
		printf("\n\n");
	}
}



struct RigidDiskBlock *
get_rdb(fd)
int fd;
{
long i;
struct RigidDiskBlock *p;

	p = (struct RigidDiskBlock *) my_malloc(BSIZE);

	i = find_rdb_block(fd, p);
	if (i < 0) {
		free(p);
		return(NULL);
	}

	rdb_found_pos = i;

	p->rdb_BadBlockList      = get_bbb(fd, (long) p->rdb_BadBlockList);
	p->rdb_PartitionList     = get_pb(fd,  (long) p->rdb_PartitionList);
	p->rdb_FileSysHeaderList = get_fhb(fd, (long) p->rdb_FileSysHeaderList);
	p->rdb_DriveInit         = get_lsb(fd, (long) p->rdb_DriveInit);

	return(p);
}


struct BadBlockBlock *
get_bbb(fd, pos)
int fd;
long pos;
{
struct BadBlockBlock *p;

	if (!validblock(pos))
		return(NULL);

	p = (struct BadBlockBlock *) my_malloc(BSIZE);

	lseek(fd, pos * BSIZE, 0);
	if (read(fd, p, BSIZE) != BSIZE) {
		perror("bad read");
		free(p);
		return(NULL);
	}

	if (p->bbb_ID != IDNAME_BADBLOCK || !checksum(p, p->bbb_SummedLongs)) {
		fprintf(stderr, "warning:  invalid bad block list entry\n");
		free(p);
		return(NULL);
	}

	p->bbb_Next = get_bbb(fd, (long) p->bbb_Next);

	return(p);
}


struct PartitionBlock *
get_pb(fd, pos)
int fd;
long pos;
{
struct PartitionBlock *p;

	if (!validblock(pos))
		return(NULL);

	p = (struct PartitionBlock *) my_malloc(BSIZE);

	lseek(fd, pos * BSIZE, 0);
	if (read(fd, p, BSIZE) != BSIZE) {
		perror("bad read");
		free(p);
		return(NULL);
	}

	if (p->pb_ID != IDNAME_PARTITION || !checksum(p, p->pb_SummedLongs)) {
		fprintf(stderr, "warning:  invalid partition list entry\n");
		free(p);
		return(NULL);
	}

	btocstr(&p->pb_DriveName);

	p->pb_Next = get_pb(fd, (long) p->pb_Next);
	return(p);
}


struct FileSysHeaderBlock *
get_fhb(fd, pos)
int fd;
long pos;
{
struct FileSysHeaderBlock *p;

	if (!validblock(pos))
		return(NULL);

	p = (struct FileSysHeaderBlock *) my_malloc(BSIZE);

	lseek(fd, pos * BSIZE, 0);
	if (read(fd, p, BSIZE) != BSIZE) {
		perror("bad read");
		free(p);
		return(NULL);
	}

	if (p->fhb_ID != IDNAME_FILESYSHEADER
	||  !checksum(p, p->fhb_SummedLongs)) {
		fprintf(stderr, "warning:  invalid file system list entry\n");
		free(p);
		return(NULL);
	}

	p->fhb_Next = get_fhb(fd, (long) p->fhb_Next);
	p->fhb_SegListBlocks = get_lsb(fd, p->fhb_SegListBlocks);

	return(p);
}


struct LoadSegBlock *
get_lsb(fd, pos)
int fd;
long pos;
{
struct LoadSegBlock *p;

	if (!validblock(pos))
		return(NULL);

	p = (struct LoadSegBlock *) my_malloc(BSIZE);

	lseek(fd, pos * BSIZE, 0);
	if (read(fd, p, BSIZE) != BSIZE) {
		perror("bad read");
		free(p);
		return(NULL);
	}

	if (p->lsb_ID != IDNAME_LOADSEG
	||  !checksum(p, p->lsb_SummedLongs)) {
		fprintf(stderr, "warning:  invalid loadseg list entry\n");
		free(p);
		return(NULL);
	}

	p->lsb_Next = get_lsb(fd, (long) p->lsb_Next);

	return(p);
}



put_rdb(fd, p, sec)
int fd;
struct RigidDiskBlock *p;
long sec;			/* where to put the rdb */
{
long s;

	assert(p != NULL);

	s = sec + 1;		/* lists will follow the rdb */

	p->rdb_BadBlockList      = put_bbb(fd, p->rdb_BadBlockList, &s);
	p->rdb_PartitionList     = put_pb(fd, p->rdb_PartitionList, &s);
	p->rdb_FileSysHeaderList = put_fhb(fd, p->rdb_FileSysHeaderList, &s);
	p->rdb_DriveInit         = put_lsb(fd, p->rdb_DriveInit, &s);

	p->rdb_HighRDSKBlock = s - 1;

	p->rdb_SummedLongs = sizeof(*p) / sizeof(long);
	p->rdb_ChkSum = 0;
	p->rdb_ChkSum = - sum2(p, p->rdb_SummedLongs);

	lseek(fd, sec * BSIZE, 0);

	if (write(fd, p, BSIZE) != BSIZE) {
		perror("bad rdb write");
		return(FALSE);
	}

	return(TRUE);
}


clear_rdb(fd, pos)
int fd;
int pos;
{
	int i;
	long dummy = 0;

	for (i = 0; i < pos; i++) {
		lseek(fd, i * BSIZE, 0);
		if (write(fd, &dummy, sizeof(dummy)) != sizeof(dummy)) {
			perror("bad rdb clear write");
			return(FALSE);
		}
	}

	return(TRUE);
}


struct BadBlockBlock *
put_bbb(fd, p, s)
int fd;
struct BadBlockBlock *p;
long *s;
{
long sec;

	if (p == NULL)
		return((struct BadBlockBlock *) 0xFFFFFFFF);

	sec = (*s)++;

	p->bbb_Next = put_bbb(fd, p->bbb_Next, s);

	p->bbb_SummedLongs = sizeof(*p) / sizeof(long);
	p->bbb_ChkSum = 0;
	p->bbb_ChkSum = - sum2(p, p->bbb_SummedLongs);

	lseek(fd, sec * BSIZE, 0);

	if (write(fd, p, BSIZE) != BSIZE) {
		perror("bad write");
		return((struct BadBlockBlock *) 0xFFFFFFFF);
	}

	return((struct BadBlockBlock *) sec);

}


struct PartitionBlock *
put_pb(fd, p, s)
int fd;
struct PartitionBlock *p;
long *s;
{
long sec;

	if (p == NULL)
		return((struct PartitionBlock *) 0xFFFFFFFF);

	sec = (*s)++;

	p->pb_Next = put_pb(fd, p->pb_Next, s);

	ctobstr(&p->pb_DriveName);

	p->pb_SummedLongs = sizeof(*p) / sizeof(long);
	p->pb_ChkSum = 0;
	p->pb_ChkSum = - sum2(p, p->pb_SummedLongs);

	lseek(fd, sec * BSIZE, 0);

	if (write(fd, p, BSIZE) != BSIZE) {
		perror("bad write");
		return((struct PartitionBlock *) 0xFFFFFFFF);
	}

	return((struct PartitionBlock *) sec);
}


struct FileSysHeaderBlock *
put_fhb(fd, p, s)
int fd;
struct FileSysHeaderBlock *p;
long *s;
{
long sec;

	if (p == NULL)
		return((struct FileSysHeaderBlock *) 0xFFFFFFFF);

	sec = (*s)++;

	p->fhb_Next = put_fhb(fd, p->fhb_Next, s);
	p->fhb_SegListBlocks = put_lsb(fd, p->fhb_SegListBlocks, s);

	p->fhb_SummedLongs = sizeof(*p) / sizeof(long);
	p->fhb_ChkSum = 0;
	p->fhb_ChkSum = 0 - sum2(p, p->fhb_SummedLongs);

	lseek(fd, sec * BSIZE, 0);

	if (write(fd, p, BSIZE) != BSIZE) {
		perror("bad write");
		return((struct FileSysHeaderBlock *) 0xFFFFFFFF);
	}

	return((struct FileSysHeaderBlock *) sec);
}


struct LoadSegBlock *
put_lsb(fd, p, s)
int fd;
struct LoadSegBlock *p;
long *s;
{
long sec;

	if (p == NULL)
		return((struct LoadSegBlock *) 0xFFFFFFFF);

	sec = (*s)++;

	p->lsb_Next = put_lsb(fd, p->lsb_Next, s);

#if 0
	p->lsb_SummedLongs = sizeof(*p) / sizeof(long);
#endif
	p->lsb_ChkSum = 0;
	p->lsb_ChkSum = 0 - sum2(p, p->lsb_SummedLongs);

	lseek(fd, sec * BSIZE, 0);

	if (write(fd, p, BSIZE) != BSIZE) {
		perror("bad write");
		return((struct LoadSegBlock *) 0xFFFFFFFF);
	}

	return((struct LoadSegBlock *) sec);
}




find_rdb_block(fd, p)
int fd;
struct RigidDiskBlock *p;
{
long i;

	for (i = 0; i < RDB_LOCATION_LIMIT; ++i) {
		lseek(fd, i * BSIZE, 0);
		if (read(fd, p, BSIZE) != BSIZE) {
			perror("rdb read error");
			return(-1);
		}
		if (p->rdb_ID == IDNAME_RIGIDDISK
		&&  checksum(p, p->rdb_SummedLongs)) {
			rdb_pos = i;
			return(i);
		}
	}

	return(-1);
}


checksum(p, n)
long *p;
long n;
{
	long a = 0;

	while (n--)
		a += *p++;
	return (a == 0);
}


long
sum2(p, n)
long *p;
int n;
{
	long a = 0;

	while (n--)
		a += *p++;
	return (a);
}


/*
 *  convert C string to BCPL string
 */

ctobstr (str)
char *str;
{
	int len, i;

	len = strlen(str);
	for (i = len; i > 0; i--)
		str[i] = str[i-1];

	str[0] = (char) len;
}


/*
 *  convert BCPL string to C string
 */

btocstr (str)
char *str;
{
	int len, i;

	len = str[0];
	for (i = 0; i < len; i++)	/* note: overlapping copy */
		str[i] = str[i+1];

	str[len] = '\0';
}


validblock(b)
long b;
{

	return (b != 0xFFFFFFFF);
}


char *
my_malloc(size)
unsigned size;
{
char *p;
extern char *malloc();
extern void *memset();

	p = malloc(size);
	if (p == NULL) {
		fprintf(stderr, "fatal error:  can't malloc %d bytes\n", size);
		exit(1);
	}

	memset(p, 0, size);
	return(p);
}


long
my_atol(s)
char *s;
{
long l;

	if (strncmp(s, "0x", 2) == 0)
		sscanf(s + 2, "%lx", &l);
	else
		sscanf(s, "%ld", &l);

	return(l);
}

