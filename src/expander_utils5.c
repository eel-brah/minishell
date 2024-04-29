/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:04:04 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/29 16:34:54 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*no_name(t_elem *elem, char ***res, char *s)
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
	if (c == '*' && elem->qoute == 0)
		elem->wild = 1;
	tmp = no_name(elem, res, s);
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
