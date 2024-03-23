#include "../include/msh.h"

size_t	count_args(char **ptrs)
{
	char	**ptr;

	ptr = ptrs;
	while (ptr && *ptr)
		ptr++;
	return (ptr - ptrs);
}

int	ft_cd(char **args)
{
	// PWD OLDPWD
	unsigned int	count;
	int				i;
	char			*home;
	char			*target;
	
	char 	*s;

	if (!args)
		return (1);
	count = count_args(args);
	if (count < 1)
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (1);
		}
		target = home;
	}
	else
		target = args[0];
	if (*target)
	{
		if (chdir(target))
		{
			ft_putstr_fd("cd: ", 2);
			perror(target);
			return (1);
		}
		else
		{
			s = getcwd(NULL, 0);
			printf("%s\n", s);
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	ft_cd(argv + 1);
}