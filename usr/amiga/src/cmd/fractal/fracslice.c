#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char  *progname;

int
main(int argc, char **argv)
{
    double startz = -2.00, endz = 2.00, increment_z = 0.01;
    char buffer[1024];

    progname = *argv++; --argc;

    while (startz <= endz)
    {
	sprintf(buffer, "%s -b -F frac%Lf.gif -q %Lf", "../fractal", startz,
		startz);
	system(buffer);

	startz += increment_z;
    }
}
