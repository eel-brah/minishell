/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:37:05 by eel-brah          #+#    #+#             */
/*   Updated: 2024/03/24 15:19:44 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

int	ft_cd(char **args)
{
	// PWD OLDPWD
	char			*home;
	char			*target;

	if (count_args(args) < 1)
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
	ft_printenv(env);
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

// int	ft_unset(char **env, char **args)
// {
// 	unsigned int	i;
// 	unsigned int	j;
// 	unsigned int	r;

// 	r = 0;
// 	while (*args)
// 	{
// 		if (is_valid_variable_name(*args))
// 		{
// 			i = 0;
// 			while (env[i])
// 			{
// 				if (!ft_strncmp(env[i], *args, ft_strlen(*args)) 
// 					&& env[i][ft_strlen(*args)] == '=')
// 				{
// 					free(env[i]);
// 					j = i;
// 					while (env[j])
// 					{
// 						env[j] = env[j + 1];
// 						j++;
// 					}
// 				}
// 				i++;
// 			}
// 		}
// 		else
// 		{
// 			print_error_2("unset", *args ,"not a valid identifier");
// 			r = 1;
// 		}
// 		args++;
// 	}
// 	return (r);
// }

