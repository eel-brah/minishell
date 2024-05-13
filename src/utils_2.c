/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:28:00 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/13 21:40:40 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

void	d_free(char **ptr)
{
	char	**tmp;

	tmp = ptr;
	while (tmp && *tmp)
		free(*tmp++);
	free(ptr);
}

int	nb_without_empty(char **env)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '=') != NULL)
			res++;
		i++;
	}
	return (res);
}

char	**env_without_empty(char **env)
{
	int		i;
	int		size;
	int		index;
	char	**res;

	size = nb_without_empty(env);
	res = malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (NULL);
	i = 0;
	index = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '=') != NULL)
			res[index++] = env[i];
		i++;
	}
	res[index] = NULL;
	return (res);
}

void	ft_printenv_no_empty(char **env)
{
	while (env && *env)
	{
		if (ft_strchr(*env, '=') != NULL)
			printf("%s\n", *env);
		env++;
	}
}

char	*read_heredoc_line(void)
{
	char	**env;
	char	*s;

	env = environ;
	s = readline("> ");
	environ = env;
	return (s);
}
