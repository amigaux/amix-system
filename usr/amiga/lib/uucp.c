# include	<stdio.h>
# include	<stdlib.h>
# include	<unistd.h>
# include	<string.h>
# include	<fcntl.h>

# define	DEFAULT_SHELL	"/sbin/sh"


FILE * popen ( const char * cmd, const char * dir )
{
    int		fds[2];
    int		i;
    int		WR;
    int		RD;
    char	*shell;
    
    RD = (strchr(dir, 'r') != NULL);
    WR = (strchr(dir, 'w') != NULL);
    
    if (pipe(fds) == -1)
	return NULL;
    
    switch (fork())
    {
      case -1:
	close(fds[0]);
	close(fds[1]);
	return NULL;

      case 0:
	setsid();

	close(21);
	dup2(fds[1], 21);
	close(fds[1]);
	fds[1] = 21;
	
	for (i = 0; i < 20; i++)
	    close(i);
	
	if (WR)
	    dup2(fds[1], 0);

	if (RD)
	{
	    dup2(fds[1], 1);
	    dup2(fds[1], 2);
	}

	if (!WR)
	    open("/dev/null", O_RDONLY);
	
	if (!RD)
	{
	    open("/dev/null", O_WRONLY);
	    open("/dev/null", O_WRONLY);
	}

	close(21);
	if ((shell = getenv("SHELL")) == NULL)
	    shell = DEFAULT_SHELL;
	
	execl(shell, shell, "-c", cmd, NULL);
	exit(-1);
	
      default:
	close(fds[1]);
	return fdopen(fds[1], dir);
    }
}

int pclose ( FILE * fp )
{
    int	status;
    
    fclose(fp);
    wait(&status);
    return status;
}


int system ( const char * command )
{
    pid_t	cpid;
    pid_t	pid;
    int		status;

    if (command == NULL)
    {
	if (access(DEFAULT_SHELL, X_OK) == 0)
	    return 1;
	else
	    return 0;
    }

    switch (cpid = fork())
    {
      case -1:
	return -1;

      case 0:
	setsid();
	execl(DEFAULT_SHELL, DEFAULT_SHELL, "-c", command, NULL);
	exit(1);

      default:
	do
	{
	    pid = wait(&status);
	}
	while(pid != cpid && pid != -1);

	if (pid == -1)
		return -1;
	
	return status;
    }
}
