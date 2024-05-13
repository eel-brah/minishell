/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-brah <eel-brah@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:23:06 by eel-brah          #+#    #+#             */
/*   Updated: 2024/05/13 13:01:16 by eel-brah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

static void	args_to_nargs(char **nptrs, char **tokens, char *arg)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		nptrs[i] = tokens[i];
		i++;
	}
	nptrs[i] = arg;
	nptrs[i + 1] = NULL;
}

static char	**first_realloc(char *arg, size_t size)
{
	char	**nptrs;

	nptrs = malloc(sizeof(char *) * size);
	if (!nptrs)
	{
		free(arg);
		return (NULL);
	}
	nptrs[0] = arg;
	nptrs[1] = NULL;
	return (nptrs);
}

char	**add_ptr_to_ptrs(char **ptrs, char *ptr)
{
	size_t	size;
	char	**nptrs;

	if (!ptr)
		return (NULL);
	size = 2;
	if (!ptrs)
		return (first_realloc(ptr, size));
	else
	{
		while (ptrs[size - 1])
			size++;
		nptrs = malloc(sizeof(char *) * (size + 1));
		if (!nptrs)
		{
			free(ptr);
			return (NULL);
		}
		args_to_nargs(nptrs, ptrs, ptr);
	}
	free(ptrs);
	return (nptrs);
}

static void	ft_atoi_v2_setup(int *sign, char **s)
{
	*sign = 1;
	while (**s && ft_strchr(WHITESPACES, **s))
		(*s)++;
	if (**s == '+' || **s == '-')
	{
		if (**s == '-')
			*sign = -1;
		(*s)++;
	}
}

ssize_t	ft_atoi_v2(char *s, bool *valid)
{
	int		sign;
	ssize_t	nb;

	*valid = true;
	nb = 0;
	ft_atoi_v2_setup(&sign, &s);
	while (*s >= '0' && *s <= '9')
	{
		if ((sign == 1 && (LONG_MAX / 10 < nb
					|| LONG_MAX - (*s - '0') < nb * 10))
			|| (sign == -1 && (LONG_MIN / 10 > nb
					|| LONG_MIN + (*s - '0') > nb * 10)))
		{
			*valid = false;
			return (0);
		}
		nb = nb * 10 + ((*s++ - '0') * sign);
		if (sign == -1 && nb > 0)
			nb *= sign;
	}
	if (*s)
		*valid = false;
	return (nb);
}
