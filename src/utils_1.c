/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:24:03 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/03 22:33:56 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	in_pipe(int i)
{
	static int	p;

	if (i == 0)
		p = 0;
	else if (i == 1)
		p = 1;
	return (p);
}

void	fix_env(char **env)
{
	char		*columns;
	char		*lines;
	static bool	flag;

	flag = true;
	columns = getenv("COLUMNS");
	lines = getenv("LINES");
	environ = env;
	if (lines)
		ft_setenv(environ, "LINES=", lines);
	if (columns)
		ft_setenv(environ, "COLUMNS=", columns);
}

bool	is_valid_variable_name(char *s)
{
	if (!s)
		return (false);
	if (!(ft_isalpha(*s) || *s == '_'))
		return (false);
	s++;
	while (*s)
	{
		if (!(ft_isalnum(*s) || *s == '_'))
			return (false);
		s++;
	}
	return (true);
}
