/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:37:05 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 22:44:51 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

static bool	ft_cd_set_target(char **args, char **target)
{
	char	*home;

	if (count_args(args) < 1)
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (false);
		}
		*target = home;
	}
	else
		*target = args[0];
	return (true);
}

int	ft_cd(char **args)
{
	char	*pwd;
	char	*current;
	char	*target;

	pwd = getenv("PWD");
	if (!ft_cd_set_target(args, &target))
		return (1);
	if (*target)
	{
		if (chdir(target))
			return (ft_putstr_fd("cd: ", 2), perror(target), 1);
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

static void	ft_echo_print_args(char **args)
{
	while (*args)
	{
		ft_putstr_fd(*args++, 1);
		if (*args)
			ft_putchar_fd(' ', 1);
	}
}

int	ft_echo(char **args)
{
	unsigned int	i;
	bool			new_line;

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
	ft_echo_print_args(args);
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
