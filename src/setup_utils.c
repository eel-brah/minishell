/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:11:56 by amokhtar          #+#    #+#             */
/*   Updated: 2024/05/14 13:24:31 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	is_all_digit(char *s)
{
	while (s && *s)
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}

bool	handl_shlvl(char **spl, char **ptr, int i)
{
	int		shlvl;
	char	*tmp;

	shlvl = ft_atoi(spl[1]) + 1;
	if (shlvl == 1000)
		ptr[i] = ft_strdup("SHLVL=");
	else if (shlvl > 1000)
	{
		print_error("minishell",
			"warning: shell level is too high, resetting to 1");
		ptr[i] = ft_strdup("SHLVL=1");
	}
	else
	{
		tmp = ft_itoa(shlvl);
		if (!tmp)
			return (perror("malloc"), d_free(spl), false);
		ptr[i] = ft_strjoin("SHLVL=", tmp);
		free(tmp);
	}
	if (!ptr[i])
		return (perror("malloc"), false);
	return (true);
}

bool	handle_shlvl(char *val, char **ptr, int i)
{
	char	**spl;
	int		c;

	spl = ft_split(val, '=');
	if (!spl)
		return (perror("malloc"), false);
	c = count_args(spl);
	if (c > 2 || (c == 2 && !is_all_digit(spl[1])) || c == 1)
	{
		ptr[i] = ft_strdup("SHLVL=1");
		if (!ptr[i])
			return (perror("malloc"), d_free(spl), false);
	}
	else if (c == 2)
	{
		if (!handl_shlvl(spl, ptr, i))
			return (d_free(spl), false);
	}
	d_free(spl);
	return (true);
}

char	**create_env(char **env, size_t size, int add, size_t i)
{
	char	**ptr;

	size = count_args(env);
	if (size == 0)
		return (create_new_env());
	check_path_pwd(env, &i, &add);
	ptr = malloc((size + 1 + add) * sizeof(env));
	if (!ptr)
		return (perror("malloc"), NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
		{
			if (!handle_shlvl(env[i], ptr, i))
				return (perror("malloc"), double_free(ptr), NULL);
		}
		else
			ptr[i] = ft_strdup(env[i]);
		if (!ptr[i])
			return (perror("malloc"), double_free(ptr), NULL);
		i++;
	}
	if (!handl_path_pwd(env, &i, ptr))
		return (NULL);
	return (ptr[i] = NULL, ptr);
}

char	**create_new_env(void)
{
	char	**env;
	char	*tmp1;
	char	*tmp;

	env = malloc(sizeof(char *) * 5);
	if (!env)
		return (perror("malloc"), NULL);
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (perror("getcwd"), NULL);
	tmp1 = ft_strjoin("PWD=", tmp);
	(1) && (free(tmp), env[0] = tmp1);
	tmp = ft_strdup("SHLVL=1");
	if (!tmp || !tmp1)
		return (perror("malloc"), free(tmp), d_free(env), NULL);
	env[1] = tmp;
	tmp = ft_strdup("_=/usr/bin/minishell");
	if (!tmp)
		return (perror("malloc"), d_free(env), NULL);
	env[2] = tmp;
	tmp = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	if (!tmp)
		return (perror("malloc"), d_free(env), NULL);
	env[3] = tmp;
	return (env[4] = NULL, env);
}
