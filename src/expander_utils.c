/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:32:26 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/25 16:48:05 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

void	*alloc_with_assing(int size, int *len, int *f, int *q)
{
	char	*res;

	res = malloc(size);
	if (!res)
		return (NULL);
	*len = 0;
	*f = 1;
	*q = '\0';
	return (res);
}

char	*alloc_without_quotes(char *s, int len, int i, int qo)
{
	int		q;
	char	*res;
	int		f;

	res = alloc_with_assing(len + 1, &len, &f, &q);
	if (!res)
		return (NULL);
	while (s && s[i])
	{
		if (s[i] == 20 && f == 1)
			(1) && (i++, f = 0);
		else if (s[i] == 20 && f == 0)
			(1) && (i++, f = 1);
		else if ((s[i] == '\'' || s[i] == '\"' ) && (qo == 0 || q == s[i]) && f)
		{
			if (qo == 0)
				(1) && (qo = 1, q = s[i]);
			else
				(1) && (qo = 0, q = '\0');
			i++;
		}
		else
			(1) && (res[len++] = s[i], i++);
	}
	return (res[len] = '\0', res);
}

char	*delete_quotes(char *s, int i, int len)
{
	int		qo;
	int		q;
	int		f;

	(1) && (qo = 0, q = '\0', f = 1);
	while (s && s[i])
	{
		if (s[i] == 20 && f == 1)
			(1) && (i++, f = 0);
		else if (s[i] == 20 && f == 0)
			(1) && (i++, f = 1);
		else if ((s[i] == '\'' || s[i] == '\"' ) && (qo == 0 || q == s[i]) && f)
		{
			if (qo == 0)
				(1) && (qo = 1, q = s[i]);
			else
				(1) && (qo = 0, q = '\0');
			i++;
			continue ;
		}
		else
			(1) && (i++, len++);
	}
	return (alloc_without_quotes(s, len, 0, 0));
}

bool	itterate_pattern(t_wild_patt *pa, char **p, char **name, int h)
{
	if ((**p == '"' || **p == '\'') && (pa->quote == 0 || pa->q == **p) && h)
	{
		if (pa->quote == 0)
			(1) && (pa->quote = 1, pa->q = **p);
		else
			(1) && (pa->quote = 0, pa->q = '\0');
		return ((*p)++, true);
	}
	if (**p == '*' && pa->quote == 0)
		(1) && (pa->wild = (*p)++, pa->tmp = *name + 1);
	else if (**p == **name)
		(1) && ((*p)++, (*name)++);
	else
	{
		if (pa->wild)
		{
			*p = pa->wild + 1;
			if (**p == pa->q && pa->quote == 1)
				(1) && (pa->quote = 0, pa->q = '\0');
			*name = (pa->tmp)++;
		}
		else
			return (false);
	}
	return (true);
}

int	check_pattern(char *pattern, char *name, int handle_quot)
{
	t_wild_patt	patt;

	patt.quote = 0;
	patt.tmp = NULL;
	patt.q = '\0';
	patt.wild = NULL;
	while (*name)
	{
		if (!itterate_pattern(&patt, &pattern, &name, handle_quot))
			return (0);
	}
	if (patt.quote && *pattern == patt.q)
		(1) && (pattern++, patt.quote = 0);
	while (*pattern == '\'' || *pattern == '"')
		pattern++;
	while (*pattern == '*' && patt.quote == 0)
		pattern++;
	return (*pattern == '\0');
}
