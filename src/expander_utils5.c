/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:04:04 by amokhtar          #+#    #+#             */
/*   Updated: 2024/05/14 18:47:34 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*handle_spec_tild(t_elem *elem, char ***res, char *s)
{
	int	d;
	int	c;

	c = s[elem->i];
	d = s[elem->i + 1];
	if (s[elem->i] == '~' && elem->qoute == 0 && elem->i == 0
		&& (ft_isalpha(s[elem->i + 1])
			|| s[elem->i + 1] == '\0' || s[elem->i + 1] == '/'))
	{
		if (!home_set(&elem))
			return (free(elem->arr), d_free(*res), *res = NULL, NULL);
		return ((char *)1337);
	}
	else if (c == '$' && (elem->q == '\"' || elem->q == '\0' || elem->here_doc)
		&& is_exist(d, "*@#?$-!0") && elem->expand)
	{
		if (handle_dollar_special(s, &elem, elem->status) == NULL)
			return (free(elem->arr),
				d_free(*res), *res = NULL, perror("malloc "), NULL);
		return ((char *)1337);
	}
	return ((char *)42);
}

char	*handl_other_carac(t_elem *elem, char ***res, char *s)
{
	char	c;
	char	d;
	char	*tmp;

	c = s[elem->i];
	d = s[elem->i + 1];
	if (c == '*' && elem->qoute == 0 && elem->expand)
		elem->wild = 1;
	tmp = handle_spec_tild(elem, res, s);
	if (!tmp)
		return (NULL);
	else if (tmp == (char *)1337)
		return ((char *)42);
	if (c == '$' && (elem->q == '\"' || elem->q == '\0' || elem->here_doc))
	{
		if (handle_dollar(s, &res, &elem) == NULL)
			return (free(elem->arr), d_free(*res),
				*res = NULL, perror("malloc "), NULL);
	}
	else
	{
		if (set_caractere(elem, c) == NULL)
			return (free(elem->arr), d_free(*res), *res = NULL, NULL);
	}
	return ((char *)42);
}

char	*split_expand(t_elem **elem, char *s, char ****res)
{
	char	*exp;

	if ((*elem)->qoute == 0 && (*elem)->here_doc == 0)
	{
		(*elem)->tmp = alloc_for_expand_without_q(s, &elem);
		if ((*elem)->tmp == (char *)16)
			return (s);
		else if ((*elem)->tmp == NULL)
			return (NULL);
		exp = delete_quotes((*elem)->arr, 0, 0);
		if (!exp)
			return (NULL);
		(*elem)->tmp = handle_wild_in_dollar(exp, &res);
		if ((*elem)->tmp == NULL)
			return (NULL);
		ft_memset((*elem)->arr, 0, (*elem)->index);
		(*elem)->index = 0;
	}
	return (s);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

void	sort_2d_array(char ***res)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (res && (*res)[i] != NULL)
	{
		j = i + 1;
		while ((*res)[j] != NULL)
		{
			if (ft_strcmp((*res)[i], (*res)[j]) > 0)
			{
				tmp = (*res)[i];
				(*res)[i] = (*res)[j];
				(*res)[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
