/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 09:54:07 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/22 12:26:31 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	env_is_there(char **env, char *name)
{
	int	i;
	int	len;

	i = 0;
	while (env[i])
	{
		len = ft_strlen(name);
		if (ft_strncmp(env[i], name, len) == 0)
			return (true);
		i++;
	}
	return (false);	
}
bool	update_env(char **env, char *name, char *val)
{
	int		i;
	int		len;
	char	*res;

	i = 0;
	(void)env;
	while (environ[i])
	{
		len = ft_strlen(name);
		if (ft_strncmp(environ[i], name, len) == 0)
		{
			free(environ[i]);
			res = ft_strjoin(name, val);
			if (!res)
				return (false);
			environ[i] = res;
			return (true);			
		}
		i++;
	}
	return (true);
}
bool	add_new_env(char **env, char *name, char *val)
{
	int		count;
	int		i;
	char	*res;
	char	**tmp;
	
	count = 0;
	while (env[count])
		count++;
	tmp = malloc(sizeof(char *) * (count + 2));
	if (!tmp)
		return (false);
	i = 0;
	while (env[i])
	{
		tmp[i] = env[i];
		i++;
	}
	res = ft_strjoin(name, val);
	if (!res)
		return (false);
	tmp[i++] = res;
	tmp[i] = NULL;
	free(environ);
	environ = tmp;
	return (true);
}

bool	ft_setenv(char **env, char *name, char *val)
{
	if (env_is_there(env, name))
	{
		if (update_env(env, name, val) == false)
			return (perror("malloc"), false);
	}
	else
	{
		if (add_new_env(env, name, val) == false)
			return (perror("malloc"), false);
	}
	return (true);
}