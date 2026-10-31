# include	<sys/sysm68k.h>
# include	<pwd.h>
# include	<errno.h>
# include	<unistd.h>


register_user ( struct passwd * p )
{
    extern int	errno;

    /*
    **	This code assumes that pwd is a pointer to a struct passwd.
    **	and has been initialized by a getpwnam() or an equivalent.
    */

    if (strstr(p->pw_shell, "uucico"))
    {
	sysm68k(_m68k_LIMUSER, EUA_UUCP);
    }
    else
    {
	if (sysm68k(_m68k_LIMUSER, EUA_ADD_USER) == -1)
	{
	    if (errno == EACCES)
	    {
		if (p->pw_uid != 0)
		{
		    printf("This system is licensed for only %d users,\n",
			sysm68k(_m68k_LIMUSER, EUA_GET_LIM));
		    puts("and this session would exceed that limit.\n");
		    puts("You may try again later.\n");
		    sleep(2);
		    exit(1);
		}
	    }
	    else
		if (errno != EINVAL)
		{
		    perror("Couldn't get session license");
		    sleep(2);
		    exit(1);
		}
	}
    }
}
