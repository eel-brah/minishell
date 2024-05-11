/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 19:01:02 by amokhtar          #+#    #+#             */
/*   Updated: 2024/05/11 12:36:30 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

bool	check_exp(char **e, char *vr, int i)
{
	if (!ft_strncmp(e[i], vr, ft_strlen(vr))
		&& e[i][ft_strlen(vr)] == '=')
	{
		free(vr);
		return (true);
	}
	else if (!ft_strncmp(e[i], vr, ft_strlen(vr))
		&& e[i][ft_strlen(vr)] == '\0')
	{
		free(vr);
		return (true);
	}
	return (false);
}

char	*handle_exp(char *pt, char *vr, int i)
{
	char	**e;

	e = environ;
	while (e[i])
	{
		if (pt)
		{
			if (!ft_strncmp(e[i], vr, pt - vr + 1)
				|| (!ft_strncmp(e[i], vr, pt - vr) && e[i][pt - vr] == '\0'))
				return (free (e[i]), e[i] = vr, (char *)1337);
			else if (!ft_strncmp(e[i], vr, pt - vr - 1)
				&& vr[pt - vr - 1] == '+')
			{
				e[i] = join_variabl(e[i], vr, pt + 1);
				if (e[i] == NULL)
					return (NULL);
				return ((char *)1337);
			}
		}
		if (check_exp(e, vr, i))
			return ((char *)1337);
		i++;
	}
	return ((char *)42);
}

char	*edit_env(char *vrbl)
{
	char	*ptr;
	char	*tmp;
	int		v;
	int		i;

	if (is_valid_variable(vrbl, &v))
	{
		ptr = ft_strchr(vrbl, '=');
		i = 0;
		tmp = handle_exp(ptr, vrbl, i);
		if (tmp == NULL)
			return (NULL);
		else if (tmp == (char *)1337)
			return ((char *)1337);
		if (v == 1 && !add_to_env(vrbl))
			return (NULL);
		if (v == 2 && !add_to_env_plus(vrbl))
			return (NULL);
	}
	else
	{
		print_error_2("export", vrbl, "not a valid identifier");
		return (free(vrbl), (char *)42);
	}
	return ((char *)1337);
}
