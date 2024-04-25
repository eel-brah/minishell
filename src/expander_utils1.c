/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:49:36 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/25 18:25:31 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*patter_true(char *arr, char ***res, int *capacity, char *s)
{
	char	*arrt;
	char	**rest;
	int		index;
	int		i;

	(1) && (i = 0, index = 0);
	while (s[i])
	{
		if (index == *capacity - 1)
		{
			*capacity *= 2;
			arrt = ft_strrealloc2(arr, *capacity);
			if (!arrt)
				return (free(arr), NULL);
			arr = arrt;
		}
		arr[index++] = s[i++];
	}
	arrt = ft_strdup(arr);
	if (!arrt)
		return (free(arr), NULL);
	rest = ft_realloc(*res, arrt);
	if (!rest)
		return (free(arr), NULL);
	return (*res = rest, ft_memset(arr, 0, *capacity), (char *)42);
}

char	**match_pattern(char *pattern, int handle_quote, int flag, int capacity)
{
	DIR				*dir;
	struct dirent	*st;
	char			*arr;
	char			**res;

	(1) && (flag = 1, res = NULL);
	(1) && (dir = opendir("."), arr = malloc(capacity));
	if (!arr)
		return (NULL);
	(1) && (ft_memset(arr, 0, capacity), st = readdir(dir));
	while (st != NULL)
	{
		if (!(st->d_name[0] == '.' && pattern[0] != '.')
			&& check_pattern(pattern, st->d_name, handle_quote) == 1)
		{
			flag = 0;
			if (!patter_true(arr, &res, &capacity, st->d_name))
				return (closedir(dir), NULL);
		}
		st = readdir(dir);
	}
	if (flag)
		return (closedir(dir), free(arr), (char **)42);
	return (closedir(dir), sort_2d_array(&res), free(arr), res);
}

char	*check_alloc(char *s, t_elem ***elem)
{
	if ((s[((**elem)->i)] == '\'' || s[((**elem)->i)] == '\"' )
		&& ((**elem)->qoute == 0 || (**elem)->q == s[((**elem)->i)]))
	{
		if ((**elem)->qoute == 0)
		{
			(**elem)->q = s[((**elem)->i)];
			(**elem)->qoute = 1;
		}
		else
		{
			(**elem)->qoute = 0;
			(**elem)->q = '\0';
		}
		if (set_caractere(**elem, s[((**elem)->i)]) == NULL)
			return (NULL);
		((**elem)->i)++;
	}
	else
	{
		if (set_caractere(**elem, s[((**elem)->i)]) == NULL)
			return (NULL);
		(((**elem)->i))++;
	}
	return ((char *)42);
}

char	*alloc_for_expand_without_q(char *s, t_elem ***elem)
{
	int		qoute;
	int		q;
	int		tmp;
	int		tmp1;
	int		tmp2;

	(1) && (qoute = (**elem)->qoute, q = (**elem)->q);
	(1) && (tmp = (**elem)->i, tmp1 = (**elem)->index, tmp2 = tmp1);
	((**elem)->i)++;
	while (s[((**elem)->i)] != '\0'
		&& !is_exist(s[((**elem)->i)], "\t\n\v\f\r  "))
	{
		if (s[((**elem)->i)] == '$')
		{
			(1) && ((**elem)->qoute = qoute, (**elem)->q = q);
			(**elem)->index = tmp1;
			while (tmp1 < tmp2)
				(**elem)->arr[tmp1++] = 0;
			(**elem)->i = tmp;
			return ((char *)16);
		}
		if (!check_alloc(s, elem))
			return (NULL);
	}
	return (((**elem)->i)--, (char *)42);
}

bool	handle_child_get_pid(char *file)
{
	char	**arr;
	int		or;

	arr = malloc(sizeof(char *) * 4);
	if (!arr)
		return (perror("malloc"), free(file), false);
	arr[0] = ft_strdup("/bin/bash");
	if (!arr[0])
		return (perror("malloc"), free(file), d_free(arr), exit(1), false);
	arr[1] = ft_strdup("-c");
	if (!arr[1])
		return (perror("malloc"), free(file), d_free(arr), exit(1), false);
	arr[2] = ft_strdup("ps -j $$ | awk 'NR==2 {print $3}'");
	if (!arr[2])
		return (perror("malloc"), free(file), d_free(arr), exit(1), false);
	arr[3] = NULL;
	or = open(file, O_CREAT | O_RDWR, PREMISSIONS);
	if (or == -1)
		return (perror("open"), free(file), d_free(arr), exit(1), false);
	if (dup2(or, 1) == -1)
		return (close(or), perror("dup2"), free(file), d_free(arr), exit(1), 0);
	close(or);
	if (execve("/bin/bash", arr, environ) == -1)
		return (perror("exceve"), free(file), d_free(arr), exit(1), false);
	return (true);
}
