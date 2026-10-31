#include "bitfields.h"

char *
compilebitfield(int value, struct Bitfields *B)
{
    int bitfield = 0, allfields = 0;
    char buildit[1024];
    struct Bitfields *C;
    static char retval[1024];

    retval[0] = '\0';

    /* handle `type' */

    for (C=B; C->flags != B_end; C++)
    {
	if (C->flags == B_type && C->value == value)
	    return C->name;
    }

    if (B->flags == B_type)
    {
	/* Unknown type */

	(void) sprintf(retval, "0x%x", value);
	return retval;
    }

    /* handle bitnums and bitvals */
    for (C=B; C->flags != B_end; C++)
    {
	if (C->flags == B_bitnum || C->flags == B_bitval)
	{
	    int i=0, v = C->value;

	    if (v)
		for (; !(v&1); ++i)
		    v >>= 1;

	    if (C->flags == B_bitval)
		(void) sprintf(buildit, "%s=0x%x ", C->name,
			       ((value >> i) & v));
	    else
		(void) sprintf(buildit, "%s=%d ", C->name, ((value >> i) & v));

	    strcat(retval, buildit);
	}
	else if (C->flags == B_bitfield)
	    bitfield |= C->value;

	allfields |= C->value;
    }
    
    buildit[0] = '\0';

    /* handle bitfields */
    if (bitfield)
    {
	int displayed = 0;

	for (C=B; C->flags != B_end; C++)
	{
	    if (C->flags == B_bitfield)
	    {
		if (value&C->value)
		{
		    if (displayed)
		    {
			strcat(buildit, "|");
		    }
		    strcat(buildit, C->name);
		    displayed=1;
		}
	    }
	}

	if (displayed)
	    strcat(retval, buildit);
    }

    if (retval[0] == '\0')
	sprintf(retval, "0x%x", value);
    else
    {
	if (value & (~allfields))
	{
	    (void) sprintf(buildit, "%sUnknown(0x%x)", bitfield ? " ": "",
			   value);
	    strcat(retval, buildit);
	}
    }

    return retval;
}
