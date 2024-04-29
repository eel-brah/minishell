/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amokhtar <amokhtar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:30:24 by amokhtar          #+#    #+#             */
/*   Updated: 2024/04/29 13:39:53 by amokhtar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/msh.h"

char	*intial_struct(t_elem *elem, char **word, char ***res, int here_doc)
{
	elem->q = '\0';
	elem->here_doc = here_doc;
	elem->qoute = 0;
	*word = NULL;
	*res = NULL;
	elem->index = 0;
	elem->i = 0;
	elem->wild = 0;
	elem->capacity = 1024;
	elem->arr = malloc(elem->capacity);
	if (!elem->arr)
		return (perror("malloc"), NULL);
	ft_memset(elem->arr, 0, elem->capacity);
	return ((char *)42);
}

char	*update_quote(char *s, t_elem *elem)
{
	if (elem->qoute == 0)
	{
		elem->q = s[elem->i];
		elem->qoute = 1;
	}
	else
	{
		elem->qoute = 0;
		elem->q = '\0';
	}
	if (set_caractere(elem, s[elem->i]) == NULL)
		return (NULL);
	return ((char *) 42);
}

char	*handle_last(t_elem *elem, char ***res, char *word)
{
	if (elem->index > 0)
	{
		if (alloc_for_elem(elem, elem->here_doc, word, res) == NULL)
			return (NULL);
	}
	else
	{
		if (!*res)
		{
			*res = malloc(sizeof(char *) * 1);
			if (!*res)
				return (free(elem->arr), NULL);
			(*res)[0] = NULL;
		}
	}
	return ((char *)42);
}

char	*handle_o_char_sp(t_elem *elem, char *s, char *word, char ***res)
{
	if (is_exist(s[elem->i], "\t\n\v\f\r ")
		&& elem->qoute == 0 && elem->index != 0 && !elem->here_doc)
	{
		if (alloc_for_elem(elem, elem->here_doc, word, res) == NULL)
			return (NULL);
	}
	else if (is_exist(s[elem->i], "\t\n\v\f\r ") && elem->here_doc == 1)
	{
		if (set_caractere(elem, s[elem->i]) == NULL)
			return (NULL);
	}
	else if (!is_exist(s[elem->i], "\t\n\v\f\r ") || elem->qoute == 1)
	{
		if (handl_other_carac(elem, res, s) == NULL)
			return (NULL);
	}
	return (s);
}

char	**expander(char *s, int here_doc, int expand, int status)
{
	char	**res;
	char	*word;
	t_elem	elem;

	if (intial_struct(&elem, &word, &res, here_doc) == NULL)
		return (NULL);
	(1) && (elem.expand = expand, elem.status = status);
	while (s && s[elem.i])
	{
		if ((s[elem.i] == '\'' || s[elem.i] == '\"' )
			&& (elem.qoute == 0 || elem.q == s[elem.i]))
		{
			if (update_quote(s, &elem) == NULL)
				return (free(elem.arr), d_free(res), NULL);
		}
		else
		{
			if (handle_o_char_sp(&elem, s, word, &res) == NULL)
				return (NULL);
		}
		elem.i++;
	}
	if (handle_last(&elem, &res, word) == NULL)
		return (NULL);
	return (free(elem.arr), res);
}
