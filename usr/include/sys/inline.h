/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_INLINE_H
#define _SYS_INLINE_H

#ident	"@(#)head.sys:sys/inline.h	11.9"

#if !defined(CXREF)  &&  !defined(lint)

#if WE_KNOW_WHAT_STREND_IS_SUPPOSED_TO_DO
#ifdef __GNUC__
static inline char *strend(char *cp)
{
	while (*cp++)
		;
	return cp;
}
#else
asm	char *
strend(cp)
{
%	mem	cp;

	mov.l	cp,%a0
	tst.b	(%a0)+
	bne	~-2
}
#endif /* GNUC */
#endif /* WE_KNOW_WHAT_STREND_IS_SUPPOSED_TO_DO */

#ifdef __GNUC__
static inline int strlen(char *cp)
{
	char *x = cp;
	while (*x++)
		;
	return x-cp-1;
}
#else
asm	int
strlen(cp)
{
%	mem	cp;

	mov.l	cp,%a1
	tst.b	(%a1)+
	bne	~-2
	sub.l	cp,%a1
	sub.l	&1,%a1
	mov.l	%a1,%d0
}
#endif /* GNUC */

#ifndef __GNUC__
asm	char *
strcpy(dp, sp)
{
%	mem	dp, sp;

	mov.l	sp,%a0
	mov.l	dp,%a1
	mov.b	(%a0)+,(%a1)+
	bne.w	~-2
}
#endif /* !GNUC */

#ifndef __GNUC__
asm	char *
strcat(dp, sp)
{
%	mem	dp, sp;

	mov.l	dp,%a0
	tst.b	(%a0)+
	bne	~-2
	mov.l	%a0,%a1
	mov.l	sp,%a0
	mov.b	(%a0)+,(%a1)+
	bne	~-2
}
#endif /* !GNUC */

#ifdef __GNUC__
static inline void struct_zero(void *addr, unsigned int len)
{
  unsigned long *p = (unsigned long *)addr;
  len >>= 2;
  do *p++ = 0;
  while (--len);
}
#else
asm	void
struct_zero(addr, len)
{
%	mem	addr, len;	lab loop;
	mov.l	addr, %a0
	mov.l	len, %d0
	lsr.l	&1, %d0
	mov.l	&0, %d1
loop:
	mov.w	%d1,(%a0)+
	sub.l	&1,%d0
	bne	loop
}
#endif /* GNUC */

#ifdef __GNUC__
static inline int _spl0()
{int __s;
 asm volatile("mov.w %%sr,%0;mov.w &0x2000,%%sr":"=d"(__s):);
 return __s;
}
#else
asm	int
_spl0()
{
	mov.w	%sr,%d0
	mov.w	&0x2000,%sr
}
#endif /* GNUC */

#ifdef __GNUC__
static inline int _spl1()
{int __s;
 asm volatile("mov.w %%sr,%0;mov.w &0x2100,%%sr":"=d"(__s):);
 return __s;
}
#else
asm	int
_spl1()
{
	mov.w	%sr,%d0
	mov.w	&0x2100,%sr
}
#endif /* GNUC */

#ifdef __GNUC__
static inline int _spl2()
{int __s;
 asm volatile("mov.w %%sr,%0;mov.w &0x2200,%%sr":"=d"(__s):);
 return __s;
}
#else
asm	int
_spl2()
{
	mov.w	%sr,%d0
	mov.w	&0x2200,%sr
}
#endif /* GNUC */

#ifdef __GNUC__
static inline int _spl3()
{int __s;
 asm volatile("mov.w %%sr,%0;mov.w &0x2300,%%sr":"=d"(__s):);
 return __s;
}
#else
asm	int
_spl3()
{
	mov.w	%sr,%d0
	mov.w	&0x2300,%sr
}
#endif /* GNUC */

#ifdef __GNUC__
static inline int _spl4()
{int __s;
 asm volatile("mov.w %%sr,%0;mov.w &0x2400,%%sr":"=d"(__s):);
 return __s;
}
#else
asm	int
_spl4()
{
	mov.w	%sr,%d0
	mov.w	&0x2400,%sr
}
#endif /* GNUC */

#ifdef __GNUC__
static inline int _spl5()
{int __s;
 asm volatile("mov.w %%sr,%0;mov.w &0x2500,%%sr":"=d"(__s):);
 return __s;
}
#else
asm	int
_spl5()
{
	mov.w	%sr,%d0
	mov.w	&0x2500,%sr
}
#endif /* GNUC */

#ifdef __GNUC__
static inline int _spl6()
{int __s;
 asm volatile("mov.w %%sr,%0;mov.w &0x2600,%%sr":"=d"(__s):);
 return __s;
}
#else
asm	int
_spl6()
{
	mov.w	%sr,%d0
	mov.w	&0x2600,%sr
}
#endif /* GNUC */

#ifdef __GNUC__
static inline int _spl7()
{int __s;
 asm volatile("mov.w %%sr,%0;mov.w &0x2700,%%sr":"=d"(__s):);
 return __s;
}
#else
asm	int
_spl7()
{
	mov.w	%sr,%d0
	mov.w	&0x2700,%sr
}
#endif /* GNUC */


#define FAST_INTERRUPTS

#define spl0	_spl0
#define spl1	_spl1
#define spl2	_spl2
#define spl3	_spl3
#define spl4	_spl4
#define	spl5	_spl4
#define	spl6	_spl4
#define	spl7	_spl4

#define splpp spl5

#define splni spl6

#define splvm spl6

#define splimp spl7

#define spltty spl7

#define splhi spl7

#ifdef __GNUC__
static inline int splx(unsigned short opsw)
{int __s;
 asm volatile("mov.w %%sr,%0;mov.w %1,%%sr":"=&d"(__s):"d"(opsw));
 return __s;
}
#else
asm	int
splx(opsw)
{
%	mem	opsw;
	mov.w	%sr,%d0
	mov.l	opsw,%d1
	mov.w	%d1,%sr
}
#endif /* GNUC */

#ifdef __GNUC__
#define movpsw(dest) asm volatile("mov.w %%sr,%0":"=d"(dest):)
#else
asm	void
movpsw(dest)
{
%	mem	dest;
	mov.w	%sr,dest
}
#endif /* GNUC */

#ifdef __GNUC__
#define kvtophys(va) \
({paddr_t __x;asm volatile("mov.l %1,%0":"=g"(__x):"g"(va));__x;})
#else
asm paddr_t
kvtophys(va)
{
%	mem	va;
	mov.l	va,%d0
}
#endif /* GNUC */

#else

#if defined(__STDC__)

extern char *strend(char *);
extern int strlen(char *);
extern char *strcpy(char *, char *);
extern char *strcat(char *, char *);
extern int struct_zero(caddr_t, int);

extern int splx(int);
extern int spl0(void);
extern int spl1(void);
extern int spl4(void);
extern int spl5(void);
extern int spl6(void);
extern int spl7(void);
extern int splhi(void);
extern int splpp(void);
extern int splvm(void);
extern int splni(void);
extern int splimp(void);
extern int spltty(void);

extern void movpsw(int *);
extern paddr_t kvtophys(caddr_t);

#else

extern char	*strend();
extern int	strlen();
extern char 	*strcpy();
extern char	*strcat();
extern int	spl0();
extern int	spl1();
extern int	spl4();
extern int	spl5();
extern int	spl6();
extern int	spl7();
extern int	splx();
extern int	splpp();
extern int	splni();
extern int	splvm();
extern int	splimp();
extern int	spltty();
extern int	splhi();
extern void	movpsw();
extern paddr_t	kvtophys();
extern int struct_zero();

#endif	/* __STDC__ */

#endif	/* !defined(CXREF)  &&  !defined(lint)	*/

#ifdef	KPERF  /* This is for kernel performance tool */
#ifdef __GNUC__
static inline int get_spl(void)
{int __s;
 asm volatile("mov.w %%sr,%0":"=d"(__s):);
 return __s;
}
#else
asm	int
get_spl()
{
	mov.w	%sr,%d0
}
#endif /* GNUC */
#endif	/* KPERF */

#endif	/* _SYS_INLINE_H */
