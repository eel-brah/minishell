/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:37:05 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/13 21:44:49 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

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

static void	ft_unset_move(char **env, unsigned int i, char **args)
{
	unsigned int	j;
	char			*ptr;

	if (!ft_strncmp(env[i], *args, ft_strlen(*args))
		&& (env[i][ft_strlen(*args)] == '='
		|| env[i][ft_strlen(*args)] == '\0'))
	{
		j = i;
		ptr = env[i];
		while (env[j])
		{
			env[j] = env[j + 1];
			j++;
		}
		free(ptr);
	}
}

int	ft_unset(char **env, char **args)
{
	unsigned int	i;
	unsigned int	r;

	r = 0;
	while (*args)
	{
		if (is_valid_variable_name(*args))
		{
			i = 0;
			while (env[i])
			{
				ft_unset_move(env, i, args);
				i++;
			}
		}
		else
		{
			print_error_2("unset", *args, "not a valid identifier");
			r = 1;
		}
		args++;
	}
	return (r);
}

static int	ft_exit_2(t_node *tree, char *arg, bool *valid)
{
	char	*p;
	int		nb;

	*valid = true;
	p = ft_strtrim(arg, WHITESPACES);
	if (!p)
	{
		*valid = false;
		perror("malloc");
		return (0);
	}
	nb = ft_atoi_v2(p, valid);
	if (!p || !*valid)
	{
		free(p);
		print_error_2("exit", arg, "numeric argument required");
		free_cmdtree(tree);
		double_free(environ);
		exit(255);
	}
	free(p);
	return (nb);
}

int	ft_exit(t_node *tree, char **args)
{
	unsigned int	count;
	bool			valid;
	ssize_t			nb;

	if (!in_pipe(3))
		ft_putstr_fd("exit\n", 2);
	count = count_args(args);
	if (count == 0)
	{
		free_cmdtree(tree);
		double_free(environ);
		exit(exit_status(0, false, false) >> 8);
	}
	nb = ft_exit_2(tree, *args, &valid);
	if (!valid)
		return (1);
	if (count > 1)
	{
		print_error("exit", "too many arguments");
		return (1);
	}
	free_cmdtree(tree);
	double_free(environ);
	exit(nb);
	return (0);
}
