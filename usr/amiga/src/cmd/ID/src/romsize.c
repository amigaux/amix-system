#include <osfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fd;
    int x;
    
    if ((fd = open("/dev/mem", O_RDONLY)) < 0 ||
	(lseek(fd, 0xFFFFEC, 0) != 0xFFFFEC) ||
	(read(fd, &x, sizeof x) != sizeof x))
    {
	perror("Can't get ROM size");
	return 1;
    }
    printf("%d\n", x);
    return 0;
}
