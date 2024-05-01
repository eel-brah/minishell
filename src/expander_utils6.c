/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:04:04 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/29 18:50:00 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*set_caractere(t_elem *elem, int c)
{
	char	*tmp;

	if (elem->index == elem->capacity - 1)
	{
		elem->capacity *= 2;
		tmp = ft_strrealloc2(elem->arr, elem->capacity);
		if (!tmp)
			return (perror("malloc"), NULL);
		elem->arr = tmp;
	}
	elem->arr[elem->index] = c;
	elem->index++;
	return ((char *)42);
}

int	is_alpha_num(int c)
{
	if (c <= 'z' && c >= 'a')
		return (1);
	if (c <= 'Z' && c >= 'A')
		return (1);
	if (c <= '9' && c >= '0')
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

static char	**handle_case(char **lines, char *line)
{
	lines = malloc(sizeof(char *) * 2);
	if (!lines)
		return (NULL);
	lines[0] = line;
	lines[1] = NULL;
	return (lines);
}

int	is_exist(int c, char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i++] == c)
			return (1);
	}
	return (0);
}

char	**ft_realloc(char **lines, char *line)
{
	int		i;
	char	**tmp;

	if (!lines)
		return (handle_case(lines, line));
	i = 0;
	tmp = lines;
	while (lines[i])
		i++;
	lines = malloc(sizeof(char *) * (i + 2));
	if (!lines)
		return (NULL);
	i = 0;
	while (tmp[i])
	{
		lines[i] = tmp[i];
		i++;
	}
	lines[i] = line;
	lines[i + 1] = NULL;
	free(tmp);
	return (lines);
}
