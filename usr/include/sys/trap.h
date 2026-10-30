
/*
 *	The following traps apply to the Motorola 68K
 *	family of processors.  These defines are used
 *	by os/trap.c in the UNIX kernel.
 */

/* MC68020 Vectors */

#define RESCHED		0	/* Pseudo trap to signal rescheduling */
#define BUS_ERROR       2	/* Bus Error/Segment Violation (SIGSEGV) */
#define ADDR_ERROR      3	/* Address Error (SIGBUS) */
#define ILL_INST        4	/* Illegal Instruction Trap (SIGILL) */
#define ZERO_DIV        5	/* Zero Divide Trap (SIGFPE) */
#define CHK_CHK2        6	/* CHK Trap (SIGSEGV) */
#define TRAPCC          7       /* and TRAPV */
#define PRIV            8	/* privileged instruction fault (SIGILL) */
#define TRACE           9	/* Trace Trap (SIGTRAP) */
#define LINE_A          10	/* Line 1010 Emulator Trap (SIGILL) */
#define LINE_F          11	/* Line 1111 Emulator Trap (SIGILL) */
#define CP_PROTOCOL     13	/* Coprocessor Protocol Violation (SIGILL) */
#define FORMAT_ERROR    14	/* Illegal Exception Format (SIGILL) */
#define UNINIT_INT      15	/* Uninitialized Interrupt Vector Error */
#define SPURIOUS        24	/* Spurious Interrupt */
#define AUTOVEC_1       25
#define AUTOVEC_2       26
#define AUTOVEC_3       27
#define AUTOVEC_4       28
#define AUTOVEC_5       29
#define AUTOVEC_6       30
#define AUTOVEC_7       31	
#define TRAP_0          32	/* TRAP 0 - System Call Trap */
#define TRAP_1          33	/* TRAP 1 - Breakpoint Trap (SIGTRAP) */
#define TRAP_2          34
#define TRAP_3          35
#define TRAP_4          36
#define TRAP_5          37
#define TRAP_6          38
#define TRAP_7          39
#define TRAP_8          40
#define TRAP_9          41
#define TRAP_10         42
#define TRAP_11         43
#define TRAP_12         44
#define TRAP_13         45
#define TRAP_14         46
#define TRAP_15         47
#define FPCP_UNORD      48
#define FPCP_INEXACT    49
#define FPCP_DIVZERO    50
#define FPCP_UNDERFLOW  51
#define FPCP_OPERAND    52
#define FPCP_OVERFLOW   53
#define FPCP_SIGNAN     54
#define FPCP_UNIMP      55	/* 68040 unimplemented data type */
#define PMMU_CONFIG     56
#define PMMU_ILLOP      57
#define PMMU_ACCESS     58

/* MC68020 stack formats	*/

#define FMT_N	 0		/* Normal Four Word	*/
#define FMT_T	 1		/* Throwaway Four Word	*/
#define FMT_6	 2		/* Normal Six Word	*/
#define FMT_M	 9		/* Coprocessor Mid-Instruction Exception  */
#define FMT_S	10		/* Short Bus Cycle Fault	*/
#define FMT_L	11		/* Long Bus Cycle Fault		*/
