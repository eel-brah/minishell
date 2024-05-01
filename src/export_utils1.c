/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 19:46:16 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/29 19:46:53 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

int	is_valid_variable(char *s, int *plus)
{
	*plus = 0;
	if (!s)
		return (0);
	if (!(ft_isalpha(*s) || *s == '_'))
		return (0);
	s++;
	while (*s && *s != '=')
	{
		if ((*s == '+' && *(s + 1) == '='))
			return (*plus = 2, 2);
		if (!(ft_isalnum(*s) || *s == '_'))
			return (0);
		s++;
	}
	return (*plus = 1, 1);
}

bool	add_to_env(char *vrbl)
{
	char	**new_env;
	char	**env_ptr;
	size_t	size;
	size_t	i;

	env_ptr = environ;
	size = count_args(env_ptr);
	new_env = malloc((size + 2) * sizeof(char *));
	if (!new_env)
	{
		perror("malloc");
		return (false);
	}
	i = 0;
	while (i < size)
	{
		new_env[i] = env_ptr[i];
		i++;
	}
	new_env[i] = vrbl;
	new_env[i + 1] = NULL;
	free(env_ptr);
	environ = new_env;
	return (true);
}

char	*alloc_without_plus(char *vrbl)
{
	char	*res;
	int		len;
	int		flag;
	int		i;
	int		index;

	flag = 1;
	i = 0;
	index = 0;
	len = ft_strlen(vrbl);
	res = malloc(sizeof(char) * len);
	if (!res)
		return (perror("malloc"), NULL);
	while (vrbl[i])
	{
		if (vrbl[i] == '+' && flag)
		{
			flag = 0;
			i++;
		}
		res[index++] = vrbl[i++];
	}
	res[index] = '\0';
	return (res);
}

bool	add_to_env_plus(char *vrbl)
{
	char	**new_env;
	char	**env_ptr;
	size_t	size;
	size_t	i;

	env_ptr = environ;
	size = count_args(env_ptr);
	new_env = malloc((size + 2) * sizeof(char *));
	if (!new_env)
		return (perror("malloc"), false);
	i = 0;
	while (i < size)
	{
		new_env[i] = env_ptr[i];
		i++;
	}
	new_env[i] = alloc_without_plus(vrbl);
	if (new_env[i] == NULL)
		return (perror("malloc"), false);
	new_env[i + 1] = NULL;
	free(env_ptr);
	free(vrbl);
	environ = new_env;
	return (true);
}
