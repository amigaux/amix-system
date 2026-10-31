/*
**	This is NOT the getNAME.c from BSD or SVR4
**
**	I wrote this myself, from scratch, to deal with
**	compresed man-page source files.
**
**	David C. Miller
*/

# include	<stdio.h>
# include	<string.h>
# include	<unistd.h>
# include	<signal.h>

main ( int argc, char * argv [] )
{
    int		piped;
    int		len;
    int		getnext;
    int		lnum = 0;

    char	buff[BUFSIZ];
    char	tmp[BUFSIZ];
    char	line[BUFSIZ*4];
    char	*section;
    
    FILE	*fp;
    
    if (argc < 3)
	exit (1);

    (void) signal(SIGPIPE, SIG_IGN);
    
    section = argv[1];

    argc -= 2;
    argv += 2;

    for (; *argv; argv++)
    {
	strcpy(buff, *argv);

	len = strlen(buff);

	piped = 0;

	if (buff[len - 2] == '.' && buff[len - 1] == 'Z')
	{
	    buff[len - 2] = '\0';
	    
	    strcpy(tmp, "zcat 2>/dev/null ");
	    strcat(tmp, buff);

	    if ((fp = popen(tmp, "r")) == NULL)
	    {
		sprintf(tmp, "Can't open %s", buff);
		perror(tmp);
		continue;
	    }

	    piped = 1;
	}
	else
	    if ((fp = fopen(buff, "r")) == NULL)
	    {
		sprintf(tmp, "Can't open %s", buff);
		perror(tmp);
		continue;
	    }

	getnext = 0;
	
	while(fgets(line, 512, fp) != 0)
	{
	    lnum++;
	    
	    if (strcmp(line, ".SH NAME\n") == 0)
	    {
		getnext = 1;
	    }
	    else
	    {
		if (getnext == 1)
		{
		    line[strlen(line) - 1] = '\0';
		    sprintf(tmp, "%s(%s)", buff, section);
		    printf("%-17s\007%s", tmp, line);
		    getnext = 2;
		    continue;
		}
		else
		    if (getnext == 2)
		    {
		        if (line[0] == '.')
			{
			    putc('\n', stdout);
			    break;
			}
			line[strlen(line) - 1] = '\0';
			printf(" %s", line);
		    }
	    }
	}

	if (piped)
	    pclose(fp);
	else
	    fclose(fp);
    }
}
