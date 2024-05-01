/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 19:43:24 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/29 19:46:50 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*join_variabl(char *old, char *new, char *ptr)
{
	char	*tmp;
	char	*res;
	char	*addplus;

	tmp = ft_strdup(ptr);
	if (!tmp)
		return (perror("malloc"), NULL);
	if (ft_strchr(old, '=') == NULL)
	{
		addplus = ft_strjoin(old, "=");
		if (!addplus)
			return (free(tmp), tmp = NULL, perror("malloc"), NULL);
		res = ft_strjoin(addplus, tmp);
	}
	else
		res = ft_strjoin(old, tmp);
	free(tmp);
	if (!res)
		return (free(addplus), perror("malloc"), NULL);
	free(old);
	free(new);
	return (res);
}

void	substr_print(char *s, char *e, int fd)
{
	if (!s || !e || s > e)
		return ;
	while (s != e)
		ft_putchar_fd(*s++, fd);
}

void	print_sored_env(char **res)
{
	int		i;
	char	*eq;

	i = 0;
	while (res[i])
	{
		ft_putstr_fd("declare -x ", 1);
		eq = ft_strchr(res[i], '=');
		if (eq == NULL)
		{
			ft_putendl_fd(res[i++], 1);
			continue ;
		}
		substr_print(res[i], eq + 1, 1);
		if (!eq[1])
			ft_putstr_fd("\"\"", 1);
		else
		{
			ft_putchar_fd('"', 1);
			substr_print(eq + 1, &eq[ft_strlen(eq)], 1);
			ft_putchar_fd('"', 1);
		}
		ft_putchar_fd('\n', 1);
		i++;
	}
}

int	ft_printexport(char **env)
{
	int		size;
	char	**res;
	int		i;

	size = count_args(env);
	res = malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (perror("malloc"), 1);
	i = 0;
	while (i < size)
	{
		res[i] = env[i];
		i++;
	}
	res[i] = NULL;
	sort_2d_array(&res);
	print_sored_env(res);
	free(res);
	return (0);
}

int	ft_export(char **args)
{
	char	*ptr;
	char	*tmp;
	int		r;

	if (!count_args(args))
		return (ft_printexport(environ));
	else
	{
		r = 0;
		while (*args)
		{
			ptr = ft_strdup(*args);
			if (!ptr)
				return (perror("malloc"), 1);
			tmp = edit_env(ptr);
			if (tmp == NULL)
				return (free(ptr), 1);
			else if (tmp == (char *)42)
				r = 1;
			args++;
		}
	}
	return (r);
}
