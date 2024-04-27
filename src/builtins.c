/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:37:05 by eel-brah          #+#    #+#             */
/*   Updated: 2024/04/27 19:59:57 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

int	ft_cd(char **args)
{
	char			*home;
	char			*pwd;
	char			*current;
	char			*target;

	pwd = getenv("PWD");
	if (count_args(args) < 1)
	{
		home = getenv("HOME");
		if (!home)
			return (ft_putstr_fd("cd: HOME not set\n", 2), 1);
		target = home;
	}
	else
		target = args[0];
	if (*target)
	{
		// int a = chdir(target);
		// printf("chdir val %d\n", a);
		if (chdir(target))
			return (ft_putstr_fd("cd: ", 2), perror(target), 1);
		// if (a)
		// {
			// return (ft_putstr_fd("cd: ", 2), perror(target), 1);
		// }
		current = getcwd(NULL, 0);
		if (!current)
			return (perror("getcwd"), 1);
		if (!ft_setenv(environ, "OLDPWD=", pwd))
			return (free(current), 1);
		if (!ft_setenv(environ, "PWD=", current))
			return (free(current), 1);
		free(current);
	}
	return (0);
}

int	ft_echo(char **args)
{
	unsigned int	i;
	bool			new_line;

	// if (!args)
	// 	return (0);
	new_line = true;
	while (count_args(args) > 0 && !ft_strncmp(args[0], "-n", 2))
	{
		i = 1;
		while (args[0][i] == 'n')
			i++;
		if (args[0][i] && args[0][i] != ' ')
			break ;
		if (!args[0][i])
		{
			args++;
			new_line = false;
		}
	}
	while (*args)
	{
		ft_putstr_fd(*args++, 1);
		if (*args)
			ft_putchar_fd(' ', 1);
	}
	if (new_line)
		ft_putchar_fd('\n', 1);
	return (0);
}

int	ft_env(char **env, char **args)
{
	if (count_args(args) > 0)
	{
		print_error("env", "Invalid number of arguments");
		return (127);
	}
	ft_printenv_no_empty(env);
	return (0);
}

int	ft_pwd(char **args)
{
	char	*s;
	int		i;

	if (count_args(args) && **args == '-')
	{
		i = 0;
		while (args[0][i] == '-')
			i++;
		if (args[0][i])
		{
			print_error("pwd", "usage: pwd");
			return (1);
		}
	}
	s = getcwd(NULL, 0);
	if (!s)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", s);
	free(s);
	return (0);
}

int	ft_unset(char **env, char **args)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	r;

	r = 0;
	while (*args)
	{
		if (is_valid_variable_name(*args))
		{
			i = 0;
			while (env[i])
			{
				// add condition if there is just the name without equale like just a check if end with null caracter
				if (!ft_strncmp(env[i], *args, ft_strlen(*args)) && (env[i][ft_strlen(*args)] == '=' || env[i][ft_strlen(*args)] == '\0'))
				{
					free(env[i]);
					j = i;
					while (env[j])
					{
						env[j] = env[j + 1];
						j++;
					}
				}
				i++;
			}
		}
		else
		{
			print_error_2("unset", *args ,"not a valid identifier");
			r = 1;
		}
		args++;
	}
	return (r);
}

ssize_t	ft_atoi_v2(char *s, bool *valid)
{
	int		sign;
	ssize_t	nb;

	sign = 1;
	*valid = true;
	nb = 0;
	while (*s && ft_strchr(WHITESPACES, *s))
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		if ((sign == 1 && (LONG_MAX / 10 < nb || LONG_MAX - (*s - '0') < nb * 10))
			|| (sign == -1 && (LONG_MIN / 10 > nb || LONG_MIN + (*s - '0') > nb * 10)))
		{
			*valid = false;
			return (0);
		}
		nb = nb * 10 + ((*s++ - '0') * sign); 
		if (sign == -1 && nb > 0)
			nb *= sign;
	}
	if (*s)
		*valid = false;
	return (nb);
}

bool	in_pipe(int	i)
{
	static int	p;

	if (i == 0)
		p = 0;
	else if (i == 1)
		p = 1;
	return (p);
}

int	ft_exit(t_node *tree, char **args)
{
	unsigned int	count;
	bool			valid;
	ssize_t			nb;

	if (!in_pipe(3))
		ft_putstr_fd("exit\n", 1);
	// exit was print into stderr but i change it to stdout for test script
	count = count_args(args);
	if (count == 0)
	{
		free_cmdtree(tree);
		double_free(environ);
		exit(exit_status(0, false, false) >> 8);
	}
	nb = ft_atoi_v2(*args, &valid);
	if (!*args || !valid)
	{
		print_error_2("exit", *args, "numeric argument required");
		free_cmdtree(tree);
		double_free(environ);
		exit(255);	
	}
	if (count > 1)
	{
		print_error("exit", "too many arguments");
		return (1);
	}
	exit(nb);
	return (0);
}
