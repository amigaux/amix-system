/*
 * Assertion verifier.
 */


#if NODEBUG
#define	assert(p)
#else
#define	assert(p)	if(! (p)) asfail(__FILE__, __LINE__, "p"); else
#endif
