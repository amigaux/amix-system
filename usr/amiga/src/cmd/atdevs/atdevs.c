
#include	<stdio.h>

main() {
int i, fd;
char s[100];
char buf[512];

	for (i = 0; i < 7; i++) {
		if (i == 4)
			continue;

		sprintf(s, "/dev/dsk/c%dd0s0", i);
		fd = open(s, 0);
		if (fd < 0)
			continue;
		if (read(fd, buf, sizeof(buf)) == sizeof(buf))
			printf("%d\n", i);
		else
			close(fd);
	}
}

